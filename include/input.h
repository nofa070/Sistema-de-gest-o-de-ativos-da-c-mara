/**
 * @file input.h
 * @brief Header com protótipos das funções de leitura/validação de input.
 * @author Francisco Alves
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

/**
 * @brief Limpa o buffer de entrada.
 * @note Função interna ao módulo de input.
 */
static void cleanInputBuffer();

/**
 * @brief Solicita ao utilizador um inteiro positivo.
 * @param msg Mensagem a apresentar.
 * @return Valor inteiro positivo.
 */
int obterIntPositivo(char *msg);

/**
 * @brief Lê uma string para um buffer com tamanho máximo.
 * @param string Buffer onde a string será guardada.
 * @param tamanho Tamanho do buffer.
 * @param msg Mensagem a apresentar.
 */
void lerString(char *string, unsigned int tamanho, char *msg);

/**
 * @brief Lê uma string com alocação dinâmica.
 * @param msg Mensagem a apresentar.
 * @return Apontador para a string alocada (deve ser libertada com free).
 */
char* lerStringDinamica(const char *msg);

/**
 * @brief Lê e valida um nome (sem números, primeira letra maiúscula, etc.).
 * @param string Buffer onde o nome será guardado.
 * @param tamanho Tamanho do buffer.
 * @param msg Mensagem a apresentar.
 */
void lerNomeValido(char *string, unsigned int tamanho, char *msg);

/**
 * @brief Pausa o ecrã à espera que o utilizador carregue numa tecla.
 */
void pausar_ecra(void);

/**
 * @brief Solicita um inteiro dentro de um intervalo.
 * @param minValor Valor mínimo.
 * @param maxValor Valor máximo.
 * @param msg Mensagem a apresentar.
 * @return Valor dentro do intervalo.
 */
int obterIntIntervalado(int minValor, int maxValor, char *msg);

/**
 * @brief Solicita um float positivo.
 * @param msg Mensagem a apresentar.
 * @return Valor float positivo.
 */
float obterFloatPositivo(char *msg);

/**
 * @brief Escreve uma string no ficheiro binário (tamanho + conteúdo).
 * @param str String a escrever.
 * @param fp Ficheiro já aberto.
 */
void escreverStringBinario(char *str, FILE *fp);

/**
 * @brief Lê uma string de um ficheiro binário (tamanho + conteúdo).
 * @param fp Ficheiro já aberto.
 * @return String alocada dinamicamente.
 */
char* lerStringBinario(FILE *fp);

#endif /* INPUT_H */