/* main.c: A simple program with use a regular expressions
 * 
 * Developed by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 * 
 * Date: 16/03/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <inttypes.h>

/* Number of arithmetic operator */
typedef enum OPERATORS{ PLUS, MINUS, MULT, DIV,
                        PER,  POWER, GREATER,
                        LESS, GREATER_EQUAL, LESS_EQUAL, EQUAL,
                        NOT_EQUAL, ATTRIBUTION, NO_OPERATOR } OPERATORS;

/* 
 *  */
void cpy_arg2expression(char *expression, const char **argv);

/* Get the numbers and the operator
 * of the expression passed with first argument */
void get_elements(const char *expression, double *num1, char *operator, double *num2);

/* 
 *  */
uint8_t verify_operator(const char *operator);

/* 
 *  */
double sum(double *num1, double *num2);

/* 
 *  */
double subtract(double *num1, double *num2);

/* 
 *  */
double multiplication(double *num1, double *num2);

/* 
 *  */
double division(double *num1, double *num2);

/* 
 *  */
bool is_greater(double *num1, double *num2);

/* 
 *  */
bool is_less(double *num1, double *num2);

/* 
 *  */
bool is_greater_equal(double *num1, double *num2);

/* 
 *  */
bool is_less_equal(double *num1, double *num2);

/* 
 *  */
bool is_equal(double *num1, double *num2);

/* 
 *  */
bool is_not_equal(double *num1, double *num2);

/* 
 *  */
double percent(double *num1, double *num2);

/* 
 *  */
double realize_calc(double *num1, char *op, double *num2);

/* 
 *  */
bool realize_logic(double *num1, char *op, double *num2);

/* 
 *  */
void free_ptr(void *ptr);

/* Arithmetic operators */
static const char *operators[13] = {
    "+", "-", "*", "/",
    "%", "**", ">", "<",
    ">=", "<=", "==", "!=",
    "="
};

int main(int argc, char **argv){
    if(argc == 1){
        char *expression = (char *) malloc(6 * sizeof(char));
        
        if(!expression){
            fprintf(stderr, "E: Out memory!\n");
            return ENOMEM;
        }        
        
        scanf("%6[^\n]", expression);
        
                
        double num1, num2;
        char *op = (char *) malloc(2 * sizeof(char));
        
        if(!op){
            fprintf(stderr, "E: Out memory!\n");
            return ENOMEM;
        }
        
        get_elements(expression, &num1, op, &num2);
        
        if(!strcmp(op, "q")){
            return 0;
        }
        
        if(verify_operator(op) < GREATER){
            double answer = realize_calc(&num1, op, &num2);
            
            printf("%.2lf\n", answer);
        }
        else{
            char *answer = (char *) malloc(6 * sizeof(char));
            
            if(!answer){
                fprintf(stderr, "E: Out memory!\n");
                return ENOMEM;
            }
            
            if(realize_logic(&num1, op, &num2) == true){
                strcpy(answer, "True");
            }
            else{
                strcpy(answer, "False"); 
            }
            
            puts(answer);
            
            free_ptr(answer);
        }
        free_ptr(op);
        free_ptr(expression);
    }/*
    else{
        fprintf(stderr, "E: Invalid arguments!\n");
        printf("Please, use %s -h for help.\n", argv[0]);
        return EXIT_FAILURE;
    }*/
    
    return EXIT_SUCCESS;
}

/*
void cpy_arg2exp(char *expression, const char **argv){
    
}
*/

void get_elements(const char *expression, double *num1,
                  char *operator, double *num2){
    *num1 = atof(&expression[0]);
    *num2 = atof(&expression[strlen(expression)-1]);
    
    for(int i = 1; expression[i] != '\0'; i++){
        if(expression[i] == '+'){
            strcpy(operator, operators[PLUS]);
            break;
        }
        else if(expression[i] == '-'){
            strcpy(operator, operators[MINUS]);
            break;
        }
        else if(expression[i] == '*' && expression[i+1] != '*'){
            strcpy(operator, operators[MULT]);
            break;
        }
        else if(expression[i] == '/'){
            strcpy(operator, operators[DIV]);
            break;
        }
        else if(expression[i] == '%'){
            strcpy(operator, operators[PER]);
            break;
        }
        else if((expression[i] == '*' && expression[i+1] == '*') || expression[i] == '^'){
            strcpy(operator, operators[POWER]);
            break;
        }
        else if(expression[i] == '>' && expression[i+1] != '='){
            strcpy(operator, operators[GREATER]);
            break;
        }
        else if(expression[i] == '<' && expression[i+1] != '='){
            strcpy(operator, operators[LESS]);
            break;
        }
        else if(expression[i] == '>' && expression[i+1] == '='){
            strcpy(operator, operators[GREATER_EQUAL]);
            break;
        }
        else if(expression[i] == '<' && expression[i+1] == '='){
            strcpy(operator, operators[LESS_EQUAL]);
            break;
        }
        else if(expression[i] == '=' && expression[i+1] == '='){
            strcpy(operator, operators[EQUAL]);
            break;
        }
        else if(expression[i] == '!' && expression[i+1] == '='){
            strcpy(operator, operators[NOT_EQUAL]);
            break;
        }
    }
}

