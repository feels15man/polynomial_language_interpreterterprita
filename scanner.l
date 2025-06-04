%{
    #include "polynom_header.h"
    #include "y.tab.h"
    #include <stdio.h>
    extern void yyerror(const char *);
    extern FILE* yyin;
%}

%option noyywrap
%option yylineno

%%

[0-9]+  {yylval.ll_val = atoi(yytext); return NUMBER;}
\+      {yylval.ll_val = 1; return PLUS;}
\-      {yylval.ll_val = -1; return MINUS;}
\*      {return MUL;}
\^      {return POWER;}
\(      {return OP_BRACKET;}
\)      {return CL_BRACKET;}
\n      {return STATEMENT_END;}
:=      {yyerror("Its deprecated. Did you mean '=' as assign");}
\=      {return ASSIGN;}
echo    {return ECHO;}

\#.*    {;}
[a-z]{1}        {;yylval.char_val = yytext[0]; return VAR_POLY;}
[a-z]{2,}       {yyerror("several variables in a row");}
[A-Z]{1,15}     {memset(yylval.str_val, 0, MAX_VAR_NAME_LEN + 1); strncpy(yylval.str_val, yytext, yyleng); return VAR_NAME;}
[\+\*\-]{2,}    {yyerror("several operators in a row"); } 
[\r\t ]           {;}

<<EOF>>         {return EOF_T;}
.               {yyerror("Unknown char");}
%%
