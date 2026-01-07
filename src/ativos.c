/**
 * @file ativos.c
 * @brief Ficheiro que contém funções de gestão de ativos.
 * @author Francisco Alves
 */

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/departamentos.h"
#include "../include/ativos.h"
#include "../include/input.h"
#include "../include/logs.h"

#define CRESCIMENTO_5 5

/**
 * @brief Função para obter o maior ID da lista de ativos.
 * @param ativos Estrutura que contém a lista de ativos e o contador.
 * @return Retorna o ID mais alto da lista de ativos.
 */
int obterMaiorIDAtivos(Ativos ativos) {
    int maxID = 0;
    for (int i = 0; i < ativos.contador; i++) {
        if (ativos.ativo[i].id > maxID) {
            maxID = ativos.ativo[i].id;
        }
    }
    return maxID;
}


/**
 * @brief Gera um IDúnico e sequencial para um novo ativo.
 * @details Percorre o array de ativos para encontrar o ID mais alto e adiciona uma unidade,
 * este método garante que será sempre criado um ID novo mesmo que o programa seja reiniciado ou sejam removidos registos.
 * @param ativos Apontador para a estrutura que contém a lista de ativos e contador.
 * @return Retorna o um valor inteiro maior que o ID do ativo mais recente da lista,
 * ou retorna 10 caso o contador ainda esteja a 0.
 */
static int gerarProximoID(Ativos *ativos) {
    if (ativos->contador == 0) {
        return 10;
    }

    int maxID = 0;
    for (int i = 0; i < ativos->contador; i++) {
        if (ativos->ativo[i].id > maxID) {
            maxID = ativos->ativo[i].id;
        }
    }

    return maxID + 1;
}

/**
 * @brief Expande a capacidade do array de ativos de forma dinâmica.
 * @details Verifica se a capacidade de ativos é suficiente para o valor de minCap. Caso contrário, usa realloc para
 * redimensionar o bloco de memória na heap.
 * @param ativos Apontador para a estrutura que contém a lista de ativos e a capacidade.
 * @param minCap Valor inteiro que representa a capacidade mínima necessária.
 * @return 1 se a expansão for bem sucedida ou 0 caso haja um erro a alocar memória.
 */
static int garantir_capacidade_ativos(Ativos *ativos, int minCap) {
    if (ativos->capacidade >= minCap) return 1;

    int novaCap = ativos->capacidade;
    if (novaCap <= 0) novaCap = 0;
    while (novaCap < minCap) novaCap += CRESCIMENTO_5;

    void *tmp = realloc(ativos->ativo, (size_t)novaCap * sizeof(*ativos->ativo));
    if (tmp == NULL) {
        printf("Erro: sem memória para alocar ativos.\n");
        registar_log("Erro: Falha no realloc ao tentar expandir a lista de ativos.");
        return 0;
    }

    ativos->ativo = (Ativo *)tmp;
    ativos->capacidade = novaCap;
    return 1;
}

/**
 * @brief Converte o valor enumerado da categoria de ativo para uma string descritiva.
 * @details A função é utilizada para mapear a CategoriaAtivo para texto,
 * permitindo que as listagens e relatórios exibam texto em vez do número correspondente á categoria do ativo.
 * @param categoria_ativo O valor de enum que representa a categoria.
 * @return Um apontador para uma string constante com o nome da categoria.
 */
static const char *passar_int_string_categoria_idx(CategoriaAtivo categoria_ativo) {
    switch (categoria_ativo) {
        case VIATURA: return "Viatura";
        case INFORMATICA: return "Informática";
        case MOBILIARIO: return "Mobiliário";
        case FERRAMENTA: return "Ferramenta";
        case OUTRO: return "Outro";
        default: return "Desconhecido";
    }
}

/**
 * @brief Converte o valor enumerado do estado do ativo para uma string descritiva.
 * @details A função é utilizada para mapear o EstadoAtivo para texto,
 * permitindo que as listagens e relatórios exibam texto em vez do número correspondente ao estado do ativo.
 * @param estado_ativo O valor de enum que representa o estado do ativo.
 * @return Um apontador para uma string constante com o nome da  categoria.
 */
