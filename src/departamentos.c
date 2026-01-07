/**
 * @file departamentos.c
 * @brief Ficheiro que contém as funções de gestão de departamentos
 * @author Francisco Alves
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/departamentos.h"
#include "../include/input.h"
#include "../include/logs.h"
#include <string.h>

#define CRESCIMENTO_5 5

/**
 * @brief Identifica o maior ID de departamento presente no sistema.
 * @param departamentos Estrutura que contém a lista de departamentos e contador.
 * @return Retorna o valor do maior ID encontrado, ou 0 se não houverem departamentos.
 */
int obterMaiorIDDepartamento(Departamentos departamentos) {
    int maxID = 0;
    for (int i = 0; i < departamentos.contador; i++) {
        if (departamentos.departamento[i].idDepartamento > maxID) {
            maxID = departamentos.departamento[i].idDepartamento;
        }
    }
    return maxID;
}

/**
 * @brief Gera um ID único e sequencial para um novo registo.
 * @details Percorre o array de ordens (ou técnicos/ativos) para encontrar o maior ID 
 * atual e incrementa 1. Garante que, mesmo após carregar dados de um ficheiro, 
 * os IDs continuem a sequência correta.
 * @param departamentos Apontador para a estrutura que contém o array e contador
 * @return Retorna o id do departamento que está a ser criado
 */
static int gerarProximoID(Departamentos *departamentos) {
    if (departamentos->contador == 0) {
        return 10;
    }

    int maxID = 0;
    for (int i = 0; i < departamentos->contador; i++) {
        if (departamentos->departamento[i].idDepartamento > maxID) {
            maxID = departamentos->departamento[i].idDepartamento;
        }
    }

    return maxID + 1;
}

/**
 * @brief Garante que o array de departamentos tem a capacidade suficiente para novos registos.
 * @note se a quantidade atual for insuficiente, usa realloc para expandir a heap.
 * @param departamentos Apontador para a estrutura que contém o array e contador.
 * @param minCap capacidade mínima desejada.
 * @return retorna 1 se a alocação de memória tiver sucesso, caso contrário retorna 0.
 */
static int garantir_capacidade_departamentos(Departamentos *departamentos, int minCap) {
    if (departamentos->capacidade >= minCap) return 1;

    int novaCap = departamentos->capacidade;
    if (novaCap <= 0) novaCap = 0;
    while (novaCap < minCap) novaCap += CRESCIMENTO_5;

    void *tmp = realloc(departamentos->departamento, (size_t)novaCap * sizeof(*departamentos->departamento));
    if (tmp == NULL) {
        printf("Erro: sem memória para alocar departamentos.\n");
        registar_log("Erro: Falha no realloc ao tentar expandir a lista de departamentos.");
        return 0;
    }
    departamentos->departamento = (Departamento *)tmp;
    departamentos->capacidade = novaCap;
    return 1;
}

/**
 * @brief Valida o nome do departamento introduzido pelo usuário.
 * @note O nome é válido caso tenha mais de 3 letras.
 * @param departamentos Estrutura que contém a lista de departamentos
 * @return Retorna 1 caso tenha sucesso, caso contrário retorna 0.
 */
int validar_nome_departamento (Departamentos departamentos) {
    if (departamentos.departamento == NULL) {
        return 0;
    }
    if (departamentos.contador <= 0) {
        return 0;
    }

    int idx = departamentos.contador - 1;
    if (departamentos.departamento[idx].nomeDepartamento == NULL) {
        return 0;
    }

    int len = (int)strlen(departamentos.departamento[idx].nomeDepartamento);
    if (len < 3) {
        printf("O nome do departamento é demasiado curto. Tente novamente");
        registar_log("Erro: Nome de departamento demasiado curto (menos de 3 caracteres).");
        return 0;
    }
    return 1;
}

/**
 * @brief Converte o estado de atividade para texto.
 * @param atividade O estado (enum) a converter.
 * @return String correspondente ao estado.
 */
