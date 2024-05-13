%{
/* Definition section */
#include<stdio.h>
#include "samplec.tab.h"
extern int yylval;
%}

/* Rule Section */
%%

[a-zA-Z_]+[a-zA-Z0-9_]* {
yylval=yytext[0];
return VARIABLE;
}
[0-9]+ {
yylval=atoi(yytext);
return NUMBER;

}

[ \t]+ {}

"=" { return '=';}

";" { return ';';}

"+" { return '+';}

"-" { return '-';}

"*" { return '*';}

"/" { return '/';}

"%" { return '%';}

"(" { return '(';}

")" { return ')';}

">" { return '>';}

"<" { return '<';}

">=" { return GE;}

"<=" { return LE;}

"==" { return EQ;}

"!=" { return NE;}


[\n] {return 0;}

. {printf("\nInvalid character sequence found\n"); exit(0);}

%%

int yywrap()
{
return 1;
}
