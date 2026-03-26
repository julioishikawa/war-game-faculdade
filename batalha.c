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
 
/*
 * Ataque bem-sucedido no formato literal do enunciado:
 * defensor muda de cor imediatamente e recebe metade das tropas do atacante.
 */
 static void aplicar_ataque(Territorio *atacante, Territorio *defensor) {
    /* Metade das tropas do atacante avança para o território conquistado. */
    int tropas_transferidas = atacante->tropas / 2;
    if (tropas_transferidas < 1) tropas_transferidas = 1;
    if (atacante->tropas - tropas_transferidas < 1) {
        /* Garante que o atacante mantenha pelo menos 1 tropa de origem. */
        tropas_transferidas = atacante->tropas - 1;
    }

    /* O defensor muda de dono: copia a cor do atacante. */
    strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
    defensor->cor[sizeof(defensor->cor) - 1] = '\0';

    /* Aplica transferência de tropas entre os territórios. */
    atacante->tropas -= tropas_transferidas;
    defensor->tropas = tropas_transferidas;

    printf("\nResultado: ataque venceu! %s foi conquistado por %s.\n",
           defensor->nome, atacante->cor);
 }
 
 /* Ataque falhou: atacante perde 1 tropa (se tiver mais de 1). */
 static void aplicar_derrota(Territorio *atacante) {
     if (atacante->tropas > 1) {
         atacante->tropas--;
     }
     printf("\nResultado: ataque falhou. %s perdeu 1 tropa.\n", atacante->nome);
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
 * - Atacante vence (dado_atacante > dado_defensor): defensor e conquistado,
 *   muda de cor e recebe metade das tropas do atacante.
 * - Defensor vence (empate ou dado_defensor >= dado_atacante): atacante perde 1 tropa.
 */
void atacar(Territorio *atacante, Territorio *defensor) {
     int dado_atacante = rolar_dado();
     int dado_defensor = rolar_dado();
 
     printf("\nRolagem de dados:\n");
     printf("  Atacante (%s): %d\n", atacante->nome, dado_atacante);
     printf("  Defensor (%s): %d\n", defensor->nome, dado_defensor);
 
     if (dado_atacante > dado_defensor) {
         aplicar_ataque(atacante, defensor);
     } else {
         aplicar_derrota(atacante);
     }
 }

void batalha_atacar(Territorio *atacante, Territorio *defensor) {
    /* Alias mantido para compatibilidade com versões anteriores do projeto. */
    atacar(atacante, defensor);
}