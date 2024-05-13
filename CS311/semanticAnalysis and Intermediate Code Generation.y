%{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int yylex();
void yyerror(const char *s);
int flag = 0;
int var = 0;

struct sym {
    char type[100];
    int value;
    int length;
    int detect;
    int register1;
    int register2;
};

struct sym symbolTable[26];

struct Stack {
    int top;
    int items[100];
};

struct Stack registerStack;

void initializedStack() {
    registerStack.top = -1;
}

void push(int val) {
    registerStack.top++;
    registerStack.items[registerStack.top] = val;
}

int pop() {
    registerStack.top--;
    return registerStack.items[registerStack.top + 1];
}

int get1stElem() {
    return registerStack.items[registerStack.top];
}

int get2ndElem() {
    return registerStack.items[registerStack.top - 1];
}

int typeIdx = -1;
int regIdx = 1;
char* arrType[6] = {"int", "float", "boolean", "int[]", "float[]", "boolean[]"};
void printSymbolTable() {
    for (int i = 0; i < 26; i++) {
        if (symbolTable[i].detect == 1) {
            printf("Type of the variable ");
            printf("%d is ", i);
            printf("%s ", symbolTable[i].type);
            printf("and value is ");
            printf("%d\n", symbolTable[i].value);
        }
    }
}
%}

%union {
    int ival;
    float fval;
    char sval[100];
};

%token IF ELSE WHILE INT FLOAT BOOLEAN
%token <ival> NUMBER
%token <ival> VARIABLE
%type <ival> exp term factor
%start fnc
%left '>' '<' GE LE EQ NE '{' '}' '(' ')'
%left '*' '/' '%'
%left '+' '-'


%%

fnc : KEYWORD VARIABLE '(' ')' {initializedStack();} '{'declList stmlList'}' {printf("ret i32 0\n");};
KEYWORD : INT {typeIdx = 0;} | FLOAT {typeIdx = 1;} | BOOLEAN {typeIdx = 2;} ;
declList : declList decl | decl;
decl : KEYWORD varList ';' {};
varList : VARIABLE {strcpy(symbolTable[$1].type, arrType[typeIdx]); printf("%%%d = alloca i32, align 4\n", regIdx); symbolTable[$1].register1 = regIdx; regIdx++;} | VARIABLE {strcpy(symbolTable[$1].type, arrType[typeIdx]); printf("%%%d = alloca i32, align 4\n", regIdx); symbolTable[$1].register1 = regIdx; regIdx++;} ',' varList | VARIABLE '[' NUMBER ']' ',' varList {strcpy(symbolTable[$1].type, arrType[typeIdx + 3]); symbolTable[$1].length = $3; printf("Array type initialized is "); printf("%s\n", symbolTable[$1].type); printf("Length of the array initialized is "); printf("%d\n", symbolTable[$1].length);} | VARIABLE '[' NUMBER ']' {strcpy(symbolTable[$1].type, arrType[typeIdx + 3]); symbolTable[$1].length = $3; printf("Array type initialized is "); printf("%s\n", symbolTable[$1].type); printf("Length of the array initialized is "); printf("%d\n", symbolTable[$1].length);} ;
stmlList : stmlList stmt | stmt;
stmt : ifelseStmt | whileStmt | assignStmt;
ifelseStmt : IF '(' exp ';'')' '{' stmlList '}' ELSE '{' stmlList '}';

whileStmt : WHILE '(' exp ';' ')' '{' stmlList '}';

assignStmt : VARIABLE '=' exp ';' {symbolTable[$1].value = $3; symbolTable[$1].detect = 1; if (var == 1) {printf("store i32 %%%d, i32* %%%d, align 4\n", regIdx - 1, symbolTable[$1].register1); var = 0;} else {printf("store i32 %d, i32* %%%d, align 4\n", symbolTable[$1].value, symbolTable[$1].register1);}};
exp : exp '+' term {$$ = $1 + $3;} | exp '-' term {$$ = $1 - $3;} | term {$$ = $1;};
term : term '*' factor {$$ = $1 * $3; if (get1stElem() == -10 || get2ndElem() == -10) {printf("%%%d = mul nsw i32 %%%d, %d\n", regIdx, get2ndElem(), $3);} else {printf("%%%d = mul nsw i32 %%%d, %%%d\n", regIdx, get1stElem(), get2ndElem());} regIdx++;} | term '/' factor {$$ = $1 / $3;} | factor {$$ = $1;};
factor : VARIABLE {$$ = symbolTable[$1].value; printf("%%%d = load i32, i32* %%%d, align 4\n", regIdx, symbolTable[$1].register1); symbolTable[$1].register2 = regIdx; push(regIdx); regIdx++; var = 1;} | NUMBER{$$ = $1; push(-10);};

%%

void yyerror(const char *s) {
    flag = 1;
}

int main() {
    /* printf("Enter statement: \n"); */
    yyparse();
    if (flag == 1) printf("\nInvalid Expression\n");
    /* printSymbolTable(); */
    return 0;
}