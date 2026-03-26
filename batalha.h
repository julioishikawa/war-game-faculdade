#ifndef BATALHA_H
#define BATALHA_H

/*
 * batalha.h
 * Declara as funções de validação e resolução de ataques entre territórios.
 */

#include "territorio.h"

/*
 * Verifica se o par atacante/defensor e valido:
 *   - não pode ser o mesmo território;
 *   - não pode atacar território da mesma cor;
 *   - atacante precisa ter pelo menos 2 tropas.
 * Retorna 1 se válido, 0 caso contrário.
 */
int batalha_ataque_valido(const Territorio *mapa, int idx_atacante, int idx_defensor);

/*
 * Resolve uma batalha com dado de 1 a 6 para cada lado e aplica os resultados:
 *   - Vitoria do atacante: defensor muda de cor e recebe metade das tropas do atacante.
 *   - Derrota do atacante: atacante perde 1 tropa (se tiver mais de 1).
 */
void atacar(Territorio *atacante, Territorio *defensor);

/* Alias para manter compatibilidade com código legado do projeto. */
void batalha_atacar(Territorio *atacante, Territorio *defensor);

#endif /* BATALHA_H */
