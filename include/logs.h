/**
 * @file logs.h
 * @brief Header com protótipos das funções de registo e consulta de logs.
 * @author Francisco Alves
 */

#ifndef LOGS_H
#define LOGS_H

/**
 * @brief Regista uma mensagem no ficheiro de logs do sistema.
 * @param mensagem Mensagem a registar.
 */
void registar_log(const char *mensagem);

/**
 * @brief Exibe o conteúdo do ficheiro de logs no ecrã.
 */
void consultar_logs();

#endif/* LOGS_H */
