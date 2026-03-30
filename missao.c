#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "missao.h"

/*
 * Busca um território no mapa pelo nome.
 * Retorna um ponteiro para o território correspondente ou NULL se não encontrado.
 */
static Territorio *buscar_territorio_por_nome(Territorio *mapa, int tamanho, const char *nome) {
    for (int i = 0; i < tamanho; i++) {
        if (strcmp((mapa + i)->nome, nome) == 0) {
            return mapa + i;
        }
    }
    return NULL;
}

/*
 * Sorteia uma missão do vetor de missões e copia para o destino.
 * O destino deve apontar para memória válida com espaço suficiente.
 */
void atribuir_missao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

/*
 * Exibe a missão do jogador no console.
 */
void exibir_missao(char missao[]) {
    printf("Missao estrategica: %s\n", missao);
}

/*
 * Verifica se a missão recebida foi cumprida no estado atual do mapa.
 * Retorna 1 quando cumprida e 0 caso contrário.
 *
 * O formato suportado é "Conquistar o continente X".
 */
int verificar_missao(char *missao, const char *corJogador, Territorio *mapa, int tamanho) {
    const char *prefixo = "Conquistar o continente ";
    size_t tamanho_prefixo = strlen(prefixo);

    /* Verifica se a missão começa com o prefixo esperado. */
    if (strncmp(missao, prefixo, tamanho_prefixo) == 0) {
        const char *continente_alvo = missao + tamanho_prefixo;

        /* Busca o território correspondente ao continente alvo. */
        Territorio *territorio_alvo = buscar_territorio_por_nome(mapa, tamanho, continente_alvo);

        /* Retorna 1 somente se o território alvo existir e pertencer ao jogador. */
        if (territorio_alvo != NULL && strcmp(territorio_alvo->cor, corJogador) == 0) {
            return 1;
        }
    }
    return 0;
}

/*
 * Retorna 1 quando a missão pode ser atribuída ao jogador.
 * Retorna 0 quando a missão é incompatível com o continente do jogador.
 */
int missao_compativel_com_jogador(const char *missao, const char *continenteJogador) {
    char missao_proibida[TAM_MISSAO];
    snprintf(missao_proibida, sizeof(missao_proibida),
             "Conquistar o continente %s", continenteJogador);

    if (strcmp(missao, missao_proibida) == 0) {
        return 0;
    }
    return 1;
}
