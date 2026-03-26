/*
 * main.c
 * Ponto de entrada do programa WAR Estruturado.
 * Orquestra o fluxo principal usando os módulos:
 *   - entrada   : leitura e validação do usuario
 *   - território: alocação, cadastro e exibicao do mapa
 *   - batalha   : validação e resolução de ataques
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "entrada.h"
#include "territorio.h"
#include "batalha.h"

static void exibir_atacantes_validos(const Territorio *mapa, int quantidade) {
    /* Mostra somente territórios aptos a atacar (2 tropas ou mais). */
    printf("\nTerritorios que podem atacar (minimo 2 tropas):\n");
    for (int i = 0; i < quantidade; i++) {
        if ((mapa + i)->tropas >= 2) {
            printf("%d) %s | Cor: %s | Tropas: %d\n",
                   i + 1, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
        }
    }
}

static void exibir_defensores_validos(const Territorio *mapa, int quantidade, int idx_atacante) {
    /* Exibe alvos possíveis com cor diferente da cor atacante. */
    const Territorio *atacante = mapa + idx_atacante;
    printf("\nTerritorios que podem ser defendidos (cor diferente de %s):\n", atacante->cor);
    for (int i = 0; i < quantidade; i++) {
        const Territorio *defensor = mapa + i;
        if (i != idx_atacante && strcmp(defensor->cor, atacante->cor) != 0) {
            printf("%d) %s | Cor: %s | Tropas: %d\n",
                   i + 1, defensor->nome, defensor->cor, defensor->tropas);
        }
    }
}

int main(void) {
    /* Inicializa o gerador pseudoaleatorio para os dados de batalha. */
    srand((unsigned int)time(NULL));

    printf("===========================================\n");
    printf("WAR ESTRUTURADO - ATAQUES ENTRE TERRITORIOS\n");
    printf("===========================================\n");

    /* Le a quantidade de territórios e cria o mapa dinamicamente. */
    int quantidade = entrada_ler_inteiro(
        "Quantidade de territorios (min 2, max 10): ", 2, 10);

    Territorio *mapa = territorio_alocar(quantidade);
    if (mapa == NULL) {
        return 1;
    }

    territorio_ler_todos(mapa, quantidade);
    territorio_exibir_todos(mapa, quantidade);

    /* Loop principal: cada iteração representa uma tentativa de ataque. */
    int continuar = 1;
    while (continuar == 1) {
        printf("\n--- NOVO ATAQUE ---\n");
        territorio_exibir_todos(mapa, quantidade);
        exibir_atacantes_validos(mapa, quantidade);

        /* Escolha dos indices (usuario digita de 1..N e convertemos para 0..N-1). */
        int idx_atacante = entrada_ler_inteiro(
            "\nEscolha o territorio atacante (numero): ", 1, quantidade) - 1;

        exibir_defensores_validos(mapa, quantidade, idx_atacante);
        int idx_defensor = entrada_ler_inteiro(
            "\nEscolha o territorio defensor (numero): ", 1, quantidade) - 1;

        /* Válida regra de ataque antes de executar a batalha. */
        if (batalha_ataque_valido(mapa, idx_atacante, idx_defensor)) {
            atacar(mapa + idx_atacante, mapa + idx_defensor);
            printf("\nEstado do mapa apos o ataque:\n");
            territorio_exibir_todos(mapa, quantidade);
        } else {
            printf("\nAtaque cancelado. Tente novamente com uma combinacao valida.\n");
        }

        continuar = entrada_ler_inteiro(
            "\nDeseja realizar outro ataque? (1 = sim, 0 = nao): ", 0, 1);
    }

    /* Libera toda a memoria dinâmica antes de encerrar o programa. */
    liberar_memoria(mapa);
    printf("\nMemoria liberada. Fim do programa.\n");
    return 0;
}