uint8_t verify_operator(const char *operator){
    if(!strcmp(operator, operators[PLUS])){
        return PLUS;
    }
    else if(!strcmp(operator, operators[MINUS])){
        return MINUS;
    }
    else if(!strcmp(operator, operators[MULT])){
        return MULT;
    }
    else if(!strcmp(operator, operators[DIV])){
        return DIV;
    }
    else if(!strcmp(operator, operators[PER])){
        return PER;
    }
    else if(!strcmp(operator, operators[POWER]) || !strcmp(operator, "^")){
        return POWER;
    }
    else if(!strcmp(operator, operators[GREATER])){
        return GREATER;
    }
    else if(!strcmp(operator, operators[LESS])){
        return LESS;
    }
    else if(!strcmp(operator, operators[GREATER_EQUAL])){
        return GREATER_EQUAL;
    }
    else if(!strcmp(operator, operators[LESS_EQUAL])){
        return LESS_EQUAL;
    }
    else if(!strcmp(operator, operators[EQUAL])){
        return EQUAL;
    }
    else if(!strcmp(operator, operators[NOT_EQUAL])){
        return NOT_EQUAL;
    }
    else{
        return NO_OPERATOR;
    }
}

double sum(double *num1, double *num2){
    return *num1 + *num2;
}

double subtract(double *num1, double *num2){
    return *num1 - *num2;
}

double multiplication(double *num1, double *num2){
    return *num1 * *num2;
}

double division(double *num1, double *num2){
    return *num1 / *num2;
}

double percent(double *num1, double *num2){
    return *num1 * (*num2 / 100);
}

bool is_greater(double *num1, double *num2){
    return *num1 > *num2;
}

bool is_less(double *num1, double *num2){
    return *num1 < *num2;
}

bool is_greater_equal(double *num1, double *num2){
    return *num1 >= *num2;
}

bool is_less_equal(double *num1, double *num2){
    return *num1 <= *num2;
}

bool is_equal(double *num1, double *num2){
    return *num1 == *num2;
}

bool is_not_equal(double *num1, double *num2){
    return *num1 != *num2;
}

double realize_calc(double *num1, char *op, double *num2){
    if(verify_operator(op) == PLUS){
        return sum(num1, num2);
    }
    else if(verify_operator(op) == MINUS){
        return subtract(num1, num2);
    }
    else if(verify_operator(op) == MULT){
        return multiplication(num1, num2);
    }
    else if(verify_operator(op) == DIV){
        return division(num1, num2);
    }
    else if(verify_operator(op) == PER){
        return percent(num1, num2);
    }
    else if(verify_operator(op) == POWER){
        double n1 = *num1, n2 = *num2;
        return pow(n1, n2);
    }
    else{
        fprintf(stderr, "E: Operator invalid!\n");
        return NO_OPERATOR;
    }
}

bool realize_logic(double *num1, char *op, double *num2){
    if(verify_operator(op) == GREATER){
        return is_greater(num1, num2);
    }
    else if(verify_operator(op) == LESS){
        return is_less(num1, num2);
    }
    else if(verify_operator(op) == GREATER_EQUAL){
        return is_greater_equal(num1, num2);
    }
    else if(verify_operator(op) == LESS_EQUAL){
        return is_less_equal(num1, num2);
    }
    else if(verify_operator(op) == EQUAL){
        return is_equal(num1, num2);
    }
    else if(verify_operator(op) == NOT_EQUAL){
        return is_not_equal(num1, num2);
    }
}

void free_ptr(void *ptr){
    free(ptr);
    ptr = NULL;
}
