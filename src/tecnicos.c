/**
 * @file tecnicos.c
 * @brief Ficheiro com funções que permitem a gestão dos técnicos.
 * @author Francisco Alves
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tecnicos.h"
#include "../include/input.h"
#include "../include/logs.h"

#include "../include/ordem.h"
#define CRESCIMENTO_5 5

/**
 * @brief Função que procura o maior ID registado nos técnicos.
 * @param tecnicos Estrutura com o array de técnicos e contador.
 * @return Retorna o maior ID registado. Caso não existam técnicos devolve 0.
 */
int obterMaiorIDTecnicos(Tecnicos tecnicos) {
    int maxID = 0;
    for (int i = 0; i < tecnicos.contador; i++) {
        if (tecnicos.tecnico[i].idTecnico > maxID) {
            maxID = tecnicos.tecnico[i].idTecnico;
        }
    }
    return maxID;
}

/**
 * @brief Função que gera o próximo ID a ser atribuido a um novo técnico.
 * @details Percorre todos os técnicos existentes e devolve o maior ID + 1.
 * Caso não existam técnicos, devolve 10.
 * @param tecnicos Apontador para a estrutura Tecnicos.
 * @return Retorna o próximo ID disponível.
 */
static int gerarProximoID(Tecnicos *tecnicos) {

    if (tecnicos->contador == 0) {
        return 10;
    }

    int maxID = 0;
    for (int i = 0; i < tecnicos->contador; i++) {
        if (tecnicos->tecnico[i].idTecnico > maxID) {
            maxID = tecnicos->tecnico[i].idTecnico;
        }
    }

    return maxID + 1;
}



/**
 * @brief Função auxiliar para garantir que existe memória suficiente na heap para os técnicos.
 * @note Caso a capacidade atual seja insuficiente, a função utiliza realloc para aumentar o array
 * em blocos de 5 (CRESCIMENTO_5).
 * @param tecnicos Apontador para estrutura que contém o array de técnicos.
 * @param minCap Capacidade mínima pretendida.
 * @return Retorna 1 caso a alocação seja feita com sucesso, caso contrário retorna 0.
 * @warning Em caso de falha de memória, a função imprime uma mensagem e não faz alterações.
 */
static int garantir_capacidade_tecnicos(Tecnicos *tecnicos, int minCap) {
    if (tecnicos->capacidade >= minCap) return 1;

    int novaCap = tecnicos->capacidade;
    if (novaCap <= 0) novaCap = 0;
    while (novaCap < minCap) novaCap += CRESCIMENTO_5;

    void *tmp = realloc(tecnicos->tecnico, (size_t)novaCap * sizeof(*tecnicos->tecnico));
    if (tmp == NULL) {
        printf("Erro: sem memória para alocar técnicos.\n");
        registar_log("Erro: Falha no realloc ao tentar expandir a lista de técnicos.");
        return 0;
    }

    tecnicos->tecnico = (Tecnico *)tmp;
    tecnicos->capacidade = novaCap;
    return 1;
}

/**
 * @brief Função auxiliar que converte a especialidade (enum) para uma string legível.
 * @param esp Especialidade do técnico.
 * @return Retorna uma string literal com a descrição da especialidade.
 */
const char *passar_int_string_especialidade(Especialidade esp) {
    switch (esp) {
        case TECNICO_TI:
            return "Tecnico TI";
        case MECANICO:
            return "Mecânico";
        case ELETRICISTA:
            return "Eletricista";
        case MANUTENCAO_GERAL:
            return "Manutenção Geral";
        case OUTROS:
            return "Outros";
        default:
            return "Desconhecido";
    }
}

/**
 * @brief Função auxiliar que converte o estado do técnico (enum) para uma string legível.
 * @param tecnicos Estrutura com o array de técnicos.
 * @return Retorna uma string literal com a descrição do estado.
 * @note A função utiliza o estado do primeiro elemento (tecnicos.tecnico->estado_tecnico).
 */
const char *passar_int_string_estado (EstadoTecnico estado_tecnico) {
    switch (estado_tecnico) {
        case ATIVO1:
            return "Ativo";
        case OCUPADO:
            return "Ocupado";
        case INATIVO1:
            return "Inativo";
        default:
            return "Desconhecido";
    }
}

/**
 * @brief Função que procura um técnico pelo ID.
 * @param tecnicos Estrutura com o array de técnicos e contador.
 * @param idProcurado ID que o utilizador pretende procurar.
 * @return Retorna o índice do técnico no array caso seja encontrado, caso contrário retorna -1.
 */