static const char *passar_int_string_estado_idx(EstadoAtivo estado_ativo) {
    switch (estado_ativo) {
        case OPERACIONAL: return "Operacional";
        case EM_MANUTENCAO: return "Em manutenção";
        case ABATIDO: return "Abatido";
        case INATIVO2: return "Inativo";
        default: return "Desconhecido";
    }
}

/**
 * @brief Função que verifique se o departamento existe e está ativo.
 * @param departamentos Apontador para estrutura que contém a lista de departamentos.
 * @param idAssociado ID que o user quer validar.
 * @return 1 caso seja válido, caso contrário 0.
 */
int validar_departamento_associado (Departamentos *departamentos,int idAssociado) {
    for (int i =0; i < departamentos->contador; i++) {
        if (departamentos->departamento[i].idDepartamento == idAssociado && departamentos->departamento[i].atividade == ATIVO) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Cria um novo ativo no sistema associando-o a um novo departamento.
 * @param ativos Apontador para a estrutura que contém a lista de ativos e contador.
 * @param departamentos Apontador para a estrutura que contém a lista de departamentos, contador e departamentos ativos.
 * @note Usa funções auxiliares como garantir_capacidade_ativos(), obterIntIntervalado()
 * e validar_departamento_associado() para assegurar a integridade dos dados.
 */
void criar_ativo (Ativos *ativos, Departamentos *departamentos) {
    int validar;
    int idAssociado;
    int maxIdDepartamentos;
    maxIdDepartamentos = obterMaiorIDDepartamento(*departamentos);

    if (ativos == NULL || departamentos == NULL) return;

    if (departamentos->contador <= 0 || departamentos->departamento == NULL || departamentos->departamentosAtivos <= 0) {
        printf("Não é possível criar um ativo sem existir pelo menos um departamento ativo.\n");
        printf("Crie primeiro um departamento e deixe-o como ATIVO.\n");
        registar_log("Erro: Tentativa de criar ativo sem existir pelo menos um departamento ativo.");
        pausar_ecra();
        return;
    }

    if (!garantir_capacidade_ativos(ativos, ativos->contador + 1)) {
        pausar_ecra();
        return;
    }

    int idx = ativos->contador;

    printf ("\n===== CRIAR ATIVO ===== \n");
    ativos->ativo[idx].designacao = lerStringDinamica("Indique a designação do ativo:\n");
    ativos->ativo[idx].categoria = obterIntIntervalado(1,5,"Indique a categoria do ativo:\n1 - Viatura\n2 - Informática\n3 - Mobiliário\n4 - Ferramenta\n5 - Outro\n");
    ativos->ativo[idx].custo = obterFloatPositivo("Indique o valor em euros do ativo.");

    do {
        idAssociado = obterIntIntervalado(0, maxIdDepartamentos, "Indique o ID do departamento ao qual este ativo estará associado.\n");
        validar = validar_departamento_associado(departamentos, idAssociado);
        if (validar == 0) {
            printf ("ID inválido, tente novamente.\n");
        }
    } while (validar == 0);

    ativos->ativo[idx].idDepartamentoAssociado = idAssociado;
    ativos->ativo[idx].localizacao = lerStringDinamica("Indique a localização do ativo:\n");

    time_t agora = time(NULL);
    struct tm *tmLocal = localtime(&agora);
    if (tmLocal != NULL) {
        ativos->ativo[idx].diaAquisicao = tmLocal->tm_mday;
        ativos->ativo[idx].mesAquisicao = tmLocal->tm_mon + 1;
        ativos->ativo[idx].anoAquisicao = tmLocal->tm_year + 1900;
    } else {
        ativos->ativo[idx].diaAquisicao = 1;
        ativos->ativo[idx].mesAquisicao = 1;
        ativos->ativo[idx].anoAquisicao = 1970;
    }

    ativos->ativo[idx].estado = OPERACIONAL;
    ativos->ativo[idx].id = gerarProximoID(ativos);

    ativos->contador++;
    ativos->ativosDisponiveis++;

    registar_log("Info: Foi criado um novo ativo.");

    pausar_ecra();
}

/**
 * @brief Função que procura p índice do ativo através do seu ID e estado.
 * @details Percorre o array dos ativos até encontrar um ID correspondente, garantindo que ele está
 * no sistema (estado diferente de ABATIDO).
 * @param ativos Apontador para a estrutura que contém a lista de ativos e contador.
 * @param idProcurado ID que vai ser procurado.
 * @return Retorna o índice associado ao ativo com o mesmo ID no array ou -1 caso não encontre.
 */
int procurar_ativo_id (Ativos *ativos, int idProcurado) {
    for (int i = 0; i<ativos->contador; i++) {
        if (ativos->ativo[i].id == idProcurado && ativos->ativo[i].estado != ABATIDO) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Função que mostra no ecrã as informações de todos os ativos.
 * @details Percorre o array de ativos e apresenta informações como o ID, designação,
 * categoria e data de aquisição e ainda data de abate(caso o estado do ativo esteja definido como Abatido).
 * @param ativos Estrutura que contém a lista de ativos e contador.
 * @note Usa funções auxiliares para converter enums para textos legiveis para o user.
 */
void listar_ativos(Ativos ativos) {
    printf ("\n ===== LISTAR ATIVOS =====\n");
    if (ativos.contador == 0) {
        printf ("Não existem Ativos disponiveis.\n");
        pausar_ecra();
        return;
    }
    if (ativos.ativo == NULL) {
        printf("Erro interno: lista de ativos não inicializada.\n");
        registar_log("Erro: Lista de ativos não inicializada (ativos.ativo == NULL) ao tentar listar.");
        pausar_ecra();
        return;
    }
    for (int i = 0; i< ativos.contador; i++) {
        printf ("ID: %d\n", ativos.ativo[i].id);
        printf ("Designação: %s\n", ativos.ativo[i].designacao ? ativos.ativo[i].designacao : "(sem designação)");
        printf ("Categoria: %s\n", passar_int_string_categoria_idx(ativos.ativo[i].categoria));
        printf("Estado do ativo: %s\n", passar_int_string_estado_idx(ativos.ativo[i].estado));
        printf ("Data de aquisição: %d/%d/%d\n", ativos.ativo[i].diaAquisicao, ativos.ativo[i].mesAquisicao, ativos.ativo[i].anoAquisicao);
        if (ativos.ativo[i].estado == ABATIDO) {
            printf("Data de abate: %d/%d/%d\n", ativos.ativo[i].diaAbate, ativos.ativo[i].mesAbate, ativos.ativo[i].anoAbate);
        }
    }
    pausar_ecra();
}

/**
 * @brief Lista todos os ativos organizados por departamento.
 * @param departamentos Estrutura que contém a lista de departamentos.
 * @param ativos Estrutura que contém a lista de ativos.
 */
void listar_ativos_por_departamento (Departamentos departamentos, Ativos ativos) {
    if (departamentos.departamento == NULL || ativos.ativo == NULL) return;
    for (int i = 0; i< departamentos.contador; i++) {
        printf ("===== %s =====\n", departamentos.departamento[i].nomeDepartamento ? departamentos.departamento[i].nomeDepartamento : "(sem nome)");
        for (int j=0; j < ativos.contador; j++) {
            if (ativos.ativo[j].idDepartamentoAssociado == departamentos.departamento[i].idDepartamento) {
                printf ("ID: %d\n", ativos.ativo[j].id);
                printf ("Designação: %s\n", ativos.ativo[j].designacao ? ativos.ativo[j].designacao : "(sem designação)");
                printf ("Categoria: %s\n", passar_int_string_categoria_idx(ativos.ativo[j].categoria));
                printf("Estado do ativo: %s\n", passar_int_string_estado_idx(ativos.ativo[j].estado));
                printf ("Data de aquisição: %d/%d/%d\n", ativos.ativo[j].diaAquisicao, ativos.ativo[j].mesAquisicao, ativos.ativo[j].anoAquisicao);
            }
        }
    }
}

/**
 * @brief Função para abater os ativos, registando também a data atual.
 * @details Altera o estado de ativo para ABATIDO e utiliza a biblioteca time.h
 * para capturar automaticamente o momento da operação. Impede que o faça se
 * os ativos ja estiverem abatidos ou se estiverem a meio de uma manutenção.
 * @param ativos Apontador para a estrutura que contém a lista de ativos.
 * @note o Ativo permanece no programa, só deixará de ser válido em funções como procurar_ativo_id().
 */
void abater_ativo (Ativos *ativos) {
    int idProcurado, idEncontrado;

    time_t agora = time(NULL);
    struct tm *tmLocal = localtime(&agora);

    idProcurado = obterIntPositivo("Indique o id do ativo que deseja abater");
    idEncontrado = procurar_ativo_id(ativos, idProcurado);

    if (ativos->ativo[idEncontrado].estado == EM_MANUTENCAO || ativos->ativo[idEncontrado].estado == ABATIDO) {
        printf("ID inválido tente novamente.\n");
        return;
    }

    ativos->ativo[idEncontrado].estado = ABATIDO;
    ativos->ativo[idEncontrado].diaAbate = tmLocal->tm_mday;
    ativos->ativo[idEncontrado].mesAbate = tmLocal->tm_mon + 1;
    ativos->ativo[idEncontrado].anoAbate = tmLocal->tm_year + 1900;

    printf("O ativo foi abatido com sucesso.");
    registar_log("Info: Um ativo foi abatido com sucesso.");
}

/**
 * @brief Guarda a base de dados de ativos num ficheiro binário.
 * @param ativos Apontador para a estrutura que contém as informações que serão escritas no ficheiro.
 * @note O ficheiro é sobrescrito a cada gravação para manter os dados atuais.
 */
void guardarAtivos(Ativos *ativos) {
    FILE *fp = fopen("ativos.bin", "wb");
    if (fp == NULL) {
        printf("Erro ao abrir ficheiro de ativos para escrita!\n");
        registar_log("Erro: Não foi possivel abrir ativos.bin para escrita.");
        return;
    }
    fwrite(&ativos->contador, sizeof(int), 1, fp);
    fwrite(&ativos->ativosDisponiveis, sizeof(int), 1, fp);

    for (int i = 0; i < ativos->contador; i++) {
        fwrite(&ativos->ativo[i].id, sizeof(int), 1, fp);
        fwrite(&ativos->ativo[i].categoria, sizeof(CategoriaAtivo), 1, fp);
        fwrite(&ativos->ativo[i].estado, sizeof(EstadoAtivo), 1, fp);
        fwrite(&ativos->ativo[i].custo, sizeof(float), 1, fp);
        fwrite(&ativos->ativo[i].custoTotalAcumulado, sizeof(float), 1, fp);
        fwrite(&ativos->ativo[i].idDepartamentoAssociado, sizeof(int), 1, fp);
        fwrite(&ativos->ativo[i].diaAquisicao, sizeof(int), 1, fp);
        fwrite(&ativos->ativo[i].mesAquisicao, sizeof(int), 1, fp);
        fwrite(&ativos->ativo[i].anoAquisicao, sizeof(int), 1, fp);
        escreverStringBinario(ativos->ativo[i].designacao, fp);
        escreverStringBinario(ativos->ativo[i].localizacao, fp);
    }

    fclose(fp);
}

/**
 * @brief Carrega a base de dados de ativos através de um ficheiro binário.
 * @param ativos Apontador para a estrutura onde os dados lidos serão armazenados.
 * @note se o ficheiro não existir a função é ignorada.
 * @warning A função realiza alocações dinâmicas pelo que deve ser garantida a libertação posterior de memória
 * para evitar memory leaks.
 */
void carregarAtivos(Ativos *ativos) {
    FILE *fp = fopen("ativos.bin", "rb");
    if (fp == NULL) return;

    fread(&ativos->contador, sizeof(int), 1, fp);
    fread(&ativos->ativosDisponiveis, sizeof(int), 1, fp);

    ativos->capacidade = ativos->contador;
    ativos->ativo = malloc(ativos->capacidade * sizeof(Ativo));

    if (ativos->ativo == NULL && ativos->contador > 0) {
        registar_log("Erro: Falha ao alocar memória ao carregar ativos (malloc devolveu NULL).");
        fclose(fp);
        return;
    }

    for (int i = 0; i < ativos->contador; i++) {
        fread(&ativos->ativo[i].id, sizeof(int), 1, fp);
        fread(&ativos->ativo[i].categoria, sizeof(CategoriaAtivo), 1, fp);
        fread(&ativos->ativo[i].estado, sizeof(EstadoAtivo), 1, fp);
        fread(&ativos->ativo[i].custo, sizeof(float), 1, fp);
        fread(&ativos->ativo[i].custoTotalAcumulado, sizeof(float), 1, fp);
        fread(&ativos->ativo[i].idDepartamentoAssociado, sizeof(int), 1, fp);
        fread(&ativos->ativo[i].diaAquisicao, sizeof(int), 1, fp);
        fread(&ativos->ativo[i].mesAquisicao, sizeof(int), 1, fp);
        fread(&ativos->ativo[i].anoAquisicao, sizeof(int), 1, fp);
        ativos->ativo[i].designacao = lerStringBinario(fp);
        ativos->ativo[i].localizacao = lerStringBinario(fp);
    }

    fclose(fp);
}

/**
 * @brief Função de pesquisa inteligente de ativos.
 * @details Permite ao usuário pesquisar ativos pela designação, mostrando resultados que começam com o termo pesquisado.
 * A pesquisa não é sensível a maiúsculas ou minúsculas.
 * @param ativos Estrutura que contém a lista de ativos.
 * @param termo String com o termo a pesquisar.
 */
void pesquisaInteligenteAtivos(Ativos ativos, const char *termo) {
    int encontrados = 0;

    if (termo == NULL || termo[0] == '\0') {
        printf("Termo de pesquisa inválido.\n");
        return;
    }

    if (ativos.contador == 0) {
        printf("Não existem ativos para pesquisar.\n");
        return;
    }

    if (ativos.ativo == NULL) {
        printf("Erro interno: lista de ativos não inicializada.\n");
        registar_log("Erro: Lista de ativos não inicializada (ativos.ativo == NULL) ao tentar pesquisar.");
        return;
    }

    int tam = (int)strlen(termo);

    printf("\n--- RESULTADOS PARA '%s' ---\n", termo);

    for (int i = 0; i < ativos.contador; i++) {
        if (ativos.ativo[i].designacao == NULL) {
            continue;
        }

        if (strncasecmp(ativos.ativo[i].designacao, termo, (size_t)tam) == 0) {
            printf("ID %d: %s\n", ativos.ativo[i].id, ativos.ativo[i].designacao);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum ativo começa por '%s'.\n", termo);
    }
}

void listarComPesquisaInteligente (Ativos ativos) {
    char *termo;

    if (ativos.contador == 0) {
        printf("Não existem ativos para pesquisar.\n");
        pausar_ecra();
        return;
    }

    termo = lerStringDinamica("Indique o termo que deseja pesquisar (início da designação):\n");
    if (termo == NULL) {
        printf("Erro ao ler termo de pesquisa.\n");
        registar_log("Erro: Falha ao ler termo de pesquisa inteligente de ativos (lerStringDinamica devolveu NULL).");
        pausar_ecra();
        return;
    }

    if (strlen(termo) == 0) {
        printf("Termo inválido.\n");
        free(termo);
        return;
    }

    pesquisaInteligenteAtivos(ativos, termo);

    free(termo);
}
