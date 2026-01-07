

#include <stdio.h>
#include <time.h>
#include "../include/logs.h"
#include "../include/input.h"

/**
* @brief Regista eventos críticos e falhas do sistema num ficheiro de auditoria.
 * @details Conforme exigido pelo enunciado, permite a depuração e o acompanhamento
 * do funcionamento do software através de carimbos temporais. [cite: 180, 181]
 * @param mensagem Descrição do evento (ex: erro de memória ou ação de utilizador).
 */
void registar_log(const char *mensagem) {
    FILE *fp = fopen("log.txt", "a");
    if (fp == NULL) return;

    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);

    fprintf(fp, "[%02d-%02d-%04d %02d:%02d:%02d] %s\n",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec,
            mensagem);

    fclose(fp);
}

/**
 * @brief Exibe o conteúdo do ficheiro de logs na consola.
 * @details Esta funcionalidade adicional permite que o administrador consulte
 * o histórico de ações e erros sem fechar a aplicação.
 */
void consultar_logs() {
    FILE *fp = fopen("log.txt", "r");
    if (fp == NULL) {
        printf("Ainda não existem registos de log.\n");
        return;
    }

    char linha[256];
    printf("\n=== HISTÓRICO DE LOGS ===\n");
    while (fgets(linha, sizeof(linha), fp)) {
        printf("%s", linha);
    }
    fclose(fp);
}