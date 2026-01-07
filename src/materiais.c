/**
 * @file materiais.c
 * @brief Ficheiro que contém as funções relacionadas aos materiais
 * @author Francisco Alves
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/materiais.h"
#include "../include/input.h"
#include "../include/logs.h"
#define CRESCIMENTO_5 5


/**
 * @brief Função para garantir que existe memória suficiente na heap para os materiais.
 * @note se a quantidade atual for insuficiente, a função utiliza realloc para alocar mais memória.
 * @param materiais Apontador para estrutura que contém a lista de materiais.
 * @param minCap Capacidade mínima
 * @return Retorna 1 caso a alocação de memória seja feita com sucesso, caso contrário retorna 0.
 */
static int garantirCapacidadeMateriais (Materiais *materiais, int minCap) {
    if (materiais->capacidade >= minCap) {
        return 1;
    }
    int novaCap = materiais->capacidade;
    if (novaCap <= 0) novaCap = 0;
    while (novaCap < minCap) novaCap += CRESCIMENTO_5;

    void *tmp = realloc (materiais->material, (size_t)novaCap * sizeof(Material));
    if (tmp == NULL) {
        printf("Erro ao alocar memória para materiais.");
        registar_log("Erro: Falha no realloc ao tentar expandir a lista de materiais.");
        return 0;
    }
    materiais->material = tmp;
    materiais->capacidade = novaCap;
    return 1;
}

/**
 * @brief Esta função serve para criar materiais novos que serão usados nas manutenções.
 * @param materiais Apontador para a estrutura que contém a lista de materiais e contador.
 * @param idx ID da ordem que será associada a este material.
 * @note Esta função utiliza funções como lerSringDinamica() e obterIntPositivo() para
 * ler strings e valores int.
 */
void adicionar_materiais (Materiais *materiais, int idx) {
    if (!garantirCapacidadeMateriais(materiais, materiais->contador + 1)) {
        pausar_ecra();
        return;
    }
    materiais->material[materiais->contador].nomeMaterial = lerStringDinamica("Indique o nome do material que deseja usar:\n");
    materiais->material[materiais->contador].custoUnitário = obterFloatPositivo("Indique o preço por unidade do material:\n");
    materiais->material[materiais->contador].OrdemAssociada = idx;
    materiais->material[materiais->contador].quantidade = obterIntPositivo("Indique a quantidade deste material que deseja usar que deseja usar:\n");
    materiais->contador++;
    registar_log("Info: Foi adicionado um material a uma ordem/manutenção.");
}

/**
 * @brief Função que escreve as informações dos materiais num ficheiro binário.
 * @param materiais Apontador para a estrutura que contém as informações que serão escritas no ficheiro.
 */
void guardarMateriais (Materiais *materiais) {

    FILE *fp = fopen ("materiais.bin", "wb");
    if (fp == NULL) {
        registar_log("Erro: Não foi possível abrir materiais.bin para escrita.");
        return;
    }

    fwrite(&materiais->contador, sizeof(int), 1, fp);

    for (int i = 0; i<materiais->contador; i++) {
        fwrite(&materiais->material[i].quantidade, sizeof(int), 1, fp);
        fwrite (&materiais->material[i].custoUnitário, sizeof(float), 1, fp);
        fwrite(&materiais->material[i].OrdemAssociada, sizeof(int), 1, fp);
        escreverStringBinario(materiais->material[i].nomeMaterial, fp);
    }

}

/**
 * @brief Função que guarda as informações dos materiais num ficheiro binário.
 * @param materiais Apontador para a estrutura com as informações que serão guardadas.
 * @warning A função utiliza malloc pelo que será necessário posteriormente libertar a memória heap.
 */
void carregarMateriais (Materiais *materiais) {
    FILE *fp = fopen ("materiais.bin", "rb");

    if (fp == NULL) return;

    fread(&materiais->contador, sizeof(int), 1, fp);
    materiais->capacidade = materiais->contador;

    materiais->material = malloc(materiais->capacidade * sizeof(Material));
    if (materiais->material == NULL && materiais->contador > 0) {
        registar_log("Erro: Falha ao alocar memória ao carregar materiais (malloc devolveu NULL).");
        fclose(fp);
        return;
    }

    for (int i = 0; i < materiais->contador; i++) {
        fread(&materiais->material[i].custoUnitário, sizeof(float), 1, fp);
        fread(&materiais->material[i].quantidade, sizeof(int), 1, fp);
        fread(&materiais->material[i].OrdemAssociada, sizeof(int), 1, fp);
        materiais->material[i].nomeMaterial = lerStringBinario(fp);
    }
}