char *passar_int_string (Atividade atividade) {
    switch (atividade) {
        case ATIVO:
            return "ATIVO";
        case INATIVO:
            return "INATIVO";
        default:
            return "DESCONHECIDO";
    }
}

/**
 * @brief Procura o índice de um departamento através do seu ID.
 * @param departamentos Estrutura que contém a lista de departamentos.
 * @param idPretendido ID que se pretende procurar.
 * @return Retorna o indíce do departamento no array, ou -1 caso não seja encontrado.
 */
int procurarIdDepartamento (Departamentos departamentos, int idPretendido) {
    for (int i = 0; i < departamentos.contador; i++) {
        if (departamentos.departamento[i].idDepartamento == idPretendido) {
            return i;
        }
    }
    return -1;
}

/**
* @brief Solicita e valida um número de telemóvel, armazenando-o como string dinâmica.
* @details Aloca memória na Heap para 9 dígitos e valida se a entrada contém
* exclusivamente algarismos. Inclui limpeza do buffer em caso de erro.
* @return Retorna um apontador (char*) para a string do número ou NULL se a alocação falhar.
* @note A memória retornada deve ser libertada com free() pelo chamador.
* @warning A leitura é limitada a 9 caracteres para evitar overflow do buffer alocado.
*/
char* obterNumeroTelemovel(void) {
    int capacidade = 10;
    char *numero = malloc (capacidade * sizeof(char));

    if (!numero) {
        registar_log("Erro: Falha ao alocar memória para o contacto (obterNumeroTelemovel).");
        return NULL;
    }

    int i, valido, len;

    do {
        valido = 1;
        printf("Indique o contacto (9 digitos): ");
        scanf("%9s", numero);
        len = 0;
        while (numero[len] != '\0') len++;
        if (len != 9) {
            valido = 0;
        } else {
            for (i = 0; i < 9; i++) {
                if (numero[i] < '0' || numero[i] > '9') {
                    valido = 0;
                    break;
                }
            }
        }
        if (!valido) {
            printf("Erro: O contacto deve ter exatamente 9 algarismos.\n");
            registar_log("Erro: Contacto inválido (número deve ter 9 algarismos).");
            while (getchar() != '\n');
        }
    } while (!valido);
    return numero;
}

/**
 * @brief Solicita, valida e armazena dinamicamente um endereço de e-mail.
 * @details Aloca memória na Heap para o e-mail e utiliza um ciclo de validação
 * que verifica a existência de exatamente um '@' e pelo menos um ponto ('.').
 * @param msg Mensagem a exibir ao utilizador para solicitar o email.
 * @return Retorna um apontador (char*) para a string alocada na heap.
 * @note O chamador da função é responsável por libertar a memória com free.
 * @warning Se a alocação da memória falhar retorna NULL.
 */
char *obterEmail(const char *msg) {

    int capacidade = 100;
    char *email = malloc (capacidade * sizeof(char));
    int valido;

    if (!email) {
        registar_log("Erro: Falha ao alocar memória para email (obterEmail).");
        return NULL;
    }


    do {
        int arroba = 0, ponto = 0;
        valido = 1;

        printf("%s", msg);
        scanf("%99s", email);
        int len = (int) strlen(email);

        for (int i = 0; i < len; i++) {
            if (email[i] == '@') arroba++;
            if (email[i] == '.') ponto++;
        }

        if (arroba != 1 || ponto < 1) {
            printf("Erro: Formato de e-mail invalido! Tente novamente.\n");
            registar_log("Erro: Formato de email inválido.");
            valido = 0;
        }
    } while (!valido);

    printf("Email associado com sucesso.\n");
    return email;
}

/**
 * @brief Solicita dados ao user e cria um novo departamento.
 * @details Aloca memória dinâmica para o nome, responsável e contacto usando lerStringDinamica.
 * Incrementa os contadores de departamentos totais e ativos.
 * @param departamentos Apontador para a lista onde o departamento está inserido.
 */
