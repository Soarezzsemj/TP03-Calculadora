#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "calculadora.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TAM 512

float pilha_val[TAM];
int topo_val = -1;

char pilha_str[TAM][512];
int topo_str = -1;

int eh_numero(char *tok) {
    int i = 0;
    if (tok[0] == '-' && tok[1] != '\0') i = 1;
    int achou = 0, ponto = 0;
    for (; tok[i] != '\0'; i++) {
        if (isdigit(tok[i])) achou = 1;
        else if (tok[i] == '.') {
            ponto++;
            if (ponto > 1) return 0;
        }
        else return 0;
    }
    return achou;
}

int eh_func(char *tok) {
    if (strcmp(tok, "raiz") == 0) return 1;
    if (strcmp(tok, "sen") == 0)  return 1;
    if (strcmp(tok, "cos") == 0)  return 1;
    if (strcmp(tok, "tg") == 0)   return 1;
    if (strcmp(tok, "log") == 0)  return 1;
    return 0;
}

int eh_op(char *tok) {
    if (strcmp(tok, "+") == 0) return 1;
    if (strcmp(tok, "-") == 0) return 1;
    if (strcmp(tok, "*") == 0) return 1;
    if (strcmp(tok, "/") == 0) return 1;
    if (strcmp(tok, "%") == 0) return 1;
    if (strcmp(tok, "^") == 0) return 1;
    return 0;
}

int prec(char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    return 0;
}

int prec_raiz(char *expr) {
    int prof = 0, menor = 4, i = 0;
    while (expr[i] != '\0') {
        if (expr[i] == '(') prof++;
        else if (expr[i] == ')') prof--;
        else if (prof == 0) {
            if (expr[i] == '+' || expr[i] == '-') {
                if (1 < menor) menor = 1;
            } else if (expr[i] == '*' || expr[i] == '/' || expr[i] == '%') {
                if (2 < menor) menor = 2;
            } else if (expr[i] == '^') {
                if (3 < menor) menor = 3;
            }
        }
        i++;
    }
    return menor;
}

char *getInFixa(char *Str) {
    if (Str == NULL) return NULL;

    static char resultado[512];
    topo_str = -1;

    char buf[512];
    strcpy(buf, Str);

    char *tok = strtok(buf, " ");
    while (tok != NULL) {
        if (eh_numero(tok)) {
            float v = atof(tok);
            char tmp[64];
            if (v == (int)v && strchr(tok, '.') == NULL)
                snprintf(tmp, sizeof(tmp), "%d", (int)v);
            else
                snprintf(tmp, sizeof(tmp), "%g", v);
            if (topo_str >= TAM - 1) return NULL;
            strcpy(pilha_str[++topo_str], tmp);

        } else if (eh_func(tok)) {
            if (topo_str < 0) return NULL;
            char op[1024];
            strcpy(op, pilha_str[topo_str--]);
            char expr[1024];
            snprintf(expr, 1024, "%s(%s)", tok, op);
            strcpy(pilha_str[++topo_str], expr);

        } else if (eh_op(tok)) {
            if (topo_str < 1) return NULL;
            char b[1024], a[1024];
            strcpy(b, pilha_str[topo_str--]);
            strcpy(a, pilha_str[topo_str--]);

            int p  = prec(tok);
            int pa = prec_raiz(a);
            int pb = prec_raiz(b);

            int par_a = (pa < p);
            int par_b = (pb < p);

            if (strcmp(tok, "^") == 0 && pb <= p)
                par_b = 1;
            if ((strcmp(tok, "-") == 0 || strcmp(tok, "/") == 0) && pb <= p && pb < 4)
                par_b = 1;

            char expr[1024];
            if (par_a && par_b)
                snprintf(expr, 1024, "(%s)%s(%s)", a, tok, b);
            else if (par_a)
                snprintf(expr, 1024, "(%s)%s%s", a, tok, b);
            else if (par_b)
                snprintf(expr, 1024, "%s%s(%s)", a, tok, b);
            else
                snprintf(expr, 1024, "%s%s%s", a, tok, b);

            strcpy(pilha_str[++topo_str], expr);

        } else {
            return NULL;
        }

        tok = strtok(NULL, " ");
    }

    if (topo_str != 0) return NULL;

    strcpy(resultado, pilha_str[topo_str--]);
    return resultado;
}

float getValor(char *Str) {
    if (Str == NULL) return NAN;

    topo_val = -1;

    char buf[512];
    strcpy(buf, Str);

    char *tok = strtok(buf, " ");
    while (tok != NULL) {
        if (eh_numero(tok)) {
            if (topo_val >= TAM - 1) return NAN;
            pilha_val[++topo_val] = atof(tok);

        } else if (eh_func(tok)) {
            if (topo_val < 0) return NAN;
            float a = pilha_val[topo_val--];
            float res;

            if (strcmp(tok, "raiz") == 0) {
                if (a < 0) return NAN;
                res = sqrt(a);
            } else if (strcmp(tok, "sen") == 0) {
                res = sin(a * M_PI / 180.0);
            } else if (strcmp(tok, "cos") == 0) {
                res = cos(a * M_PI / 180.0);
            } else if (strcmp(tok, "tg") == 0) {
                res = tan(a * M_PI / 180.0);
            } else if (strcmp(tok, "log") == 0) {
                if (a <= 0) return NAN;
                res = log10(a);
            } else {
                return NAN;
            }

            pilha_val[++topo_val] = res;

        } else if (eh_op(tok)) {
            if (topo_val < 1) return NAN;
            float b = pilha_val[topo_val--];
            float a = pilha_val[topo_val--];
            float res;

            if (strcmp(tok, "+") == 0) res = a + b;
            else if (strcmp(tok, "-") == 0) res = a - b;
            else if (strcmp(tok, "*") == 0) res = a * b;
            else if (strcmp(tok, "/") == 0) {
                if (b == 0) return NAN;
                res = a / b;
            } else if (strcmp(tok, "%") == 0) {
                if (b == 0) return NAN;
                res = fmod(a, b);
            } else if (strcmp(tok, "^") == 0) {
                res = pow(a, b);
            } else {
                return NAN;
            }

            pilha_val[++topo_val] = res;

        } else {
            return NAN;
        }

        tok = strtok(NULL, " ");
    }

    if (topo_val != 0) return NAN;

    return pilha_val[topo_val--];
}