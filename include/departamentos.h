/**
 * @file departamentos.h
 * @brief Header com as estruturas e protótipos do módulo de gestão de departamentos.
 * @author Francisco Alves
 */

#ifndef  DEPARTAMENTOS_H
#define DEPARTAMENTOS_H

typedef enum {
    ATIVO = 1,
    INATIVO
}Atividade;

typedef struct {
    int idDepartamento;
    char *nomeDepartamento;
    char *responsavel;
    char *contacto;
    Atividade atividade;
}Departamento;

typedef struct {
    Departamento *departamento;
    int contador;
    int departamentosAtivos;
    int capacidade;
}Departamentos;

/**
 * @brief Cria um novo departamento.
 * @param departamentos Apontador para a estrutura com a lista de departamentos.
 */
void criarDepartamento (Departamentos *departamentos);

/**
 * @brief Lista os departamentos registados.
 * @param departamentos Estrutura com a lista de departamentos.
 */
void listar_departamentos (Departamentos departamentos);

/**
 * @brief Atualiza os dados de um departamento existente.
 * @param departamentos Apontador para a estrutura com a lista de departamentos.
 */
void atualizar_departamento(Departamentos *departamentos);

/**
 * @brief Inativa um departamento (não remove do array, apenas marca como INATIVO).
 * @param departamentos Apontador para a estrutura com a lista de departamentos.
 */
void inativar_Departamento (Departamentos *departamentos);

/**
 * @brief Guarda os departamentos num ficheiro binário.
 * @param departamentos Apontador para a estrutura com a lista de departamentos.
 */
void guardarDepartamentos (Departamentos *departamentos);

/**
 * @brief Carrega os departamentos a partir de um ficheiro binário.
 * @param departamentos Apontador para a estrutura com a lista de departamentos.
 */
void carregarDepartamentos(Departamentos *departamentos);

/**
 * @brief Obtém o maior ID existente na lista de departamentos.
 * @param departamentos Estrutura com a lista de departamentos.
 * @return Retorna o maior ID ou 0 caso não existam departamentos.
 */
int obterMaiorIDDepartamento(Departamentos departamentos);

#endif /* DEPARTAMENTOS_H */