void criarDepartamento (Departamentos *departamentos) {
    if (departamentos == NULL) return;

    if (!garantir_capacidade_departamentos(departamentos, departamentos->contador + 1)) {
        pausar_ecra();
        return;
    }

    int escolha;
    int validar;

    int idx = departamentos->contador;
    departamentos->contador++;

    do {
        departamentos->departamento[idx].nomeDepartamento = lerStringDinamica("Indique o nome do departamento:\n");
        validar = validar_nome_departamento(*departamentos);
    } while (validar == 0);

    departamentos->departamento[idx].responsavel = lerStringDinamica("Indique o nome do responsável do departamento: \n");
    escolha = obterIntIntervalado(1,2,"Deseja colocar como contacto um número de telemóvel ou um email? (1) Número de telemóvel (2) Email\n");
    switch (escolha) {
        case 1:
            departamentos->departamento[idx].contacto = obterNumeroTelemovel();
            if (departamentos->departamento[idx].contacto == NULL) {
                registar_log("Erro: Não foi possível obter contacto (malloc falhou) ao criar departamento.");
            }
            pausar_ecra();
            break;
        case 2:
            departamentos->departamento[idx].contacto = obterEmail("Indique o email que deseja associar ao departamento\n");
            if (departamentos->departamento[idx].contacto == NULL) {
                registar_log("Erro: Não foi possível obter email (malloc falhou) ao criar departamento.");
            }
            pausar_ecra();
            break;
        default:
            printf("Opção inválida.\n");
            registar_log("Aviso: Opção inválida ao escolher tipo de contacto do departamento.");
            pausar_ecra();
            break;
    }

    departamentos->departamento[idx].atividade = ATIVO;
    departamentos->departamento[idx].idDepartamento = gerarProximoID(departamentos);
    departamentos->departamentosAtivos++;

    registar_log("Info: Foi criado um novo departamento.");
}

/**
 * @brief Função que imprime no ecrã informações de todos os departamentos disponíveis.
 * @detais Percorre o array de departamentos e imprime detalhes de cada um. Caso não hajam registos
 * apresenta uma mensagem informativa.
 * @param departamentos Estrutura que contém a lista de departamentos.
 * @note Utiliza a função passar_int_string() para exibir o estado de forma legível.
 */
void listar_departamentos (Departamentos departamentos) {
    puts("\n===== DEPARTAMENTOS =====");
    if (departamentos.contador <=0) {
        printf("Não existem departamentos registados.\n");
        pausar_ecra();
        return;
    }

    if (departamentos.contador>0) {
        for (int i= 0; i<departamentos.contador;i++) {
            printf("\nNome do departamento: %s\n", departamentos.departamento[i].nomeDepartamento);
            printf("ID: %d\n",departamentos.departamento[i].idDepartamento);
            printf("Responsável do departamento: %s\n", departamentos.departamento[i].responsavel);
            printf("Contacto: %s\n", departamentos.departamento[i].contacto);
            printf("Estado: %s\n", passar_int_string(departamentos.departamento[i].atividade));
        }
        pausar_ecra();
    }
}

/**
 * @brief Função que permite mudar as informações do departamento introduzidas pelo user.
 * @details A função usa procura o ID do departamento e oferece opções para alterar o
 * nome do departamento, nome do responsável do departamento ou o contacto associado ao departamento.
 * @param departamentos Apontador para a estrutura que contém a lista de departamentos.
 * @note Ao atualizar campos de texto, a função utiliza lerStringDinamica, obterEmail ou
 * obterNumeroTelemovel, que alocam nova memória na Heap.
 * @warning Ao atualizar os campos deve se certificar que se liberta a
 * memoria anterior da heap para evitar erros de memory leak.
 */
