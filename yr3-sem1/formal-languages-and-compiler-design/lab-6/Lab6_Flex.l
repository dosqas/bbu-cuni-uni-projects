%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lab6_Yacc.tab.h"

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

"dataset"   { return DATASET; }
"input"     { return INPUT; }
"filter"    { return FILTER; }
"where"     { return WHERE; }
"select"    { return SELECT; }
"count"     { return COUNT; }
"group"     { return GROUP; }
"by"        { return BY; }
"sort"      { return SORT; }
"asc"       { return ASC; }
"desc"      { return DESC; }
"output"    { return OUTPUT; }
"if"        { return IF; }
"end"       { return END; }
"for"       { return FOR; }
"in"        { return IN; }
"from"      { return FROM; }
"to"        { return TO; }
"and"       { return AND; }
"or"        { return OR; }
"len"       { return LEN; }
"avg"       { return AVG; }
"max"       { return MAX; }
"min"       { return MIN; }

"="         { return ASSIGN; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<="        { return LE; }
">="        { return GE; }
"<"         { return LT; }
">"         { return GT; }

"["         { return LBRACKET; }
"]"         { return RBRACKET; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
":"         { return COLON; }
","         { return COMMA; }
"."         { return DOT; }
"("         { return LPAREN; }
")"         { return RPAREN; }


{IDENTIFIER} { yylval.str = strdup(yytext); return IDENTIFIER; }
{NUMBER}     { yylval.str = strdup(yytext); return NUMBER; }
{STRING}     { yylval.str = strdup(yytext); return STRING; }

{WHITESPACE}            { /* ignore */ }

.                       { yyerror("Illegal character"); }

%%
