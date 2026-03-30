#ifndef MISSAO_H
#define MISSAO_H

#include "territorio.h"

#define TAM_MISSAO 120

/*
 * Sorteia uma missao do vetor e copia para 'destino' com strcpy.
 * O destino deve apontar para memoria valida.
 */
void atribuir_missao(char *destino, char *missoes[], int totalMissoes);

/* Exibe a missao do jogador (passagem por valor do ponteiro). */
void exibir_missao(char missao[]);

/*
 * Verifica se a missao recebida foi cumprida no estado atual do mapa.
 * Retorna 1 quando cumprida e 0 caso contrario.
 */
int verificar_missao(char *missao, const char *corJogador, Territorio *mapa, int tamanho);

/*
 * Retorna 1 quando a missao pode ser atribuida para o continente do jogador.
 * Exemplo: jogador de Europa nao pode receber "Conquistar o continente Europa".
 */
int missao_compativel_com_jogador(const char *missao, const char *continenteJogador);

#endif /* MISSAO_H */