int procurar_tecnico_id (Tecnicos tecnicos, int idProcurado) {
    for (int i = 0; i<tecnicos.contador; i++) {
        if (tecnicos.tecnico[i].idTecnico == idProcurado) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Função que cria um novo técnico.
 * @param tecnicos Apontador para a estrutura de técnicos.
 * @note A função lê o nome e a especialidade do técnico, atribui estado ATIVO1 e gera um novo ID.
 * @warning Esta função incrementa os contadores tecnicos->contador e tecnicos->tecnicosAtivos.
 */
void criar_tecnico (Tecnicos *tecnicos) {
    if (tecnicos == NULL) return;
    if (!garantir_capacidade_tecnicos(tecnicos, tecnicos->contador + 1)) {
        pausar_ecra();
        return;
    }

    int idx = tecnicos->contador;

    tecnicos->tecnico[idx].nome = lerStringDinamica("Indique o nome do técnico:\n");
    tecnicos->tecnico[idx].especialidade = obterIntIntervalado(1, 5,
        "Indique a especialidade do técnico:\n1 - Técnico TI\n2 - Mecânico\n3 - Eletricista\n4 - Manutenção Geral\n5 - Outras\n");
    tecnicos->tecnico[idx].estado_tecnico = ATIVO1;
    tecnicos->tecnico[idx].idTecnico = gerarProximoID(tecnicos);

    tecnicos->contador++;
    tecnicos->tecnicosAtivos++;

    registar_log("Info: Foi criado um novo técnico.");
    pausar_ecra();
}

/**
 * @brief Função que lista todos os técnicos registados.
 * @param tecnicos Estrutura com o array de técnicos e contador.
 * @note A listagem é feita via printf.
 */
void listar_tecnicos (Tecnicos tecnicos) {
    printf ("\n===== TECNICOS =====\n");
    for (int i=0; i < tecnicos.contador; i++) {
        printf ("ID: %d", tecnicos.tecnico->idTecnico);
        printf("Nome: %s\n", tecnicos.tecnico[i].nome);
        printf("Especialidade: %s",passar_int_string_especialidade(tecnicos.tecnico[i].especialidade));
        printf("Estado: %s", passar_int_string_estado(tecnicos.tecnico[i].estado_tecnico));
    }
}

/**
 * @brief Função que desativa (inativa) um técnico.
 * @param tecnicos Estrutura com o array de técnicos e contador.
 * @note A função pede um ID, valida e depois altera o estado para INATIVO1 caso o utilizador confirme.
 * @warning O técnico não é removido do array; apenas é marcado como INATIVO1.
 */
void desativar_tecnico (Tecnicos tecnicos) {
    int idProcurado, idEncontrado;
    int escolha;
    int maxIdTecnicos;
    maxIdTecnicos = obterMaiorIDTecnicos(tecnicos);
    printf("\n===== DESATIVAR TECNICO =====\n");
    idProcurado = obterIntIntervalado(0, maxIdTecnicos, "Indique o id que deseja procurar: \n");
    if (tecnicos.tecnico[idProcurado].estado_tecnico == INATIVO1 || tecnicos.tecnico[idProcurado].estado_tecnico == OCUPADO) {
        printf("ID inválido.\n");
        pausar_ecra();
        return;
    }
    idEncontrado = procurar_tecnico_id(tecnicos, idProcurado);
    escolha = obterIntIntervalado(1, 2,"Tem a certeza que deseja apagar o técnico de id %d? (1) Sim (2) Não\n");
    if (escolha == 1) {
        tecnicos.tecnico[idEncontrado].estado_tecnico = INATIVO1;
        printf ("O técnico foi apagado com sucesso.\n");
        registar_log("Info: Um técnico foi desativado.");
        pausar_ecra();
    }
    if (escolha == 2) {
        printf("Operação cancelada.\n");
        pausar_ecra();
    }
}

/**
 * @brief Função que guarda os técnicos num ficheiro binário.
 * @param tecnicos Apontador para a estrutura de técnicos.
 * @warning Escreve no ficheiro "tecnicos.bin". Caso o ficheiro não possa ser aberto, a função termina.
 */
void guardarTecnicos(Tecnicos *tecnicos) {
    FILE *fp = fopen("tecnicos.bin", "wb");

    if (fp == NULL) {
        printf("Erro ao abrir ficheiro de técnicos para escrita.");
        registar_log("Erro: Não foi possivel abrir tecnicos.bin para escrita.");
        return;
    }

    fwrite (&tecnicos->contador, sizeof(int), 1, fp);
    fwrite (&tecnicos->tecnicosAtivos, sizeof(int), 1, fp);

    for (int i = 0; tecnicos->contador; i++) {
        fwrite (&tecnicos->tecnico[i].idTecnico, sizeof(int), 1, fp);
        fwrite (&tecnicos->tecnico[i].idManutencaoAssociado, sizeof(int), 1, fp);
        fwrite (&tecnicos->tecnico[i].especialidade, sizeof(Especialidade), 1, fp);
        fwrite (&tecnicos->tecnico[i].estado_tecnico, sizeof(EstadoTecnico), 1, fp);
        escreverStringBinario(tecnicos->tecnico[i].nome, fp);
    }
    fclose (fp);
}

/**
 * @brief Função que carrega os técnicos a partir de um ficheiro binário.
 * @param tecnicos Apontador para a estrutura de técnicos.
 * @note Caso o ficheiro "tecnicos.bin" não exista, a função termina sem alterar nada.
 * @warning A função utiliza malloc para alocar memória para o array de técnicos.
 */
void carregarTecnicos(Tecnicos *tecnicos) {
    FILE *fp = fopen("tecnicos.bin", "rb");
    if (fp == NULL) return;

    fread (&tecnicos->contador, sizeof(int), 1, fp);
    fread (&tecnicos->tecnicosAtivos, sizeof(int), 1, fp);

    tecnicos->capacidade = tecnicos->contador;
    tecnicos->tecnico = malloc(tecnicos->capacidade * sizeof(Tecnico));
    if (tecnicos->tecnico == NULL && tecnicos->contador > 0) {
        registar_log("Erro: Falha ao alocar memória ao carregar técnicos (malloc devolveu NULL).");
        fclose(fp);
        return;
    }

    for (int i = 0; i < tecnicos->contador; i++) {
        fread (&tecnicos->tecnico[i].idTecnico, sizeof(int), 1, fp);
        fread (&tecnicos->tecnico[i].idManutencaoAssociado, sizeof(int), 1, fp);
        fread (&tecnicos->tecnico[i].especialidade, sizeof(Especialidade), 1, fp);
        fread (&tecnicos->tecnico[i].estado_tecnico, sizeof(EstadoTecnico), 1, fp);
        tecnicos->tecnico[i].nome = lerStringBinario(fp);
    }
    fclose(fp);
}