void atualizar_departamento(Departamentos *departamentos) {
    puts ("\n===== ATUALIZAR DEPARTAMENTO =====");
    int idPretendido;
    int idProcurado;
    int escolha;
    int validar;
    int maxIdDepartamento;
    maxIdDepartamento = obterMaiorIDDepartamento(*departamentos);
    if (departamentos->departamentosAtivos == 0) {
        printf ("Não existem departamentos ativos.\n");
        pausar_ecra();
        return;
    }
    do {
        idPretendido = obterIntIntervalado(0,maxIdDepartamento,"Indique o id do departamento que deseja editar: \n");
        idProcurado = procurarIdDepartamento(*departamentos,idPretendido);
        if (idProcurado == -1) {
            printf("Não existe nenhuma ocorrência com o id que introduziu. Tente novamente");
            registar_log("Aviso: Tentativa de atualizar departamento com ID inexistente.");
        }
    }while (idProcurado == -1);


    escolha = obterIntIntervalado(0,3, "Introduza qual campo deseja editar:\n1 - Nome do departamento\n2 - Responsavel do departamento\n3 - Contacto do departamento");
    int escolha1;
    switch (escolha) {
        case 1:
            do {
                free(departamentos->departamento[idProcurado].nomeDepartamento);
                departamentos->departamento[idProcurado].nomeDepartamento = lerStringDinamica("Indique o novo nome do departamento:\n");
                validar = validar_nome_departamento(*departamentos);
            }while (validar == 0);
            pausar_ecra();
            break;
        case 2:
            free(departamentos->departamento[idProcurado].responsavel);
            departamentos->departamento[idProcurado].responsavel = lerStringDinamica("Indique o nome do novo responsavel do departamento:\n");
            pausar_ecra();
            break;
        case 3:
            free(departamentos->departamento[idProcurado].contacto);
            escolha1 = obterIntIntervalado(1,2,"Deseja colocar como contacto um n\\u00famero de telem\\u00f3vel ou um email? (1) N\\u00famero de telem\\u00f3vel (2) Email\\n");
            switch (escolha1) {
                case 1:
                    departamentos->departamento[idProcurado].contacto = obterNumeroTelemovel();
                    pausar_ecra();
                    break;
                case 2:
                    departamentos->departamento[idProcurado].contacto = obterEmail("Indique o email que deseja associar ao departamento\\n");
                    pausar_ecra();
                    break;
                default:
                    printf("Opção inválida.\\n");
                    registar_log("Aviso: Opção inválida ao alterar contacto do departamento.");
                    pausar_ecra();
                    break;
            }
            break;
        default:
            printf("Opção inválida.\\n");
            registar_log("Aviso: Opção inválida ao escolher campo para atualizar departamento.");
            pausar_ecra();
            break;
    }
}

/**
 * @brief Função para inativar departamentos.
 * @details A função muda o estado do departamento para inativo para que ele não seja eliminado permanentemente
 * do sistema, permitindo que ele permaneça no histórico do programa. A função utiliza validações para
 * validar o ID do departamento, e para verificar que o mesmo já não está inativo.
 * @param departamentos Apontador para a estrutura que contém a lista de departamentos.
 * @note  A função decrementa o contador ' departamentosAtivos ' se for bem sucedida. 
 */
void inativar_Departamento (Departamentos *departamentos) {
    puts("\n===== INATIVAR DEPARTAMENTO =====");

    if (departamentos == NULL || departamentos->departamento == NULL || departamentos->contador <= 0) {
        printf ("Não existem departamentos registados.\n");
        registar_log("Aviso: Tentativa de inativar departamento sem existirem departamentos registados.");
        pausar_ecra();
        return;
    }

    if (departamentos->departamentosAtivos == 0) {
        printf ("Não existem departamentos ativos.\n");
        registar_log("Aviso: Tentativa de inativar departamento sem existirem departamentos ativos.");
        pausar_ecra();
        return;
    }

    int idPretendido;
    int idProcurado;
    int maxIdDepartamentos;
    maxIdDepartamentos = obterMaiorIDDepartamento(*departamentos);

    do {
        idPretendido = obterIntIntervalado(0,maxIdDepartamentos,"Indique o id do departamento que deseja inativar: \n");
        idProcurado = procurarIdDepartamento(*departamentos,idPretendido);
        if (idProcurado == -1) {
            printf("Não existe nenhuma ocorrência com o id que introduziu. Tente novamente\n");
            registar_log("Aviso: Tentativa de inativar departamento com ID inexistente.");
        }
    } while (idProcurado == -1);

    if (departamentos->departamento[idProcurado].atividade == INATIVO) {
        printf("O departamento selecionado já se encontra inativo.\n");
        registar_log("Aviso: Tentativa de inativar um departamento que já está inativo.");
        pausar_ecra();
        return;
    }

    departamentos->departamento[idProcurado].atividade = INATIVO;
    if (departamentos->departamentosAtivos > 0) {
        departamentos->departamentosAtivos--;
    }

    puts("O departamento foi inativado com sucesso.");
    registar_log("Info: Um departamento foi inativado.");
    pausar_ecra();
}

