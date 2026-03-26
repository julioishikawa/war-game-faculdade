#ifndef TERRITORIO_H
#define TERRITORIO_H

/*
 * territorio.h
 * Define a estrutura Territorio e declara as funções de
 * cadastro e exibição do mapa.
 */

typedef struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Aloca e retorna um novo mapa com 'quantidade' territórios.
 * Retorna NULL em caso de falha de alocação. */
Territorio *territorio_alocar(int quantidade);

/* Libera a memoria do mapa previamente alocado. */
void liberar_memoria(Territorio *mapa);

/* Alias para manter compatibilidade com codigo legado do projeto. */
void territorio_liberar(Territorio *mapa);

/* Preenche os dados de um único território via entrada do usuario. */
void territorio_ler(Territorio *t, int indice, int total);

/* Preenche todos os territórios do mapa via entrada do usuario. */
void territorio_ler_todos(Territorio *mapa, int quantidade);

/* Exibe o estado atual de todos os territórios do mapa. */
void territorio_exibir_todos(const Territorio *mapa, int quantidade);

#endif /* TERRITORIO_H */
