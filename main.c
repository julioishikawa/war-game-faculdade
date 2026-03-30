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
#include "missao.h"

typedef struct ContinenteBase {
    const char *nome;
    const char *cor;
} ContinenteBase;

static void inicializar_mapa_continentes(Territorio *mapa) {
    const ContinenteBase continentes[] = {
        {"America", "verde"},
        {"Europa", "azul"},
        {"Asia", "vermelha"},
        {"Africa", "amarela"},
        {"Oceania", "branca"}
    };

    for (int i = 0; i < 5; i++) {
        strcpy((mapa + i)->nome, continentes[i].nome);
        strcpy((mapa + i)->cor, continentes[i].cor);
        (mapa + i)->tropas = 10;
    }
}

static void exibir_continentes_disponiveis(const Territorio *mapa, const int *escolhido, int tamanho) {
    printf("\n=========== CONTINENTES DISPONIVEIS ===========\n");
    for (int i = 0; i < tamanho; i++) {
        if (!escolhido[i]) {
            printf("%d) %-10s | Cor: %-8s | Tropas iniciais: %d\n",
                   i + 1, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
        }
    }
    printf("===============================================\n");
}

static int existe_atacante_da_cor(const Territorio *mapa, int quantidade, const char *cor) {
    for (int i = 0; i < quantidade; i++) {
        if ((mapa + i)->tropas >= 2 && strcmp((mapa + i)->cor, cor) == 0) {
            return 1;
        }
    }
    return 0;
}

static void exibir_atacantes_validos(const Territorio *mapa, int quantidade, const char *cor) {
    /* Mostra somente territórios da cor do jogador aptos a atacar. */
    printf("\nTerritorios de %s que podem atacar (minimo 2 tropas):\n", cor);
    for (int i = 0; i < quantidade; i++) {
        if ((mapa + i)->tropas >= 2 && strcmp((mapa + i)->cor, cor) == 0) {
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

static void liberar_memoria_jogo(Territorio *mapa, char **missoes, char (*nomes)[30], char (*cores)[10]) {
    if (missoes != NULL) {
        int i = 0;
        while (missoes[i] != NULL) {
            free(missoes[i]);
            i++;
        }
        free(missoes);
    }
    free(nomes);
    free(cores);
    liberar_memoria(mapa);
}

int main(void) {
    /* Inicializa o gerador pseudoaleatorio para os dados de batalha. */
    srand((unsigned int)time(NULL));

    printf("===========================================\n");
    printf("WAR ESTRUTURADO - ATAQUES ENTRE TERRITORIOS\n");
    printf("===========================================\n");

    /* O mapa inicial e fixo: 5 continentes com 10 tropas cada. */
    const int quantidade = 5;

    Territorio *mapa = territorio_alocar(quantidade);
    if (mapa == NULL) {
        return 1;
    }
    inicializar_mapa_continentes(mapa);

    int total_jogadores = entrada_ler_inteiro(
        "Quantidade de jogadores (min 2, max 5): ", 2, 5);

    char (*nomes_jogadores)[30] =
        (char (*)[30])calloc((size_t)total_jogadores, sizeof(*nomes_jogadores));
    char (*cores_jogadores)[10] =
        (char (*)[10])calloc((size_t)total_jogadores, sizeof(*cores_jogadores));
    char **missoes_jogadores =
        (char **)calloc((size_t)total_jogadores + 1, sizeof(char *));

    if (nomes_jogadores == NULL || cores_jogadores == NULL || missoes_jogadores == NULL) {
        printf("\nErro: nao foi possivel alocar memoria para jogadores/missoes.\n");
        free(nomes_jogadores);
        free(cores_jogadores);
        free(missoes_jogadores);
        liberar_memoria(mapa);
        return 1;
    }

    for (int i = 0; i < total_jogadores; i++) {
        missoes_jogadores[i] = (char *)malloc(TAM_MISSAO * sizeof(char));
        if (missoes_jogadores[i] == NULL) {
            printf("\nErro: nao foi possivel alocar memoria para missao.\n");
            liberar_memoria_jogo(mapa, missoes_jogadores, nomes_jogadores, cores_jogadores);
            return 1;
        }
    }

    int *continente_escolhido = (int *)calloc((size_t)quantidade, sizeof(int));
    if (continente_escolhido == NULL) {
        printf("\nErro: nao foi possivel alocar memoria para escolhas.\n");
        liberar_memoria_jogo(mapa, missoes_jogadores, nomes_jogadores, cores_jogadores);
        return 1;
    }

    printf("\nCada jogador deve escolher um continente unico.\n");
    for (int i = 0; i < total_jogadores; i++) {
        int escolha = -1;
        while (1) {
            exibir_continentes_disponiveis(mapa, continente_escolhido, quantidade);
            escolha = entrada_ler_inteiro(
                "\nEscolha o numero do continente do jogador: ", 1, quantidade) - 1;
            if (!continente_escolhido[escolha]) {
                break;
            }
            printf("\nEsse continente ja foi escolhido. Selecione outro.\n");
        }

        continente_escolhido[escolha] = 1;
        strcpy(nomes_jogadores[i], (mapa + escolha)->nome);
        strcpy(cores_jogadores[i], (mapa + escolha)->cor);
        printf("Jogador %d ficou com %s (cor %s).\n",
               i + 1, nomes_jogadores[i], cores_jogadores[i]);
    }
    free(continente_escolhido);

    char *missoes_base[] = {
        "Conquistar o continente America",
        "Conquistar o continente Europa",
        "Conquistar o continente Asia",
        "Conquistar o continente Africa",
        "Conquistar o continente Oceania"
    };
    int total_missoes = (int)(sizeof(missoes_base) / sizeof(missoes_base[0]));

    printf("\nMissoes sorteadas:\n");
    for (int i = 0; i < total_jogadores; i++) {
        do {
            atribuir_missao(missoes_jogadores[i], missoes_base, total_missoes);
        } while (!missao_compativel_com_jogador(missoes_jogadores[i], nomes_jogadores[i]));
        printf("\n%s (%s): ", nomes_jogadores[i], cores_jogadores[i]);
        exibir_missao(missoes_jogadores[i]);
    }

    printf("\nMapa inicial padrao carregado com 10 tropas por continente.\n");
    territorio_exibir_todos(mapa, quantidade);

    /* Loop principal: cada iteração representa uma tentativa de ataque. */
    int continuar = 1;
    int turno = 0;
    while (continuar == 1) {
        int jogador_atual = turno % total_jogadores;
        printf("\n--- NOVO ATAQUE ---\n");
        printf("Turno de %s (cor %s)\n",
               nomes_jogadores[jogador_atual], cores_jogadores[jogador_atual]);
        territorio_exibir_todos(mapa, quantidade);

        if (!existe_atacante_da_cor(mapa, quantidade, cores_jogadores[jogador_atual])) {
            printf("\nSem ataques validos para a cor %s neste turno.\n",
                   cores_jogadores[jogador_atual]);
            int menu_opcao = 0;
            do {
                printf("\nMenu de turno:\n");
                printf("1) Ir para o proximo turno\n");
                printf("2) Ver missoes dos jogadores\n");
                printf("0) Encerrar jogo\n");
                menu_opcao = entrada_ler_inteiro("\nEscolha uma opcao: ", 0, 2);
                if (menu_opcao == 2) {
                    printf("\n--- Missoes dos jogadores ---\n");
                    for (int i = 0; i < total_jogadores; i++) {
                        printf("%s (%s): ", nomes_jogadores[i], cores_jogadores[i]);
                        exibir_missao(missoes_jogadores[i]);
                    }
                }
            } while (menu_opcao == 2);
            if (menu_opcao == 1) {
                turno++;
                continue;
            } else {
                continuar = 0;
                break;
            }
        }

        exibir_atacantes_validos(mapa, quantidade, cores_jogadores[jogador_atual]);

        /* Escolha dos indices (usuario digita de 1..N e convertemos para 0..N-1). */
        int idx_atacante = entrada_ler_inteiro(
            "\nEscolha o territorio atacante (numero): ", 1, quantidade) - 1;

        if (strcmp((mapa + idx_atacante)->cor, cores_jogadores[jogador_atual]) != 0) {
            printf("\nAtaque cancelado: o territorio atacante nao pertence ao jogador da vez.\n");
            int menu_opcao = 0;
            do {
                printf("\nMenu de turno:\n");
                printf("1) Ir para o proximo turno\n");
                printf("2) Ver missoes dos jogadores\n");
                printf("0) Encerrar jogo\n");
                menu_opcao = entrada_ler_inteiro("\nEscolha uma opcao: ", 0, 2);
                if (menu_opcao == 2) {
                    printf("\n--- Missoes dos jogadores ---\n");
                    for (int i = 0; i < total_jogadores; i++) {
                        printf("%s (%s): ", nomes_jogadores[i], cores_jogadores[i]);
                        exibir_missao(missoes_jogadores[i]);
                    }
                }
            } while (menu_opcao == 2);
            if (menu_opcao == 1) {
                turno++;
                continue;
            } else {
                continuar = 0;
                break;
            }
        }

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

        for (int i = 0; i < total_jogadores; i++) {
            if (verificar_missao(missoes_jogadores[i], cores_jogadores[i], mapa, quantidade)) {
                printf("\nVITORIA! %s cumpriu sua missao e venceu o jogo!\n",
                       nomes_jogadores[i]);
                liberar_memoria_jogo(mapa, missoes_jogadores, nomes_jogadores, cores_jogadores);
                printf("\nMemoria liberada. Fim do programa.\n");
                return 0;
            }
        }
        int menu_opcao = 0;
        do {
            printf("\nMenu de turno:\n");
            printf("1) Ir para o proximo turno\n");
            printf("2) Ver missoes dos jogadores\n");
            printf("0) Encerrar jogo\n");
            menu_opcao = entrada_ler_inteiro("\nEscolha uma opcao: ", 0, 2);
            if (menu_opcao == 2) {
                printf("\n--- Missoes dos jogadores ---\n");
                for (int i = 0; i < total_jogadores; i++) {
                    printf("%s (%s): ", nomes_jogadores[i], cores_jogadores[i]);
                    exibir_missao(missoes_jogadores[i]);
                }
            }
        } while (menu_opcao == 2);
        if (menu_opcao == 1) {
            turno++;
        } else {
            continuar = 0;
        }
    }

    /* Encerramento sem vencedor por escolha do usuario. */
    liberar_memoria_jogo(mapa, missoes_jogadores, nomes_jogadores, cores_jogadores);
    printf("\nJogo encerrado pelo usuario. Memoria liberada.\n");
    return 0;
}