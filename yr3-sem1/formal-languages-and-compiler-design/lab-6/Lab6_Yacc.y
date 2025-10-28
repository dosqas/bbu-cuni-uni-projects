%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex();
extern FILE *yyin;  // declare yyin so Bison knows about it
%}

%union {
    char* str;   // for identifiers and strings
    int num;     // for numbers
}

%debug

%token <str> IDENTIFIER STRING
%token <num> NUMBER

/* --- Tokens --- */
%token DATASET INPUT FILTER WHERE SELECT COUNT GROUP BY SORT ASC DESC OUTPUT
%token IF END FOR IN FROM TO AND OR LEN AVG MAX MIN
%token ASSIGN EQ NEQ LE GE LT GT
%token LBRACKET RBRACKET LBRACE RBRACE COLON COMMA DOT LPAREN RPAREN

%start program

%%

program:
      statement
    {
        printf("Applied: program -> statement\n");
    }
    | statement program
    {
        printf("Applied: program -> statement program\n");
    }
    ;

statement:
      dataset_init
    {
        printf("Applied: statement -> dataset_init\n");
    }
    | input_stmt
    {
        printf("Applied: statement -> input_stmt\n");
    }
    | filter_stmt
    {
        printf("Applied: statement -> filter_stmt\n");
    }
    | select_stmt
    {
        printf("Applied: statement -> select_stmt\n");
    }
    | count_stmt
    {
        printf("Applied: statement -> count_stmt\n");
    }
    | group_stmt
    {
        printf("Applied: statement -> group_stmt\n");
    }
    | sort_stmt
    {
        printf("Applied: statement -> sort_stmt\n");
    }
    | output_stmt
    {
        printf("Applied: statement -> output_stmt\n");
    }
    | if_stmt
    {
        printf("Applied: statement -> if_stmt\n");
    }
    | for_stmt
    {
        printf("Applied: statement -> for_stmt\n");
    }
    ;

dataset_init:
      DATASET IDENTIFIER ASSIGN dataset_literal
    {
        printf("Applied: dataset_init -> DATASET IDENTIFIER ASSIGN dataset_literal\n");
    }
    ;

input_stmt:
      INPUT IDENTIFIER
    {
        printf("Applied: input_stmt -> INPUT IDENTIFIER\n");
    }
    | INPUT IDENTIFIER FROM STRING
    {
        printf("Applied: input_stmt -> INPUT IDENTIFIER FROM STRING\n");
    }
    ;

filter_stmt:
      FILTER IDENTIFIER WHERE condition
    {
        printf("Applied: filter_stmt -> FILTER IDENTIFIER WHERE condition\n");
    }
    ;

select_stmt:
      SELECT IDENTIFIER field_list
    {
        printf("Applied: select_stmt -> SELECT IDENTIFIER field_list\n");
    }
    ;

count_stmt:
      COUNT IDENTIFIER
    {
        printf("Applied: count_stmt -> COUNT IDENTIFIER\n");
    }
    ;

group_stmt:
      GROUP IDENTIFIER BY field
    {
        printf("Applied: group_stmt -> GROUP IDENTIFIER BY field\n");
    }
    ;

sort_stmt:
      SORT IDENTIFIER order
    {
        printf("Applied: sort_stmt -> SORT IDENTIFIER order\n");
    }
    ;

output_stmt:
      OUTPUT field_or_id optional_to
    {
        printf("Applied: output_stmt -> OUTPUT field_or_id optional_to\n");
    }
    ;

field_or_id:
      IDENTIFIER
    {
        printf("Applied: field_or_id -> IDENTIFIER\n");
    }
    | field
    {
        printf("Applied: field_or_id -> field\n");
    }
    ;

optional_to:
      /* empty */
    {
        printf("Applied: optional_to -> (empty)\n");
    }
    | TO STRING
    {
        printf("Applied: optional_to -> TO STRING\n");
    }
    ;

if_stmt:
      IF condition statements END
    {
        printf("Applied: if_stmt -> IF condition statements END\n");
    }
    ;

