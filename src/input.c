/**
 * @file input.c
 * @brief ficheiro que contém funções que permitem a leitura dos dados do stdin.
 * @author Francisco Alves
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define VALOR_INVALIDO "O valor inserido e invalido."


/**
 * @brief Função que limpa o buffer de entrada de dados.
 * @warning Caso não exista nenhuma informação no buffer, o programa
 * pode ficar a aguardar que eixsta informação no buffer.
 */
static void cleanInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/**
 * @brief Função que solicita ao user um valor inteiro positivo.
 * @param msg A mensagem a mostrar ao user.
 * @return Valor inserido pelo user.
 */
int obterIntPositivo(char *msg) {
    int valor;
    printf("%s", msg);
    while (scanf("%d", &valor) != 1 || valor < 0) {
        puts(VALOR_INVALIDO);
        cleanInputBuffer();
        printf("%s", msg);
    }
    cleanInputBuffer();
    return valor;
}

/**
 * @brief Função que solicita um valor float positivo ao user.
 * @param msg Mensagem a mostrar ao user
 * @return Valor inserido pelo user
 */
float obterFloatPositivo(char *msg) {
    float valor;
    printf("%s", msg);
    while (scanf("%f", &valor) != 1 || valor < 0) {
        puts(VALOR_INVALIDO);
        cleanInputBuffer();
        printf("%s", msg);
    }
    cleanInputBuffer();
    return valor;
}

/**
 * 
 * @param string Apontador para a farse lida.
 * @param tamanho Tamanho máximo da frase a ser lida.
 * @warning O tamanho da frase tem de ter em conta o espaço do caracter de término da string ('\0')
 * @param msg Mensagem a ser mostrada ao user.
 */
void lerString(char *string, unsigned int tamanho, char *msg) {
    printf("%s", msg);
    if (fgets(string, tamanho, stdin) != NULL) {
        unsigned int len = (unsigned int)strlen(string);
        if (len > 0 && string[len - 1] == '\n') {
            string[len - 1] = '\0';
        } else if (len == 0) {
            string[0] = '\0';
        } else {
            cleanInputBuffer();
        }
    } else if (tamanho > 0) {
        string[0] = '\0';
    }
}


/**
 * @brief Lê uma string do teclado com alocação dinâmica de memória.
 * @details Implementa um buffer expansível que cresce conforme a necessidade do utilizador,
 * garantindo que não existam limites artificiais para o tamanho do texto introduzido.
 * Utiliza a técnica de duplicação de capacidade (realloc) para otimizar o desempenho.
 * @param msg Mensagem a exibir ao utilizador antes da leitura.
 * @return Um apontador (char*) para a string lida na Heap. O utilizador deve libertar mais tarde (free) esta memória.
 */
char* lerStringDinamica(const char *msg) {
    printf("%s", msg);

    unsigned int capacidade = 16;
    unsigned int tamanho = 0;
    char *string = malloc(capacidade * sizeof(char));

    if (!string) return NULL;

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (tamanho + 1 >= capacidade) {
            capacidade *= 2;
            char *temp = realloc(string, capacidade * sizeof(char));
            if (!temp) {
                free(string);
                return NULL;
            }
            string = temp;
        }
        string[tamanho++] = (char)c;
    }

    string[tamanho] = '\0';
    return string;
}

/**
 * @brief Lê e valida o nome de uma entidade (Técnico ou Responsável).
 * @details Garante que o nome cumpre três regras de validação: ter no mínimo 3 caracteres,
 * começar com letra maiúscula e não conter algarismos. Repete a leitura até que
 * todas as condições sejam satisfeitas.
 * @param string Apontador para o buffer onde o nome será armazenado.
 * @param tamanho Tamanho máximo do buffer (limite da string).
 * @param msg Mensagem a exibir ao utilizador.
 * @note Esta função cumpre os requisitos de validação de dados e proteção contra
 * entradas inválidas exigidos pelo projeto.
 */
void lerNomeValido(char *string, unsigned int tamanho, char *msg) {
    int valido;
    do {
        valido = 1;
        lerString(string, tamanho, msg);

        if (strlen(string) < 3) {
            printf("O nome deve ter no minimo 3 caracteres.\n");
            valido = 0;
            continue;
        }

        if (!isupper(string[0])) {
            printf("O nome deve comecar com uma letra maiuscula.\n");
            valido = 0;
            continue;
        }

        for (int i = 0; string[i] != '\0'; i++) {
            if (isdigit(string[i])) {
                printf("O nome nao pode conter numeros.\n");
                valido = 0;
                break;
            }
        }
    } while (!valido);
}

/**
 * @brief Interrompe a execução do programa até que o utilizador prima a tecla Enter.
 * @details Esta função é utilizada para pausar a interface após a exibição de mensagens, 
 * listagens ou relatórios, garantindo que o utilizador tenha tempo de ler a informação 
 * antes do menu seguinte ser carregado.
 * @note Limpa o buffer de entrada para evitar que caracteres residuais de leituras 
 * anteriores ignorem a pausa.
 */
void pausar_ecra(void) {
    printf("\n\nPrima Enter para continuar...");
    while (getchar() != '\n');
}

/**
 * @brief Função que pede um valor intervalado ao user.
 * @param minValor Valor int minimo que poderá ser introduzido.
 * @param maxValor Valor int máximo que poderá ser introduzido.
 * @note Os valores máximo e mínimos devem ser introduzidos manualmente pelo programador.
 * A função é normalmente usada para quando existem várias opções num menu, ou a meio de uma função.
 * @param msg Mensagem a mostrar ao user.
 * @return Valor introduzido pelo user.
 */
int obterIntIntervalado(int minValor, int maxValor, char *msg) {
    int valor;
    printf("%s", msg);
    while (scanf("%d", &valor) != 1 || valor < minValor || valor > maxValor) {
        puts(VALOR_INVALIDO);
        cleanInputBuffer();
        printf("%s", msg);
    }
    cleanInputBuffer();
    return valor;
}

/**
 * @brief Grava uma string dinâmicanum ficheiro binário.
 * @param str Apontador para a string a ser guarada, pode ser NULL.
 * @param fp Apontador para o ficheiro binário  aberto em modo de escrita.
 */
void escreverStringBinario(char *str, FILE *fp) {
    int tam = (str != NULL) ? strlen(str) + 1 : 0;
    fwrite(&tam, sizeof(int), 1, fp); // Grava o tamanho
    if (tam > 0) {
        fwrite(str, sizeof(char), tam, fp); // Grava o texto
    }
}

/**
 * @brief Lê a string de um ficheiro binário e aloca a memória necessária.
 * @param fp Apontador para o ficheiro binário aberto em modo de leitura.
 * @return Um apontador para a string alocada ou NULL se a string for vazia ou a alocação falhar.
 */
char* lerStringBinario(FILE *fp) {
    int tam;
    fread(&tam, sizeof(int), 1, fp);
    if (tam == 0) return NULL;

    char *str = malloc(tam * sizeof(char));
    if (str != NULL) {
        fread(str, sizeof(char), tam, fp);
    }
    return str;
}
