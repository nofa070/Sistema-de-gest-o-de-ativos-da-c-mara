/**
 * @file materiais.h
 * @brief Header com as estruturas e protótipos do módulo de gestão de materiais.
 * @author Francisco Alves
 */

#ifndef MATERIAIS_H
#define MATERIAIS_H

typedef struct {
    char *nomeMaterial;
    int quantidade;
    float custoUnitário;
    int OrdemAssociada;
}Material;

typedef struct {
    Material *material;
    int contador;
    int capacidade;
}Materiais;

/**
 * @brief Adiciona um material associado a uma ordem/manutenção.
 * @param materiais Apontador para a estrutura com a lista de materiais.
 * @param idx ID da ordem que será associada ao material.
 */
void adicionar_materiais (Materiais *materiais, int idx);

/**
 * @brief Guarda os materiais num ficheiro binário.
 * @param materiais Apontador para a estrutura com a lista de materiais.
 */
void guardarMateriais (Materiais *materiais);

/**
 * @brief Carrega os materiais a partir de um ficheiro binário.
 * @param materiais Apontador para a estrutura com a lista de materiais.
 */
void carregarMateriais (Materiais *materiais);

#endif /* MATERIAIS_H */
