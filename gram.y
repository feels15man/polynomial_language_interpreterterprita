%{
    #include <stdio.h>
    #include <math.h>
    #include <stdlib.h>
    #include "polynom_header.h"
    #include "functions.h"

    extern int yylex();

    extern FILE *yyin;
    extern int yylineno;
    extern char* yytext;

    VARIABLE var_array[MAX_VAR_COUNT];

    void yyerror(const char* s){
        printf("[ERROR]: %s in line %d\n", s, yylineno); 
        printf("text that caused error: '%s'\n", yytext);
        exit(1);
    }

%}

%union{
    long long int ll_val;
    char char_val;
    char str_val[MAX_VAR_NAME_LEN + 1];
    MONOMIAL_STRUCT monom_val;
    POLYNOMIAL_STRUCT *polynom_val;
}

%start main

%token ASSIGN
%token <str_val> VAR_NAME // Name of variable that uses in file NOT IN POLYNOM
%token ECHO
%token MUL 
%token POWER 
%token <ll_val> NUMBER PLUS MINUS 
/* %token EQ */
%token <char_val> VAR_POLY // x y z ... Variable of polynom
%token OP_BRACKET
%token CL_BRACKET
%token STATEMENT_END
%token EOF_T

%type <polynom_val> math_expression t_expression 
%type <polynom_val> f_expression p_expression 
%type <ll_val> sign assignment

%left PLUS MINUS
%left MUL 
%right POWER
%left OP_BRACKET CL_BRACKET

%%
main: 
    /*| statement STATEMENT_END main */
    | main STATEMENT_END 
    | main statement STATEMENT_END 
    | main EOF_T {return 0;}

statement:
    math_expression {printf("math: "); 
        #ifdef DEBUG 
        print_polynom($1); 
        #endif 
        free($1);}
    | ECHO math_expression {
        #ifdef DEBUG 
        printf("ECHO !!!!!\n"); 
        #endif 
        print_polynom($2);
        }
    | assignment {;}
    
assignment:
    VAR_NAME ASSIGN math_expression {$$ = 0; var_set_value(var_array, $1, $3);}

math_expression:
    math_expression sign t_expression {polynom_multiplication_with_n($3, $2); $$ = polynom_add_polynom($3, $1);
        #ifdef DEBUG
         printf("math s t_expr\n"); print_polynom($$); 
         #endif
         }
    | sign t_expression {polynom_multiplication_with_n($2, $1);$$ = init_polynom(); *$$ = *$2; 
        #ifdef DEBUG
        printf("sign t_expression: "); print_polynom($$);
        #endif
        }
    | t_expression {$$ = init_polynom(); *$$ = *$1;
        #ifdef DEBUG
        printf("t_expr "); print_polynom($$);
        #endif 
        }

t_expression:
    t_expression MUL f_expression {$$ = polynom_multiplication($1, $3);
        #ifdef DEBUG
        printf("multiplication mul tex fex: "); print_polynom($$);
        #endif
        }
    | t_expression f_expression {$$ = polynom_multiplication($1, $2); 
        #ifdef DEBUG
        printf("multiplication tex fex: "); print_polynom($$);
        #endif
        }
    | f_expression {$$ = init_polynom(); *$$ = *$1;
        #ifdef DEBUG
        printf("multiplication fex: "); print_polynom($$);
        #endif
        }

f_expression:
    p_expression POWER f_expression {$$ = polynom_to_power($1, $3); 
        #ifdef DEBUG
        printf("power: ");print_polynom($$);
        #endif
        }
    | p_expression {$$ = init_polynom(); *$$ = *$1; 
        #ifdef DEBUG
        printf("p_expr: "); print_polynom($$);
        #endif
        }

p_expression:
    NUMBER {$$ = init_polynom(); polynom_add_number($$, $1); 
        #ifdef DEBUG
        printf("number: "); print_polynom($$);
        #endif
        } 
    | VAR_POLY {$$ = init_polynom(); polynom_set_var($$, $1); $$->coef_arr[1] = 1; 
        #ifdef DEBUG
        printf("var_poly: "); print_polynom($$);
        #endif
        }
    | OP_BRACKET math_expression CL_BRACKET {$$ = init_polynom(); *$$ = *$2;
        #ifdef DEBUG
        printf("(math_expr): "); print_polynom($$);
        #endif
        }
    | VAR_NAME {$$ = init_polynom(); *$$ = *var_get_value(var_array, $1);
        #ifdef DEBUG
        printf("var_name :"); print_polynom($$);
        #endif
        }


sign: PLUS {$$=1;}
    | MINUS {$$=-1;} 


%%
int main(int argc, const char *argv[]){
    yyin = fopen(argv[1], "r");
    
    init_var_array(var_array);

    if (yyin == NULL)
    {
        printf("Error: file open\n");
        return 1;
    }
    yyparse();
    fclose(yyin);
}
