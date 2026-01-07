/**
 * @file ativos.h
 * @brief Header com as estruturas e protótipos do módulo de gestão de ativos.
 * @author Francisco Alves
 */

#ifndef ATIVOS_H
#define ATIVOS_H

#include "departamentos.h"

typedef enum {
    VIATURA = 1,
    INFORMATICA,
    MOBILIARIO,
    FERRAMENTA,
    OUTRO
} CategoriaAtivo;

typedef enum {
    OPERACIONAL,
    EM_MANUTENCAO,
    ABATIDO,
    INATIVO2
} EstadoAtivo;

typedef struct {
    int id;
    char *designacao;
    CategoriaAtivo categoria;
    char *localizacao;
    int diaAquisicao;
    int mesAquisicao;
    int anoAquisicao;
    int diaAbate;
    int mesAbate;
    int anoAbate;
    EstadoAtivo estado;
    int contagemManutencoesCorretivas;
    float custoTotalAcumulado;
    float custo;
    int idDepartamentoAssociado;
} Ativo;

typedef struct {
    Ativo *ativo;
    int contador;
    int  ativosDisponiveis;
    int capacidade;
}Ativos;


/**
 * @brief Cria um novo ativo no sistema e associa-o a um departamento.
 * @param ativos Apontador para a estrutura com a lista de ativos.
 * @param departamentos Apontador para a estrutura com a lista de departamentos.
 */
void criar_ativo (Ativos *ativos, Departamentos *departamentos);

/**
 * @brief Procura um ativo através do seu ID.
 * @param ativos Apontador para a estrutura com a lista de ativos.
 * @param idProcurado ID a procurar.
 * @return Retorna o índice do ativo no array ou -1 caso não encontre.
 */
int procurar_ativo_id (Ativos *ativos, int idProcurado);

/**
 * @brief Lista todos os ativos registados.
 * @param ativos Estrutura com a lista de ativos.
 */
void listar_ativos(Ativos ativos);

/**
 * @brief Lista os ativos agrupados por departamento.
 * @param departamentos Estrutura com a lista de departamentos.
 * @param ativos Estrutura com a lista de ativos.
 */
void listar_ativos_por_departamento (Departamentos departamentos, Ativos ativos);

/**
 * @brief Abate um ativo (marca como ABATIDO) e regista a data.
 * @param ativos Apontador para a estrutura com a lista de ativos.
 */
void abater_ativo (Ativos *ativos);

/**
 * @brief Guarda os ativos num ficheiro binário.
 * @param ativos Apontador para a estrutura com a lista de ativos.
 */
void guardarAtivos(Ativos *ativos);

/**
 * @brief Carrega os ativos a partir de um ficheiro binário.
 * @param ativos Apontador para a estrutura com a lista de ativos.
 */
void carregarAtivos(Ativos *ativos);

/**
 * @brief Obtém o maior ID existente na lista de ativos.
 * @param ativos Estrutura com a lista de ativos.
 * @return Retorna o maior ID ou 0 caso não existam ativos.
 */
int obterMaiorIDAtivos(Ativos ativos);

/**
 * @brief Pede ao utilizador um termo e lista os ativos que começam por esse termo.
 * @param ativos Estrutura com a lista de ativos.
 */
void listarComPesquisaInteligente (Ativos ativos);

#endif /* ATIVOS_H */
