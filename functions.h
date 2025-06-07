#ifndef _FUNCTIONS_HEADER
#define _FUNCTIONS_HEADER

#include "polynom_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void yyerror(const char *);

void print_polynom(POLYNOMIAL_STRUCT *);
void monom_addition(MONOMIAL_STRUCT *, MONOMIAL_STRUCT *);
int var_add_new_value(VARIABLE [], char *, POLYNOMIAL_STRUCT* );

// Polynoms
void print_monom(MONOMIAL_STRUCT m){
    printf("Monom:\n" );
    printf("|\tcoef = %d\n", m.coefficient);
    printf("|\tvariable = %c\n", m.var);
    printf("|\tpower = %d\n", m.power);
}

void print_number(POLYNOMIAL_STRUCT *p){
    printf(p->coef_arr[0] > 0 ? "+ %lld ": "%lld ", p->coef_arr[0]);
}

int print_0_pow_or_no(POLYNOMIAL_STRUCT *p){
    for (int i = MAX_POLY_POWER - 1; i > 0; i--){
        if (p->coef_arr[i])
            return 0;
    }
    return 1;
}

void print_polynom(POLYNOMIAL_STRUCT *p){
    // if (!p->var){
    //     printf("(Polynom has no var)\n");
    // }
    int f = 0;
    for(int i = MAX_POLY_POWER - 1; i >= 0 ; i--){
        if (!i && !p->coef_arr[i] && !f)
            printf("0");
        else if (!p->coef_arr[i])
            continue;

        //sign
        if (p->coef_arr[i] < 0)
            printf("-");
        else if (f)
            printf("+");

        //coef
        if (p->coef_arr[i] != 1 && p->coef_arr[i] != -1)
            printf("%llu", (unsigned long long) p->coef_arr[i]);
        else if(!i)
            printf("1");

        //var
        if (i > 0)
            printf("%c", p->var);

        //power
        if (i > 1)
            printf("^%d", i);
        
        f = 1;
    }
    printf("\n");
}


POLYNOMIAL_STRUCT *init_polynom(void){
    POLYNOMIAL_STRUCT *res = (POLYNOMIAL_STRUCT *) malloc(sizeof(POLYNOMIAL_STRUCT));
    for (int i = 0; i < MAX_POLY_POWER; i++) 
    {
        res->coef_arr[i] = 0;
    }
    res->var = 0;
    return res;
}

void polynom_set_var(POLYNOMIAL_STRUCT* p, char c){
   p->var = c; 
}

void polynom_add_number(POLYNOMIAL_STRUCT* p, long long n){
    p->coef_arr[0] += n;
}

POLYNOMIAL_STRUCT* polynom_add_polynom(POLYNOMIAL_STRUCT *p1, POLYNOMIAL_STRUCT* p2){
    if (p1->var && p2->var && p2->var != p1->var)
        yyerror("Polynoms have different vars in addition");
    POLYNOMIAL_STRUCT *res = init_polynom();
    polynom_set_var(res, p1->var? p1->var:p2->var);
    for (int i = 0; i < MAX_POLY_POWER; i++){
        res->coef_arr[i] += p1->coef_arr[i] + p2->coef_arr[i];
    }
    return res;
}

void polynom_multiplication_with_n(POLYNOMIAL_STRUCT *p, int n){
    for (int i = 0; i < MAX_POLY_POWER; i++)
        p->coef_arr[i] *= n;
    // printf("polynom mul with n :");
    // print_polynom(p);
}

POLYNOMIAL_STRUCT *polynom_multiplication(POLYNOMIAL_STRUCT *p, POLYNOMIAL_STRUCT* m){
    if (p->var && m->var && p->var != m->var)
        yyerror("Polynoms have different vars");

    POLYNOMIAL_STRUCT *tmp = init_polynom();
    polynom_set_var(tmp, p->var? p->var:m->var);
    for (int i = 0; i < MAX_POLY_POWER; i++){
        for (int j = 0; j < MAX_POLY_POWER; j++){
            if (p->coef_arr[i] && m->coef_arr[j] && i + j >= MAX_POLY_POWER)
                yyerror("The power of expression is greater than MAX_POLY_POWER");
            if ( i + j >= MAX_POLY_POWER)
                continue;
            tmp->coef_arr[i + j] += p->coef_arr[i] * m->coef_arr[j];
        }
    }
    // if (p != m){
    //     free(p);
    //     free(m);
    // }
    // else
    //     free(p);
    // print_polynom(tmp);
    return tmp;
}

int polynom_is_zero(POLYNOMIAL_STRUCT* p){
    for(int i = 0; i < MAX_POLY_POWER; i++){
        if (p->coef_arr[i])
            return 0;
    }
    return 1;
}

POLYNOMIAL_STRUCT* polynom_to_power(POLYNOMIAL_STRUCT *p, POLYNOMIAL_STRUCT* power){
    for(int i = 1; i < MAX_POLY_POWER; i++){
        if (power->coef_arr[i])
            yyerror("Attempt to polynom in power of polynom");
    }
    int pow_coef = (int) power->coef_arr[0];

    if (polynom_is_zero(p) && pow_coef == 0)
        yyerror("What... ? You try to 0^0... What r u waiting for?");

    if (pow_coef < 0)
        yyerror("Negativer power");

    POLYNOMIAL_STRUCT* res = init_polynom();
    if (power->coef_arr[0] == 0){
        res->coef_arr[0] = 1; 
        return res;
    }

    *res = *p;
    for (int i = 1; i < pow_coef; i++)
    {
        res = polynom_multiplication(p, res);
        // print_polynom(res);
    }
    return res;
}


// Variables

void init_var_array(VARIABLE arr[]){
    for (int i = 0; i < MAX_VAR_COUNT; i++){
        arr[i].name = NULL;
        arr[i].value = NULL;
    } 
    // printf("init var arr successfuly\n");
}

POLYNOMIAL_STRUCT* var_get_value(VARIABLE arr[], char *s){
    // printf("var get value of '%s'\n", s);
    for (int i = 0; i < MAX_VAR_COUNT; i++){
        // if (arr[i].name)
        //     printf("%d: name = '%s'\n", i, arr[i].name);
        if (arr[i].name && !strcmp(arr[i].name, s))
            return arr[i].value;
    }
    yyerror("Undefined variable");
    return NULL;
}

int var_set_value(VARIABLE arr[], char *s, POLYNOMIAL_STRUCT* v){
    // printf("var set val  '%s' = ", s);
    // print_polynom(v);
    int i = 0;
    for (i = 0; i < MAX_VAR_COUNT; i++){
        if (arr[i].name && !strcmp(arr[i].name, s))
            break;
        // printf("%d\n", i);
    }
    if (i == MAX_VAR_COUNT)
        var_add_new_value(arr, s, v);
    else
        arr[i].value = v;
    return 0;
}

int var_add_new_value(VARIABLE arr[], char *s, POLYNOMIAL_STRUCT* v){
    // printf("var add new val  '%s' = ", s);
    int i = 0;
    while (i < MAX_VAR_COUNT && arr[i].name ) i++;
    if (i == MAX_VAR_COUNT)
        yyerror("No more available slots for variable");

    arr[i].name = (char*) malloc(sizeof(char) * MAX_VAR_NAME_LEN);
    strcpy(arr[i].name, s);

    arr[i].value = v;
    // printf("var_add_new_value added successfuly %d: name = '%s'\n", i, arr[i].name);
    // print_polynom(arr[i].value);
    return 0;
}


#endif
