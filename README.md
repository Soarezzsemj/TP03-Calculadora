# Avaliador de Expressões Numéricas

Trabalho prático da disciplina **Estrutura de Dados** — UCB, 3º semestre.

O programa avalia expressões matemáticas escritas em notação pós-fixa, convertendo-as para a notação infixa e calculando o resultado.

---

## Estrutura do projeto

```
.
├── calculadora.h     # cabeçalho fornecido pelo professor (não alterado)
├── 25201228.c        # implementação das funções getInFixa e getValor
└── main.c            # arquivo de testes fornecido pelo professor
```

---

## Como compilar e executar

```bash
gcc 25201228.c main.c -o calculadora.exe
./calculadora.exe
```

> No Windows basta rodar `calculadora.exe` após compilar.

---

## Funcionalidades

- Conversão de notação pós-fixa para infixa
- Operações aritméticas: `+`, `-`, `*`, `/`, `%`, `^`
- Funções matemáticas: `raiz`, `sen`, `cos`, `tg`, `log`
    - Ângulos em graus
    - `log` é logaritmo decimal (base 10)
- Detecção de expressões inválidas:
    - Divisão ou módulo por zero
    - Logaritmo de número ≤ 0
    - Raiz de número negativo
    - Operandos insuficientes
    - Token desconhecido

---

## Exemplos

| Pós-fixa | Infixa | Valor |
|---|---|---|
| `3 4 + 5 *` | `(3+4)*5` | 35 |
| `7 2 * 4 +` | `7*2+4` | 18 |
| `2 3 + log 5 /` | `log(2+3)/5` | ≈ 0.14 |
| `45 60 + 30 cos *` | `(45+60)*cos(30)` | ≈ 90.93 |
| `0.5 45 sen 2 ^ +` | `0.5+sen(45)^2` | 1 |