/**
 * @file ordem.h
 * @brief Header com as estruturas e protótipos do módulo de gestão de ordens/manutenções.
 * @author Francisco Alves
 */

#ifndef ORDEM_H
#define ORDEM_H

#include "ativos.h"
#include "departamentos.h"
#include "tecnicos.h"
#include "materiais.h"

typedef enum {
    PENDENTE,
    EXECUCAO,
    CONCLUIDA,
    CANCELADA
}EstadoOrdem;

typedef enum {
    BAIXA =1,
    MEDIA,
    ALTA
}Prioridade;

typedef enum {
    PREVENTIVA = 1,
    CORRETIVA
}TipoManutencao;

typedef struct {
    int idOrdem;
    int idAtivo;
    int idDepartamento;
    int idTecnico;
    EstadoOrdem estado;
    Prioridade prioridade;
    TipoManutencao tipo_manutencao;

    /* datas/horas para calcular tempo de resolução */
    int diaInicio;
    int mesInicio;
    int anoInicio;
    int horaInicio;
    int minInicio;
    int segInicio;

    int diaFim;
    int mesFim;
    int anoFim;
    int horaFim;
    int minFim;
    int segFim;
}Ordem;

typedef struct {
    Ordem *ordem;
    int contador;
    int ordensAtivas;
    int capacidade;
}Ordens;

/**
 * @brief Lista as ordens/manutenções pendentes.
 * @param ordens Estrutura com a lista de ordens.
 */
void listar_ordens_pendentes (Ordens ordens);

/**
 * @brief Lista todas as ordens/manutenções registadas.
 * @param ordens Estrutura com a lista de ordens.
 */
void listar_ordens (Ordens ordens);

/**
 * @brief Cria uma nova ordem/manutenção e associa um ativo.
 * @param ativos Apontador para a estrutura de ativos.
 * @param ordens Apontador para a estrutura de ordens.
 * @param departamentos Estrutura de departamentos.
 */
void criar_ordem (Ativos *ativos, Ordens *ordens, Departamentos departamentos);

/**
 * @brief Geração/gestão de ordens, consoante o estado (pendente/em execução).
 * @param ordens Apontador para a estrutura de ordens.
 * @param tecnicos Apontador para a estrutura de técnicos.
 * @param ativos Apontador para a estrutura de ativos.
 * @param materiais Apontador para a estrutura de materiais.
 */
void gerir_ordem (Ordens *ordens, Tecnicos *tecnicos, Ativos *ativos, Materiais *materiais);

/**
 * @brief Guarda as ordens num ficheiro binário.
 * @param ordens Apontador para a estrutura de ordens.
 */
void guardarOrdens (Ordens *ordens);

/**
 * @brief Carrega as ordens a partir de um ficheiro binário.
 * @param ordens Apontador para a estrutura de ordens.
 */
void carregarOrdens (Ordens *ordens);

/**
 * @brief Obtém o maior ID existente na lista de ordens.
 * @param ordens Estrutura com a lista de ordens.
 * @return Retorna o maior ID ou 0 caso não existam ordens.
 */
int obterMaiorIDOrdens(Ordens ordens);

/**
 * @brief Conta quantas manutenções (EXECUCAO) estão associadas a um técnico.
 * @param tecnico Estrutura do técnico.
 * @param ordens Estrutura com a lista de ordens.
 * @return Número de manutenções ativas associadas ao técnico.
 */
int numeroManutencoesTecnico (Tecnico tecnico, Ordens ordens);

/**
 * @brief Calcula a taxa de ocupação de um técnico.
 * @param tecnico Apontador para um técnico.
 * @param ordens Apontador para a lista de ordens.
 * @return Retorna a taxa de ocupação em percentagem.
 */
int mostrarTaxaOcupacaoTecnico (Tecnico *tecnico, Ordens *ordens);

/**
 * @brief Converte o estado de uma ordem (enum) para texto.
 * @param est Estado da ordem.
 * @return Retorna uma string com o estado.
 */
const char *passar_int_string_estado_tecnicos (EstadoOrdem est);

/**
 * @brief Lista as ordens filtrando por estado.
 * @param ordens Apontador para a estrutura de ordens.
 * @param estado Estado a listar.
 * @param materiais Estrutura com a lista de materiais.
 */
void listarOrdensEstado (Ordens *ordens, EstadoOrdem estado, Materiais materiais);

/**
 * @brief Lista as ordens filtrando por prioridade.
 * @param ordens Apontador para a estrutura de ordens.
 * @param prioridade Prioridade a listar.
 * @param materiais Estrutura com a lista de materiais.
 */
void listarOrdensPrioridade (Ordens *ordens, Prioridade prioridade, Materiais materiais);

/**
 * @brief Lista as ordens filtrando por tipo de manutenção.
 * @param ordens Apontador para a estrutura de ordens.
 * @param tipo Tipo de manutenção.
 * @param materiais Estrutura com a lista de materiais.
 */
void listarOrdensTipo (Ordens *ordens, TipoManutencao tipo, Materiais materiais);

#endif /* ORDEM_H */
