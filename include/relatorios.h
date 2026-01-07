/**
 * @file relatorios.h
 * @brief Header com protótipos das funções de relatórios/análises do sistema.
 * @author Francisco Alves
 */

#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "ordem.h"
#include "materiais.h"

/**
 * @brief Mostra o relatório geral de ativos.
 * @param ativos Apontador para a estrutura com a lista de ativos.
 */
void mostrarRelatorioAtivos(Ativos *ativos);

/**
 * @brief Mostra o relatório geral de departamentos.
 * @param departamentos Apontador para a estrutura com a lista de departamentos.
 * @param ativos Apontador para a estrutura com a lista de ativos.
 * @param ordens Apontador para a estrutura com a lista de ordens.
 */
void mostrarRelatorioDepartamentos (Departamentos *departamentos, Ativos *ativos, Ordens *ordens);

/**
 * @brief Mostra o relatório geral de técnicos.
 * @param tecnicos Apontador para a estrutura com a lista de técnicos.
 * @param ordens Apontador para a estrutura com a lista de ordens.
 */
void mostrarRelatorioTecnicos (Tecnicos *tecnicos, Ordens *ordens);

/**
 * @brief Mostra o relatório geral das ordens/manutenções.
 * @param ordens Apontador para a estrutura com a lista de ordens.
 * @param materiais Estrutura com a lista de materiais.
 */
void mostrarRelatorioOrdens (Ordens *ordens, Materiais materiais);

/**
 * @brief Mostra um alerta de ativos instáveis (com muitas ocorrências).
 * @param ativos Estrutura com a lista de ativos.
 * @param ordens Estrutura com a lista de ordens.
 */
void relatorioAtivosInstaveis(Ativos ativos, Ordens ordens);

/**
 * @brief Mostra uma análise de incidências por local.
 * @param ativos Estrutura com a lista de ativos.
 * @param ordens Estrutura com a lista de ordens.
 */
void relatorioProblemasPorLocal(Ativos ativos, Ordens ordens);

#endif /* RELATORIOS_H */
