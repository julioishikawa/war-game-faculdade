/*
 * territorio.c
 * Implementa as funções de alocação, cadastro e exibição de territórios.
 */

#include <stdio.h>
#include <stdlib.h>
#include "territorio.h"
#include "entrada.h"

Territorio *territorio_alocar(int quantidade) {
    /* calloc inicializa todos os campos com zero e evita lixo de memoria. */
    Territorio *mapa = (Territorio *)calloc((size_t)quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("\nErro: nao foi possivel alocar memoria para o mapa.\n");
    }
    return mapa;
}

void liberar_memoria(Territorio *mapa) {
    /* Libera o bloco dinamico que representa o mapa de territórios. */
    free(mapa);
}

void territorio_liberar(Territorio *mapa) {
    /* Alias para compatibilidade com versões anteriores. */
    liberar_memoria(mapa);
}

void territorio_ler(Territorio *t, int indice, int total) {
    printf("\n--- Territorio %d de %d ---\n", indice + 1, total);

    printf("Nome do territorio (uma palavra): ");
    if (scanf(" %29s", t->nome) != 1) {
        /* Em caso de falha de leitura, deixa string vazia. */
        t->nome[0] = '\0';
    }

    printf("Cor do exercito (uma palavra): ");
    if (scanf(" %9s", t->cor) != 1) {
        /* Em caso de falha de leitura, deixa string vazia. */
        t->cor[0] = '\0';
    }

    printf("Numero de tropas (minimo 1): ");
    if (scanf("%d", &t->tropas) != 1 || t->tropas < 1) {
        /* Garante valor mínimo válido para manter a simulação consistente. */
        t->tropas = 1;
    }
    entrada_limpar_buffer();
}

void territorio_ler_todos(Territorio *mapa, int quantidade) {
    /* Percorre o vetor dinâmico de territórios e cadastra um por vez. */
    for (int i = 0; i < quantidade; i++) {
        territorio_ler(mapa + i, i, quantidade);
    }
}

void territorio_exibir_todos(const Territorio *mapa, int quantidade) {
    printf("\n===========================================\n");
    printf("MAPA ATUAL\n");
    printf("===========================================\n");

    for (int i = 0; i < quantidade; i++) {
        /* Acesso por ponteiro para exibir cada posicao do vetor. */
        const Territorio *t = mapa + i;
        printf("%d) %-12s | Cor: %-9s | Tropas: %d\n",
               i + 1, t->nome, t->cor, t->tropas);
    }
}
