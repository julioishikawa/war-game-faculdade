/*
 * batalha.c
 * Implementa a lógica de validação e resolução de ataques entre territórios.
 * Cada chamada a atacar() resolve UMA rodada (uma rolagem de dados).
 * O usuario controla o ritmo voltando ao menu principal apos cada ataque.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "batalha.h"
 
 /* Rola um dado de 6 faces. */
 static int rolar_dado(void) {
     return (rand() % 6) + 1;
 }
 
/* Conquista somente quando o defensor chega a 0 tropas. */
static void conquistar_territorio(Territorio *atacante, Territorio *defensor) {
    int tropas_transferidas = atacante->tropas / 2;
    if (tropas_transferidas < 1) {
        tropas_transferidas = 1;
    }
    if (atacante->tropas - tropas_transferidas < 1) {
        tropas_transferidas = atacante->tropas - 1;
    }

    strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
    defensor->cor[sizeof(defensor->cor) - 1] = '\0';

    atacante->tropas -= tropas_transferidas;
    defensor->tropas = tropas_transferidas;

    printf("\nTerritorio conquistado! %s agora pertence a %s.\n",
           defensor->nome, atacante->cor);
}
 
 int batalha_ataque_valido(const Territorio *mapa, int idx_atacante, int idx_defensor) {
    /* Acesso por ponteiro para reforçar a manipulação do vetor dinâmico. */
     const Territorio *atacante = mapa + idx_atacante;
     const Territorio *defensor = mapa + idx_defensor;
 
     if (idx_atacante == idx_defensor) {
         printf("\nEscolha invalida: atacante e defensor nao podem ser o mesmo territorio.\n");
         return 0;
     }
     if (strcmp(atacante->cor, defensor->cor) == 0) {
         printf("\nEscolha invalida: nao e permitido atacar territorio da mesma cor.\n");
         return 0;
     }
     if (atacante->tropas < 2) {
         printf("\nEscolha invalida: o atacante precisa ter ao menos 2 tropas.\n");
         return 0;
     }
     return 1;
 }
 
/*
 * Executa UMA rolagem de dados entre atacante e defensor.
 * - Vitoria do atacante: defensor perde 1 tropa.
 * - Vitoria do defensor (ou empate): atacante perde 1 tropa.
 * - Conquista so ocorre quando defensor chega a 0 tropas.
 */
void atacar(Territorio *atacante, Territorio *defensor) {
     int dado_atacante = rolar_dado();
     int dado_defensor = rolar_dado();
 
     printf("\nRolagem de dados:\n");
     printf("  Atacante (%s): %d\n", atacante->nome, dado_atacante);
     printf("  Defensor (%s): %d\n", defensor->nome, dado_defensor);
 
    if (dado_atacante > dado_defensor) {
        if (defensor->tropas > 0) {
            defensor->tropas--;
        }
        printf("\nResultado: ataque venceu! %s perdeu 1 tropa.\n", defensor->nome);

        if (defensor->tropas == 0) {
            conquistar_territorio(atacante, defensor);
        }
    } else {
        if (atacante->tropas > 1) {
            atacante->tropas--;
        }
        printf("\nResultado: ataque falhou. %s perdeu 1 tropa.\n", atacante->nome);
    }
 }

void batalha_atacar(Territorio *atacante, Territorio *defensor) {
    /* Alias mantido para compatibilidade com versões anteriores do projeto. */
    atacar(atacante, defensor);
}