#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calculadora.h"

/* Tolerância para resultados aproximados */
#define TOLERANCE 0.001f

static void run_test(int num, const char *posfixa, const char *expected_infixa, float expected_valor) {
    /* Copia mutável para tokenizar */
    char buf[512];
    strncpy(buf, posfixa, 511);
    buf[511] = '\0';

    char *infixa = getInFixa(buf);

    /* Recopia pois strtok modifica o buffer */
    strncpy(buf, posfixa, 511);
    float valor = getValor(buf);

    /* Verifica resultado numérico */
    int valor_ok;
    if (isnan(expected_valor)) {
        valor_ok = isnan(valor);
    } else {
        valor_ok = (fabsf(valor - expected_valor) <= TOLERANCE);
    }

    printf("Teste %d:\n", num);
    printf("  Pos-fixa   : %s\n", posfixa);
    printf("  Infixa     : %s\n", infixa ? infixa : "(NULL - ERRO)");
    if (expected_infixa)
        printf("  Esp. Infixa: %s\n", expected_infixa);
    printf("  Valor      : %.4f", valor);
    if (!isnan(expected_valor))
        printf("  (esperado: %.4f)", expected_valor);
    printf("\n");
    printf("  Valor OK   : %s\n", valor_ok ? "SIM" : "NAO");
    printf("\n");
}

int main(void) {
    printf("=== Avaliador de Expressoes Numericas ===\n\n");

    /* Testes da tabela do enunciado */
    run_test(1, "3 4 + 5 *",    "(3+4)*5",           35.0f);
    run_test(2, "7 2 * 4 +",    "7*2+4",             18.0f);
    run_test(3, "8 5 2 4 + * +","8+(5*(2+4))",       38.0f);
    run_test(4, "6 2 / 3 + 4 *","(6/2+3)*4",         24.0f);
    run_test(5, "9 5 2 8 * 4 + * +","9+(5*(4+8*2))", 109.0f);
    run_test(6, "2 3 + log 5 /","log(2+3)/5",        0.13979f);
    run_test(7, "10 log 3 ^ 2 +","log(10)^3+2",      3.0f);
    run_test(8, "45 60 + 30 cos *","(45+60)*cos(30)", 90.9327f);
    run_test(9, "0.5 45 sen 2 ^ +","0.5+sen(45)^2",  1.0f);

    /* Testes extras criados pelo aluno */
    printf("=== Testes extras ===\n\n");

    /* Raiz quadrada de 16 = 4 */
    run_test(10, "16 raiz", "raiz(16)", 4.0f);

    /* log(100) = 2 */
    run_test(11, "100 log", "log(100)", 2.0f);

    /* sen(90) = 1 */
    run_test(12, "90 sen", "sen(90)", 1.0f);

    /* cos(0) = 1 */
    run_test(13, "0 cos", "cos(0)", 1.0f);

    /* tg(45) = 1 */
    run_test(14, "45 tg", "tg(45)", 1.0f);

    /* 2 ^ 10 = 1024 */
    run_test(15, "2 10 ^", "2^10", 1024.0f);

    /* 10 % 3 = 1 */
    run_test(16, "10 3 %", "10%3", 1.0f);

    /* (2 + 3) * (4 - 1) = 15: "2 3 + 4 1 - *" */
    run_test(17, "2 3 + 4 1 - *", "(2+3)*(4-1)", 15.0f);

    /* Erro: divisão por zero */
    printf("Teste 18 (divisao por zero - deve retornar NAN):\n");
    {
        char buf[64] = "5 0 /";
        float v = getValor(buf);
        printf("  Valor: %s\n\n", isnan(v) ? "NAN (correto)" : "ERRO - nao detectou divisao por zero");
    }

    /* Erro: log de zero */
    printf("Teste 19 (log de zero - deve retornar NAN):\n");
    {
        char buf[64] = "0 log";
        float v = getValor(buf);
        printf("  Valor: %s\n\n", isnan(v) ? "NAN (correto)" : "ERRO - nao detectou log de zero");
    }

    /* Erro: raiz de negativo */
    printf("Teste 20 (raiz de negativo - deve retornar NAN):\n");
    {
        char buf[64] = "-4 raiz";
        float v = getValor(buf);
        printf("  Valor: %s\n\n", isnan(v) ? "NAN (correto)" : "ERRO - nao detectou raiz de negativo");
    }

    /* Erro: operandos insuficientes */
    printf("Teste 21 (operandos insuficientes - deve retornar NULL):\n");
    {
        char buf[64] = "3 +";
        char *inf = getInFixa(buf);
        printf("  getInFixa: %s\n\n", inf == NULL ? "NULL (correto)" : "ERRO - deveria ser NULL");
    }

    /* Erro: token desconhecido */
    printf("Teste 22 (token desconhecido - deve retornar NULL):\n");
    {
        char buf[64] = "3 4 xyz";
        char *inf = getInFixa(buf);
        printf("  getInFixa: %s\n\n", inf == NULL ? "NULL (correto)" : "ERRO - deveria ser NULL");
    }

    return 0;
}