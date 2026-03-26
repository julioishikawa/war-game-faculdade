#ifndef ENTRADA_H
#define ENTRADA_H

/*
 * entrada.h
 * Declara utilitarios de leitura e validação da entrada do usuario.
 */

/* Descarta caracteres restantes no buffer ate o fim da linha. */
void entrada_limpar_buffer(void);

/* Le e válida um inteiro no intervalo [min, max].
 * Repete a leitura ate o usuario digitar um valor válido. */
int entrada_ler_inteiro(const char *mensagem, int min, int max);

#endif /* ENTRADA_H */
