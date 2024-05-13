%{
/* Definition section */
#include<stdio.h>
#include "temp.tab.h"
%}

/* Rule Section */
%%

if {return IF;}
else {return ELSE;}
while {return WHILE;}

int {return INT;}

float {return FLOAT;}

boolean {return BOOLEAN;}

[a-zA-Z_]+[a-zA-Z0-9_]* {
yylval.ival=yytext[0];
return VARIABLE;
}
[0-9]+ {
yylval.ival=atoi(yytext);
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

">" { return '>';}

"<" { return '<';}

">=" { return GE;}

"<=" { return LE;}

"==" { return EQ;}

"!=" { return NE;}

"{" {return '{';}
"}" {return '}';}
"," {return ',';}
"(" {return '(';}
")" {return ')';}
"[" {return '[';}
"]" {return ']';}

[\n] {}

# {return 0;}

. {printf("\nInvalid character sequence found\n"); exit(0);}

%%

int yywrap()
{
return 1;
}