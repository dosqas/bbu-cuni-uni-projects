%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 211
#define MAX_PIF_ENTRIES 10000

unsigned int hash(const char* lexeme);
void ST_init();
void PIF_insert(int code, int st_index);
int ST_insert(const char *lexeme);
void ST_free();

void ST_print();
void PIF_print();

void yyerror(const char *s);

%}

%option yylineno
%option noyywrap

LETTER      [a-zA-Z]
DIGIT       [0-9]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
NUMBER      0|[1-9]{DIGIT}*
STRING      \"([^\"\n])*\"
WHITESPACE  [ \t\n\r]+

%%

"dataset"               { PIF_add(10, 0); } 
"input"                 { PIF_add(11, 0); }
"filter"                { PIF_add(12, 0); }
"where"                 { PIF_add(13, 0); }
"select"                { PIF_add(14, 0); }
"count"                 { PIF_add(15, 0); }
"group"                 { PIF_add(16, 0); }
"by"                    { PIF_add(17, 0); }
"sort"                  { PIF_add(18, 0); }
"asc"                   { PIF_add(19, 0); }
"desc"                  { PIF_add(20, 0); }
"output"                { PIF_add(21, 0); }
"if"                    { PIF_add(22, 0); }
"end"                   { PIF_add(23, 0); }
"for"                   { PIF_add(24, 0); }
"in"                    { PIF_add(25, 0); }
"from"                  { PIF_add(26, 0); }
"to"                    { PIF_add(27, 0); }
"and"                   { PIF_add(28, 0); }
"or"                    { PIF_add(29, 0); }
"len"                   { PIF_add(30, 0); }
"avg"                   { PIF_add(31, 0); }
"max"                   { PIF_add(32, 0); }
"min"                   { PIF_add(33, 0); }

"="                     { PIF_add(40, 0); }

"=="                    { PIF_add(41, 0); }
"!="                    { PIF_add(42, 0); }
"<="                    { PIF_add(44, 0); }
">="                    { PIF_add(46, 0); }
"<"                     { PIF_add(43, 0); }
">"                     { PIF_add(45, 0); }

"["                     { PIF_add(47, 0); }
"]"                     { PIF_add(48, 0); }
"{"                     { PIF_add(49, 0); }
"}"                     { PIF_add(50, 0); }
":"                     { PIF_add(51, 0); }
","                     { PIF_add(52, 0); }
"."                     { PIF_add(53, 0); }
"("                     { PIF_add(54, 0); }
")"                     { PIF_add(55, 0); }

{NUMBER} {
    // yytext matches the current token (in our case, the number)
    int st_index = ST_insert(yytext);
    PIF_add(100, st_index);
}

{STRING} {
    int st_index = ST_insert(yytext);
    PIF_add(101, st_index);
}

{IDENTIFIER} {
    int st_index = ST_insert(yytext);
    PIF_add(103, st_index);
}

{WHITESPACE}            { /* ignore */ }

.                       { yyerror("Illegal character"); }

%%

// ---- PIF STRUCTURE ----
typedef struct {
    int code;
    int st_index;
} PIFEntry;

PIFEntry PIF[MAX_PIF_ENTRIES];
int PIF_size = 0;

// ---- PIF FUNCTIONS ----
void PIF_insert(int code, int st_index) {
    if (PIF_size >= MAX_PIF_ENTRIES) {
        fprintf(stderr, "PIF overflow!\n");
        exit(1);
    }
    PIF[PIF_size].code = code;
    PIF[PIF_size].st_index = st_index;
    PIF_size++;
}

void PIF_print() {
    printf("\n---- PROGRAM INTERNAL FORM ----\n");
    for (int i = 0; i < PIF_size; i++) {
        printf("(%d, %d)\n", PIF[i].code, PIF[i].st_index);
    }
}

// ---- ST STRUCTURE ----
typedef struct SymbolEntry {
    char* lexeme;
    int index;
    struct SymbolEntry *next;
} SymbolEntry;

typedef struct {
    SymbolEntry *table[TABLE_SIZE];
    int next_index;
} SymbolTable;

SymbolTable ST;

// ---- ST FUNCTIONS ----
unsigned int hash(const char* lexeme) {
    unsigned int h = 0;
    while (*lexeme) {
        h = (h * 31 + *lexeme++) % TABLE_SIZE;
    }
    return h;
}

void ST_init() {
    for (int i = 0; i < TABLE_SIZE; i++)
        ST.table[i] = NULL;
    ST.next_index = 0;
}

int ST_insert(const char* lexeme) {
    unsigned int h = hash(lexeme);
    SymbolEntry *entry = ST.table[h];
    while (entry) {
        if (strcmp(entry->lexeme, lexeme) == 0)
            return entry->index;
        entry = entry->next;
    }
    SymbolEntry *new_entry = malloc(sizeof(SymbolEntry));
    new_entry->lexeme = strdup(lexeme);
    new_entry->index = ++ST.next_index;
    new_entry->next = ST.table[h];
    ST.table[h] = new_entry;
    return new_entry->index;
}

void ST_print() {
    printf("\n---- SYMBOL TABLE ----\n");

    SymbolEntry* all_entries[TABLE_SIZE * 10];
    int count = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        SymbolEntry *entry = ST.table[i];
        while (entry) {
            all_entries[count++] = entry;
            entry = entry->next;
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (all_entries[i]->index > all_entries[j]->index) {
                SymbolEntry *tmp = all_entries[i];
                all_entries[i] = all_entries[j];
                all_entries[j] = tmp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        printf("(%d, %s)\n", all_entries[i]->index, all_entries[i]->lexeme);
    }
}


void ST_free() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        SymbolEntry *entry = ST.table[i];
        while (entry) {
            SymbolEntry *tmp = entry;
            entry = entry->next;
            free(tmp->lexeme);
            free(tmp);
        }
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "Lexical Error (line %d): %s at '%s'\n", yylineno, s, yytext);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Error opening file");
            return 1;
        }
        yyin = f;
    }

    ST_init();

    printf("Starting lexical analysis...\n");
    yylex();
    printf("Lexical analysis complete.\n");

    PIF_print();
    ST_print();
    ST_free();
    return 0;
}