/**
 * @brief Guarda todos os dados dos departamentos num ficheiro binário.
 * @details Serializa a estrutura para o disco, gravando primeiro o contador total e,
 * em seguida, os dados de cada departamento. Utiliza escreverStringBinario() para
 * lidar com os campos de texto dinâmicos.
 * @param departamentos Apontador para a estrutura que contém os dados a persistir.
 * @note Implementa um formato de armazenamento híbrido: utiliza campos de tamanho fixo para metadados
 * (ID e estado) e delega a escrita de campos de tamanho variável à função auxiliar escreverStringBinario().
 */
void guardarDepartamentos (Departamentos *departamentos) {
    FILE *fp = fopen("departamentos.bin", "wb");
    if  (fp == NULL) {
        printf("Erro ao criar o ficheiro de departamentos!\n");
        registar_log("Erro: Não foi possível abrir departamentos.bin para escrita.");
        return;
    }
    fwrite(&departamentos->contador, sizeof(int), 1, fp);

    for (int i = 0; i < departamentos->contador; i++) {
        fwrite(&departamentos->departamento[i].idDepartamento, sizeof(int), 1, fp);
        fwrite(&departamentos->departamento[i].atividade, sizeof(int), 1, fp);

        escreverStringBinario(departamentos->departamento[i].nomeDepartamento, fp);
        escreverStringBinario(departamentos->departamento[i].responsavel, fp);
        escreverStringBinario(departamentos->departamento[i].contacto, fp);
    }
}

/**
 * @brief Recupera os dados dos departamentos a partir de um ficheiro binário.
 * @details Lê o contador de registos e aloca, ajusta a capacidade e aloca o array principal na heap (para evitar memory leaks),
 * reconstroi as strings dinâmicas (nome, responsável, contacto) utilizando a função lerStringBinário().
 * @param departamentos Apontador para a estrutura onde os dados lidos serão carregados.
 * @note Se o ficheiro não existir (Por exemplo numa primeira utilização do programa), a função termina silenciosamente.
 * @warning A função usa várias alocações de memória dinâmica (malloc), pelo que é essencial que a memória seja libertada no final.
 */
void carregarDepartamentos(Departamentos *departamentos) {
    FILE *fp = fopen("departamentos.bin", "rb");
    if (fp == NULL) return;

    fread(&departamentos->contador, sizeof(int), 1, fp);
    departamentos->capacidade = departamentos->contador;

    departamentos->departamento = malloc(departamentos->capacidade * sizeof(Departamento));
    if (departamentos->departamento == NULL && departamentos->contador > 0) {
        registar_log("Erro: Falha ao alocar memória ao carregar departamentos (malloc devolveu NULL).");
        fclose(fp);
        return;
    }

    for (int i = 0; i < departamentos->contador; i++) {
        fread(&departamentos->departamento[i].idDepartamento, sizeof(int), 1, fp);
        fread(&departamentos->departamento[i].atividade, sizeof(int), 1, fp);
        departamentos->departamento[i].nomeDepartamento = lerStringBinario(fp);
        departamentos->departamento[i].responsavel = lerStringBinario(fp);
        departamentos->departamento[i].contacto = lerStringBinario(fp);
    }
    fclose(fp);
}