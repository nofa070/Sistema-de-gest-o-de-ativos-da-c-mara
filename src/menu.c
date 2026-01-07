/**
 * @file menu.c
 * @brief Ficheiro que contém a função de apresentação do menu principal.
 * @author Francisco Alves
*/
#include <stdio.h>

/**
 * @brief Função que apresenta o menu principal.
 */
void apresentar_menu () {
    printf ("\n===== MENU =====\n");
    printf ("1 - Gerir Ativos\n2 - Gerir Departamentos\n3 - Gerir técnicos\n4 - Gerir manutenções\n5 - Relatórios e outros.\n6 - Sair\n");
}