for_stmt:
      FOR IDENTIFIER IN IDENTIFIER statements END
    {
        printf("Applied: for_stmt -> FOR IDENTIFIER IN IDENTIFIER statements END\n");
    }
    ;

statements:
      statement
    {
        printf("Applied: statements -> statement\n");
    }
    | statement statements
    {
        printf("Applied: statements -> statement statements\n");
    }
    ;

condition:
      expr comp_op expr
    {
        printf("Applied: condition -> expr comp_op expr\n");
    }
    | condition logical_op condition
    {
        printf("Applied: condition -> condition logical_op condition\n");
    }
    ;

expr:
      value
    {
        printf("Applied: expr -> value\n");
    }
    | field
    {
        printf("Applied: expr -> field\n");
    }
    | function LPAREN field RPAREN
    {
        printf("Applied: expr -> function LPAREN field RPAREN\n");
    }
    | function LPAREN IDENTIFIER RPAREN
    {
        printf("Applied: expr -> function LPAREN IDENTIFIER RPAREN\n");
    }
    ;

function:
      LEN
    {
        printf("Applied: function -> LEN\n");
    }
    | AVG
    {
        printf("Applied: function -> AVG\n");
    }
    | MAX
    {
        printf("Applied: function -> MAX\n");
    }
    | MIN
    {
        printf("Applied: function -> MIN\n");
    }
    | COUNT
    {
        printf("Applied: function -> COUNT\n");
    }
    ;

dataset_literal:
      LBRACKET record_list RBRACKET
    {
        printf("Applied: dataset_literal -> LBRACKET record_list RBRACKET\n");
    }
    ;

record_list:
      record
    {
        printf("Applied: record_list -> record\n");
    }
    | record COMMA record_list
    {
        printf("Applied: record_list -> record COMMA record_list\n");
    }
    ;

record:
      LBRACE field_assignments RBRACE
    {
        printf("Applied: record -> LBRACE field_assignments RBRACE\n");
    }
    ;

field_assignments:
      field COLON value
    {
        printf("Applied: field_assignments -> field COLON value\n");
    }
    | field COLON value COMMA field_assignments
    {
        printf("Applied: field_assignments -> field COLON value COMMA field_assignments\n");
    }
    ;

field_list:
      field
    {
        printf("Applied: field_list -> field\n");
    }
    | field COMMA field_list
    {
        printf("Applied: field_list -> field COMMA field_list\n");
    }
    ;

field:
      IDENTIFIER
    {
        printf("Applied: field -> IDENTIFIER\n");
    }
    | IDENTIFIER DOT field
    {
        printf("Applied: field -> IDENTIFIER DOT field\n");
    }
    ;

value:
      STRING
    {
        printf("Applied: value -> STRING\n");
    }
    | NUMBER
    {
        printf("Applied: value -> NUMBER\n");
    }
    ;

comp_op:
      EQ
    {
        printf("Applied: comp_op -> EQ\n");
    }
    | NEQ
    {
        printf("Applied: comp_op -> NEQ\n");
    }
    | LT
    {
        printf("Applied: comp_op -> LT\n");
    }
    | LE
    {
        printf("Applied: comp_op -> LE\n");
    }
    | GT
    {
        printf("Applied: comp_op -> GT\n");
    }
    | GE
    {
        printf("Applied: comp_op -> GE\n");
    }
    ;

logical_op:
      AND
    {
        printf("Applied: logical_op -> AND\n");
    }
    | OR
    {
        printf("Applied: logical_op -> OR\n");
    }
    ;

order:
      ASC
    {
        printf("Applied: order -> ASC\n");
    }
    | DESC
    {
        printf("Applied: order -> DESC\n");
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Error opening file");
            return 1;
        }
        yyin = f;  // flex input file
    }

    // extern int yydebug;   // declare the Bison debug variable
    // yydebug = 1;          // turn on debug printing

    printf("Starting lexical and syntactic analysis...\n");
    yyparse();  // Bison parser calls yylex internally
    printf("Parsing complete.\n");

    return 0;
}
