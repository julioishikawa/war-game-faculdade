/*
 * entrada.c
 * Implementa utilitarios de leitura e validação da entrada do usuario.
 */

#include <stdio.h>
#include "entrada.h"

void entrada_limpar_buffer(void) {
    int c;
    /* Remove caracteres extras deixados pelo scanf ate o fim da linha. */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Descarta ate o fim da linha. */
    }
}

int entrada_ler_inteiro(const char *mensagem, int min, int max) {
    int valor;

    /* Loop de validação: so retorna quando o valor estiver no intervalo permitido. */
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1) {
            entrada_limpar_buffer();
            if (valor >= min && valor <= max) {
                /* Entrada válida: devolve para o chamador. */
                return valor;
            }
        } else {
            /* Usuario digitou algo não numerico. */
            entrada_limpar_buffer();
        }
        printf("\nEntrada invalida. Digite um valor entre %d e %d.\n", min, max);
    }
}
