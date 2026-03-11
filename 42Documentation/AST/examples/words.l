%{
#include <stdio.h>
%}

%%

"if"|"else"|"while"|"return"    printf("KEYWORD: %s\n", yytext);

[a-zA-Z][a-zA-Z0-9_]*          printf("IDENTIFIER: %s\n", yytext);

[0-9]+\.[0-9]+                  printf("FLOAT: %s\n", yytext);
[0-9]+                          printf("INTEGER: %s\n", yytext);

\"[^\"]*\"                      printf("STRING: %s\n", yytext);

"//"[^\n]*                      /* ignore single-line comments */

[[:blank:]|\n]                  /* ignore whitespace */

"="|"=="|"!="|"<"|">"           printf("OPERATOR: %s\n", yytext);

"{"                             printf("LBRACE\n");
"}"                             printf("RBRACE\n");

.                               printf("UNKNOWN: %c\n", *yytext);

%%
