%{
/* Definition section */
#include<stdio.h>
int flag=0;
int yyerror();
int yylex();
int store[26];
%}

%token NUMBER
%token VARIABLE

%left '>' '<' '=' ';' GE LE EQ NE

%left '+' '-'

%left '*' '/' '%'


/* Rule Section */
%%
statements: V {printf("First exp\n");}
	| V';'V {printf("Second exp\n");}
	;
V: VARIABLE'='ArithmeticExpression {;
};


ArithmeticExpression: E {

printf("\nResult=%d\n", $1);

};
E: E'+'E {$$=$1+$3;}
|E'-'E {$$=$1-$3;}
|E'*'E {$$=$1*$3;}
|E'/'E {$$=$1/$3;}
|E'%'E {$$=$1%$3;}
|'('E')' {$$=$2;}
|'-'E {$$=-1*$2;}
|E'>'E {$$=$1>$3;}
|E'<'E {$$=$1<$3;}
|E GE E {$$=$1>=$3;}
|E LE E {$$=$1<=$3;}
|E EQ E {$$=$1==$3;}
|E NE E {$$=$1!=$3;}
| VARIABLE 
| NUMBER


;


%%

//driver code
int main()
{
printf("\nEnter arithmetic expression:");
yyparse();
if(flag==0)
printf("\nEntered arithmetic expression is Valid\n");
else
printf("\nINVALID expression.\n");
return 0;
}

int yyerror()
{
flag=1;
return 0;
}
