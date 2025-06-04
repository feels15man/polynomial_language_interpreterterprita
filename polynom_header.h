#ifndef _POLYNOM_HEADER
#define _POLYNOM_HEADER

#define MAX_VAR_NAME_LEN 15
#define MAX_POLY_POWER 50
#define MAX_VAR_COUNT 20

typedef struct monom_struct{
    char var;
    int power;
    int coefficient;
} MONOMIAL_STRUCT;

typedef struct polynom_struct{
    char var;
    long long coef_arr[MAX_POLY_POWER];
    // MONOMIAL_STRUCT monom_arr[50];
} POLYNOMIAL_STRUCT;

typedef struct variable{
    char *name;
    POLYNOMIAL_STRUCT* value;
} VARIABLE;

#endif