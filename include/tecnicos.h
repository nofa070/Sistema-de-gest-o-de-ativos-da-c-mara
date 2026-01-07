/**
 * @file tecnicos.h
 * @brief Header com as estruturas e protótipos do módulo de gestão de técnicos.
 * @author Francisco Alves
 */

#ifndef TECNICOS_H
#define TECNICOS_H

/**
 * @brief Especialidades dos técnicos.
 */
typedef enum {
    TECNICO_TI = 1,     /**< Técnico de TI */
    MECANICO,           /**< Mecânico */
    ELETRICISTA,        /**< Eletricista */
    MANUTENCAO_GERAL,  /**< Manutenção Geral */
    OUTROS              /**< Outras especialidades */
}Especialidade;

/**
 * @brief Estados possíveis para um técnico.
 */
typedef enum {
    ATIVO1,     /**< Técnico ativo */
    OCUPADO,    /**< Técnico ocupado */
    INATIVO1    /**< Técnico inativo */
}EstadoTecnico;

/**
 * @brief Estrutura que representa um técnico.
 */
typedef struct {
    int idTecnico;                /**< Identificador único do técnico */
    char *nome;                   /**< Nome do técnico */
    Especialidade especialidade;  /**< Especialidade do técnico */
    EstadoTecnico estado_tecnico; /**< Estado atual do técnico */
    int idManutencaoAssociado;    /**< ID da manutenção associada (se houver) */
}Tecnico;

/**
 * @brief Estrutura que representa a lista de técnicos.
 */
typedef struct {
    Tecnico *tecnico;  /**< Array de técnicos */
    int contador;      /**< Contador de técnicos na lista */
    int tecnicosAtivos;/**< Contador de técnicos ativos */
    int capacidade;    /**< Capacidade máxima da lista de técnicos */
}Tecnicos;

/**
 * @brief Cria um novo técnico.
 * @param tecnicos Apontador para a estrutura com a lista de técnicos.
 */
void criar_tecnico (Tecnicos *tecnicos);

/**
 * @brief Lista os técnicos registados.
 * @param tecnicos Estrutura com a lista de técnicos.
 */
void listar_tecnicos (Tecnicos tecnicos);

/**
 * @brief Desativa um técnico (marca como INATIVO1).
 * @param tecnicos Estrutura com a lista de técnicos.
 */
void desativar_tecnico (Tecnicos tecnicos);

/**
 * @brief Procura um técnico pelo ID.
 * @param tecnicos Estrutura com a lista de técnicos.
 * @param idProcurado ID a procurar.
 * @return Retorna o índice do técnico no array ou -1 caso não encontre.
 */
int procurar_tecnico_id (Tecnicos tecnicos, int idProcurado);

/**
 * @brief Guarda os técnicos num ficheiro binário.
 * @param tecnicos Apontador para a estrutura com a lista de técnicos.
 */
void guardarTecnicos(Tecnicos *tecnicos);

/**
 * @brief Carrega os técnicos a partir de um ficheiro binário.
 * @param tecnicos Apontador para a estrutura com a lista de técnicos.
 */
void carregarTecnicos(Tecnicos *tecnicos);

/**
 * @brief Obtém o maior ID existente na lista de técnicos.
 * @param tecnicos Estrutura com a lista de técnicos.
 * @return Retorna o maior ID ou 0 caso não existam técnicos.
 */
int obterMaiorIDTecnicos(Tecnicos tecnicos);

/**
 * @brief Converte a especialidade (enum) para texto.
 * @param esp Especialidade.
 * @return Retorna uma string com a especialidade.
 */
const char *passar_int_string_especialidade(Especialidade esp);

/**
 * @brief Converte o estado do técnico (enum) para texto.
 * @param estado_tecnico Estado do técnico.
 * @return Retorna uma string com o estado.
 */
const char *passar_int_string_estado (EstadoTecnico estado_tecnico);

#endif /* TECNICOS_H */
