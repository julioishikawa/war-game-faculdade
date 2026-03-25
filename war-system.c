#include <stdio.h>
#include <string.h>

/* Maximo de territorios que o banco de dados guarda nesta missao. */
#define NUM_TERRITORIOS 5

/*
 * Territorio - tipo composto que agrupa todos os dados por regiao.
 * - nome: identificador do territorio (ate 29 caracteres + '\0').
 * - cor: rotulo da cor do exercito / faccao (ate 9 caracteres + '\0').
 * - tropas: quantidade de forcas estacionadas no local.
 */
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

/*
 * Le um territorio da entrada padrao com scanf.
 * Usei limites de largura (%29s, %9s) para que o scanf nunca escreva alem dos vetores de caracteres.
 * O espaco inicial nos formatos ignora quebras de linha sobrando de leituras numericas anteriores.
 */
static void lerTerritorio(int indice, struct Territorio *t) {
    printf("\n--- Territorio %d de %d ---\n", indice + 1, NUM_TERRITORIOS);

    printf("Digite o nome do territorio (uma palavra, max. 29 caracteres): ");
    if (scanf(" %29s", t->nome) != 1) {
        t->nome[0] = '\0';
    }

    printf("Digite a cor do exercito (uma palavra, max. 9 caracteres): ");
    if (scanf(" %9s", t->cor) != 1) {
        t->cor[0] = '\0';
    }

    printf("Digite o numero de tropas: ");
    if (scanf("%d", &t->tropas) != 1) {
        t->tropas = 0;
    }
}

/*
 * Preenche todo o vetor: uma iteracao por posicao (5 territorios).
 * Cada chamada a lerTerritorio coleta nome, cor e tropas nessa ordem.
 */
static void registrarTodosTerritorios(struct Territorio territorios[], int quantidade) {
    printf("\n===================================\n");
    printf("\nWAR ESTRUTURADO - CADASTRO DE TERRITORIOS\n");
    printf("\n===================================\n");
    printf("\nVoce vai informar os dados de %d territorios.\n", quantidade);

    for (int i = 0; i < quantidade; i++) {
        lerTerritorio(i, &territorios[i]);
    }
}

/* Imprime todos os registros com rotulos e separadores claros. */
static void exibirTodosTerritorios(const struct Territorio territorios[], int quantidade) {
    printf("\n===================================\n");
    printf("\nMAPA DO MUNDO WAR - ESTADO ATUAL\n");
    printf("\n===================================\n");
    printf("\n========== Territorios cadastrados ==========\n");

    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritorio #%d\n", i + 1);
        printf("  Nome  : %s\n", territorios[i].nome);
        printf("  Cor   : %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
    }

    printf("\n===========================================\n");
    fflush(stdout); /* Garante que a saida apareca antes do encerramento do programa. */
}

static void esperarEnterParaSair(void) {
    int c;

    printf("\nPressione Enter para sair...");
    fflush(stdout);

    /* Normalmente sobra um '\n' no buffer depois do ultimo scanf.
     * Primeiro consumimos essa quebra de linha "pendente".
     * Em seguida, esperamos um novo Enter do usuario. */
    c = getchar();

    if (c == '\n' || c == '\r') {
        /* Espera o Enter real (após o prompt). */
        do {
            c = getchar();
        } while (c != '\n' && c != '\r' && c != EOF);
    } else if (c != EOF) {
        /* Se o usuario digitou algo antes de apertar Enter, descarta ate a quebra de linha. */
        do {
            c = getchar();
        } while (c != '\n' && c != '\r' && c != EOF);
    }
}

int main(void) {
    /* Tabela de tamanho fixo de struct Territorio - um elemento por vaga. */
    struct Territorio territorios[NUM_TERRITORIOS];

    /* Zera a tabela para que leituras com falha deixem campos vazios previsiveis. */
    memset(territorios, 0, sizeof(territorios));

    registrarTodosTerritorios(territorios, NUM_TERRITORIOS);

    /* Requisito: mostrar todos os dados logo apos o cadastro (sem atraso extra). */
    exibirTodosTerritorios(territorios, NUM_TERRITORIOS);

    /* Pausa para o usuario ver a saida antes do programa terminar. */
    esperarEnterParaSair();

    return 0;
}
