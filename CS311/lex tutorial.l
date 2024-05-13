%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
%}

%%
#include {printf("<Preprocessor Directive, include>\n");}
["<][A-Za-z]+[.][h][">]  {printf("<Header File, %s>\n", yytext);}
int|char|float|bool|void|if|else|while|for|case|switch|unsigned|long|do|return {printf("<keyword, %s>\n", yytext);}
true|false {printf("<bool, %s>\n", yytext);}
[+]?[-]?[0-9]+ {printf("<num, %s>\n", yytext);}
((\-?)(\+\-)*|(\+?)(\-\+)*)[0-9]*(\.?)[0-9]+ {printf("<float, %s>\n", yytext);}
\"[^\"]*\" {printf("<literal, %s>\n", yytext);}
[A-Za-z_][A-Za-z0-9_]* {printf("<id, %s>\n", yytext);}
==|!=|>|<|>=|<= {printf("<Assignment Operator, %s>\n", yytext);}
[(){}=;,] {printf("<%s>\n", yytext);}
[\-\+\*\/]  {printf("<Operators, %s>\n", yytext);}

%%

int yywrap() {}

int main(int argc, char **argv) {
    FILE *fp;
    char filename[50];
    printf("Enter the filename: \n");
    scanf("%s",filename);
    fp = fopen(filename,"r");
    yyin = fp;
    yylex();
    return 0;
}