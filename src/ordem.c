/**
 * @file ordem.c
 * @brief Ficheiro com funções que permitem a gestão das ordens/manutenções.
 * @author Francisco Alves
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/ordem.h"
#include "../include/ativos.h"
#include "../include/departamentos.h"
#include "../include/input.h"
#include "../include/tecnicos.h"
#include "../include/materiais.h"
#include "../include/logs.h"

#define CRESCIMENTO_5 5


/**
 * @brief Função que procura o maior ID registado nas ordens.
 * @param ordens Estrutura com o array de ordens e contador.
 * @return Retorna o maior ID registado. Caso não existam ordens devolve 0.
 */
int obterMaiorIDOrdens(Ordens ordens) {
    int maxID = 0;
    for (int i = 0; i < ordens.contador; i++) {
        if (ordens.ordem[i].idOrdem > maxID) {
            maxID = ordens.ordem[i].idOrdem;
        }
    }
    return maxID;
}


/**
 * @brief Função que conta o número de ordens em execução associadas ao tecnico que analisa.
 * @param tecnico Estrutura do tipo Tecnico que contem os dados do tecnico a analisar
 * @param ordens Estrutura que contem o array e o contador total de ordens.
 * @return Retorna o numero total de ordens em execução associadas ao tecnico.
 */
int numeroManutencoesTecnico (Tecnico tecnico, Ordens ordens) {
    int numero = 0;
    for (int i = 0; i<ordens.contador; i++) {
        if (ordens.ordem[i].estado == EXECUCAO) {
            if (ordens.ordem[i].idTecnico == tecnico.idTecnico) {
                numero++;
            }
        }
    }
    return numero;
}

static const char *passarIntStringPrioridade (Prioridade prioridade) {
    switch (prioridade) {
        case BAIXA:
            return "Baixa";
        case MEDIA:
            return "Media";
        case ALTA:
            return "Alta";
        default:
            return "Desconhecido";
    }
}

float calcularCustos (Ordem *ordem, Materiais *materiais) {
    float total = 0;

    if (ordem == NULL || materiais == NULL) {
        return 0;
    }

    if (materiais->contador == 0) {
        return 0;
    }

    for (int i = 0; i < materiais->contador; i++) {
        if (materiais->material[i].OrdemAssociada == ordem->idOrdem) {
            total += materiais->material[i].custoUnitário * (float)materiais->material[i].quantidade;
        }
    }

    return total;
}


/**
 * @brief Função que gera o próximo ID a ser atribuido a uma nova ordem.
 * @details Percorre todas as ordens existentes e devolve o maior ID + 1. Se não existirem ordens,
 * devolve 10.
 * @param ordens Apontador para a estrutura Ordens.
 * @return Retorna o próximo ID disponível.
 */
static int gerarProximoID(Ordens *ordens) {
    if (ordens->contador == 0) {
        return 10;
    }

    int maxID = 0;
    for (int i = 0; i < ordens->contador; i++) {
        if (ordens->ordem[i].idOrdem > maxID) {
            maxID = ordens->ordem[i].idOrdem;
        }
    }

    return maxID + 1;
}

/**
 * @brief Função auxiliar para garantir que existe memória suficiente na heap para as ordens.
 * @note Caso a capacidade atual seja insuficiente, a função utiliza realloc para aumentar o array
 * em blocos de 5 (CRESCIMENTO_5).
 * @param ordens Apontador para estrutura que contém o array de ordens.
 * @param minCap Capacidade mínima pretendida.
 * @return Retorna 1 caso a alocação seja feita com sucesso, caso contrário retorna 0.
 * @warning Em caso de falha de memória, a função imprime uma mensagem e não faz alterações.
 */
static int garantir_capacidade_ordens(Ordens *ordens, int minCap) {
    if (ordens->capacidade >= minCap) return 1;

    int novaCap = ordens->capacidade;
    if (novaCap <= 0) novaCap = 0;
    while (novaCap < minCap) novaCap += CRESCIMENTO_5;

    void *tmp = realloc(ordens->ordem, (size_t)novaCap * sizeof(*ordens->ordem));
    if (tmp == NULL) {
        printf("Erro: sem memória para alocar ordens.\n");
        registar_log("Erro: Falha no realloc ao tentar expandir a lista de ordens.");
        return 0;
    }

    ordens->ordem = (Ordem *)tmp;
    ordens->capacidade = novaCap;
    return 1;
}

/**
 * @brief Função que mostra a taxa de ocupaão de um determinado tecnico.
 * @param tecnico Apontador para a estrutura que contèm o array com as informações a analisar.
 * @param ordens Apontador para a estrutura que contèm a lista de ordens e contador.
 * @return Numero inteiro que resulta da divisão das ordens ativas de um determinado tecnico pelo
 * limite maximo, multiplicado por 100.
 */
int mostrarTaxaOcupacaoTecnico (Tecnico *tecnico, Ordens *ordens) {
    int taxa = 0;
    int limite_maximo = 5;
    int ordens_ativas = 0;

    ordens_ativas = numeroManutencoesTecnico(*tecnico, *ordens);

    taxa = (ordens_ativas / limite_maximo) * 100;
    return taxa;
}

/**
 * @brief Função que lista apenas os técnicos ativos.
 * @param tecnicos Estrutura com o array de técnicos e contador.
 * @note Apenas são listados os técnicos com estado ATIVO1.
 */
void listar_tecnicos_ativos (Tecnicos *tecnicos, Ordens *ordens) {
    printf ("\n===== TECNICOS ATIVOS =====\n");
    for (int i=0; i < tecnicos->contador; i++) {
        if (tecnicos->tecnico[i].estado_tecnico == ATIVO1)
            printf ("ID: %d", tecnicos->tecnico[i].idTecnico);
        printf ("Nome: %s\n", tecnicos->tecnico[i].nome);
        printf ("Especialidade: %s",passar_int_string_especialidade(tecnicos->tecnico[i].especialidade));
        printf ("Estado: %s", passar_int_string_estado(tecnicos->tecnico[i].estado_tecnico));
        printf("Taxa de ocupação: %d\n", mostrarTaxaOcupacaoTecnico(&tecnicos->tecnico[i], ordens));
    }
}
/**
 * @brief Função que procura uma ordem pelo ID.
 * @param ordens Apontador para a estrutura Ordens.
 * @param idProcurado ID que o utilizador pretende procurar.
 * @return Retorna o índice da ordem no array caso seja encontrada (e esteja PENDENTE/EXECUCAO).
 * Caso contrário retorna -1.
 * @note Esta função só aceita ordens com estado PENDENTE ou EXECUCAO.
 */
int procurar_ordens_id (Ordens *ordens, int idProcurado) {
    for (int i = 0; i<ordens->contador; i++) {
        if (ordens->ordem[i].idOrdem == idProcurado && (ordens->ordem[i].estado == PENDENTE || ordens->ordem[i].estado == EXECUCAO)) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Função auxiliar que converte o estado de uma ordem (enum) para uma string legível.
 * @param est Estado da ordem.
 * @return Retorna uma string literal com a descrição do estado.
 */
const char *passar_int_string_estado_tecnicos (EstadoOrdem est) {
    switch (est) {
        case PENDENTE:
            return "Pendente";
        case EXECUCAO:
            return "Em execução";
        case CONCLUIDA:
            return "Concluída";
        case CANCELADA:
            return "Cancelada";
        default:
            return "Desconhecido";
    }
}

/**
 * @brief Função auxiliar que converte o tipo de manutenção (enum) para uma string legível.
 * @param tipo Tipo de manutenção.
 * @return Retorna uma string literal com a descrição do tipo.
 */
static const char *passar_int_string_tipo_manutencao(TipoManutencao tipo) {
    switch (tipo) {
        case PREVENTIVA:
            return "Preventiva";
        case CORRETIVA:
            return "Corretiva";
        default:
            return "Desconhecido";
    }
}

/**
 * @brief Função que lista as ordens/manutenções de acordo com o estado selecionado.
 * @param ordens Apontador para estrutura que contém a lista de ordens e contador.
 * @param estado Estado que deseja listar.
 * @param materiais Estrutura que contém a lista de materiais.
 */
void listarOrdensEstado (Ordens *ordens, EstadoOrdem estado, Materiais materiais) {
    switch (estado) {
        case PENDENTE:
            printf("\n===== ORDENS PENDENTES =====\n");
            for (int i = 0; i < ordens->contador; i++) {
                if (ordens->ordem[i].estado == estado) {
                    printf("ID ordem: %d\n", ordens->ordem[i].idOrdem);
                    printf("ID Ativo: %d\n", ordens->ordem[i].idAtivo);
                    printf("ID Departamento: %d\n", ordens->ordem[i].idDepartamento);
                    printf("Prioridade: %s\n", passarIntStringPrioridade(ordens->ordem[i].prioridade));
                    printf("Tipo manutenção: %s\n", passar_int_string_tipo_manutencao(ordens->ordem[i].tipo_manutencao));
                }
            }

            break;
        case EXECUCAO:
            printf("\n===== ORDENS EM EXECUÇÃO =====\n");
            break;
        case CONCLUIDA:
            printf("\n===== ORDENS CONCLUÍDAS =====\n");
            break;
        case CANCELADA:
            printf("\n===== ORDENS CANCELADAS =====\n");
            for (int i = 0; i < ordens->contador; i++) {
                if (ordens->ordem[i].estado == estado) {
                    printf("ID ordem: %d\n", ordens->ordem[i].idOrdem);
                    printf("ID Ativo: %d\n", ordens->ordem[i].idAtivo);
                    printf("ID Departamento: %d\n", ordens->ordem[i].idDepartamento);
                    printf("Prioridade: %s\n", passarIntStringPrioridade(ordens->ordem[i].prioridade));
                    printf("Tipo manutenção: %s\n", passar_int_string_tipo_manutencao(ordens->ordem[i].tipo_manutencao));
                }
            }
            break;
        default:
            return;
    }
    for (int i = 0; i < ordens->contador; i++) {
        if (ordens->ordem[i].estado == estado) {
            printf("ID ordem: %d\n", ordens->ordem[i].idOrdem);
            printf("ID Ativo: %d\n", ordens->ordem[i].idAtivo);
            printf("ID Departamento: %d\n", ordens->ordem[i].idDepartamento);
            printf("ID tecnico: %d\n", ordens->ordem[i].idTecnico);
            printf("Prioridade: %s\n", passarIntStringPrioridade(ordens->ordem[i].prioridade));
            printf("Tipo manutenção: %s\n", passar_int_string_tipo_manutencao(ordens->ordem[i].tipo_manutencao));
            printf("Custos associados: %f\n", calcularCustos(&ordens->ordem[i], &materiais));
        }
    }
}

/**
 * @brief Função que lista as ordens/manutenções consoante o tipo de prioridade que selecionar.
 * @param ordens Apontador para a estrutura que contém a lista de ordens e contador.
 * @param prioridade Tipo de prioridade que deseja listar.
 * @param materiais Estrutura que contém a lista de materiais e contador.
 */
void listarOrdensPrioridade (Ordens *ordens, Prioridade prioridade, Materiais materiais) {
    switch (prioridade) {
        case BAIXA:
            printf("\n===== ORDENS PRIORIDADE BAIXA =====\n");
            break;
        case MEDIA:
            printf("\n===== ORDENS PRIORIDADE MEDIA =====\n");
            break;
        case ALTA:
            printf("\n===== ORDENS PRIORIDADE ALTA =====\n");
            break;
        default:
            return;
    }
    for (int i = 0; i < ordens->contador; i++) {
        if (ordens->ordem[i].prioridade == prioridade) {
            printf("ID ordem: %d\n", ordens->ordem[i].idOrdem);
            printf("ID Ativo: %d\n", ordens->ordem[i].idAtivo);
            printf("ID Departamento: %d\n", ordens->ordem[i].idDepartamento);
            printf("Prioridade: %s\n", passarIntStringPrioridade(ordens->ordem[i].prioridade));
            printf("Tipo manutenção: %s\n", passar_int_string_tipo_manutencao(ordens->ordem[i].tipo_manutencao));
            printf("Custos associados: %f\n", calcularCustos(&ordens->ordem[i], &materiais));
        }
    }
}

/**
 * @brief Função que lista as ordens consoante o tipo de manutenção.
 * @param ordens Apontador para a estrutura que contém a lista de ordens e contador.
 * @param tipo Tipo de manutenção que deseja listar.
 * @param materiais Estrutura que contém a lista de materiais e contador.
 */
void listarOrdensTipo (Ordens *ordens, TipoManutencao tipo, Materiais materiais) {
    switch (tipo) {
        case PREVENTIVA:
            printf("\n===== ORDENS PREVENTIVAS =====\n");
            break;
        case CORRETIVA:
            printf("\n===== ORDENS CORRETIVAS =====\n");
            break;
        default:
            return;
    }
    for (int i = 0; i < ordens->contador; i++) {
        if (ordens->ordem[i].tipo_manutencao == tipo) {
            printf("ID ordem: %d\n", ordens->ordem[i].idOrdem);
            printf("ID Ativo: %d\n", ordens->ordem[i].idAtivo);
            printf("ID Departamento: %d\n", ordens->ordem[i].idDepartamento);
            printf("Prioridade: %s\n", passarIntStringPrioridade(ordens->ordem[i].prioridade));
            printf("Tipo manutenção: %s\n", passar_int_string_tipo_manutencao(ordens->ordem[i].tipo_manutencao));
        }
    }
}


/**
 * @brief Função auxiliar que converte a prioridade da manutenção (enum) para uma string legível.
 * @param prioridade Prioridade da manutenção.
 * @return Retorna uma string literal com a descrição da prioridade.
 */
static const char *passar_int_string_prioridade_manutencao(Prioridade prioridade) {
    switch (prioridade) {
        case BAIXA:
            return "Baixa";
        case MEDIA:
            return "Media";
        case ALTA:
            return "Alta";
        default:
            return "Desconhecido";
    }
}

/**
 * @brief Função que lista todas as ordens pendentes.
 * @param ordens Estrutura com o array de ordens e contador.
 * @note A listagem é feita via printf. Caso não existam ordens pendentes é apresentada uma mensagem.
 */
void listar_ordens_pendentes (Ordens ordens) {
    int contador = 0;
    for (int i = 0; i < ordens.contador ; i++) {
        if (ordens.ordem[i].estado == PENDENTE) {
            printf ("ID: %d\n", ordens.ordem[i].idOrdem);
            printf ("ID do Ativo: %d\n", ordens.ordem[i].idAtivo);
            printf ("ID do Departamento Associado: %d\n", ordens.ordem[i].idDepartamento);
            printf ("Tipo de manutenção: %s\n", passar_int_string_tipo_manutencao(ordens.ordem[i].tipo_manutencao));
            printf ("Prioridade: %s\n", passar_int_string_prioridade_manutencao(ordens.ordem[i].prioridade));
            contador++;
        }
    }

    if (contador == 0) {
        puts ("Não existem ocorrências pendentes.");
    }
}

/**
 * @brief Função que lista todas as ordens registadas.
 * @param ordens Estrutura com o array de ordens e contador.
 * @note Se não existirem ordens, a função informa o utilizador e termina.
 */
void listar_ordens (Ordens ordens) {
    if (ordens.contador == 0) {
        printf("Não existem ordens registadas.\n");
        return;
    }
    for (int i = 0; i< ordens.contador; i++) {
        if (ordens.ordem[i].estado == PENDENTE) {
            printf ("ID da manutenção: %d\n", ordens.ordem[i].idOrdem);
            printf ("ID do Ativo: %d\n", ordens.ordem[i].idAtivo);
            printf ("ID do Departamento Associado: %d\n", ordens.ordem[i].idDepartamento);
            printf ("Prioridade: %s\n", passar_int_string_prioridade_manutencao(ordens.ordem[i].prioridade));
        }else {
            printf("ID: %d\n", ordens.ordem[i].idOrdem);
            printf ("ID do Ativo: %d\n", ordens.ordem[i].idAtivo);
            printf ("ID do Departamento Associado: %d\n", ordens.ordem[i].idDepartamento);
            printf ("ID do Técnico associado: %d\n", ordens.ordem[i].idTecnico);
            printf("Tipo de manutenção: %s\n", passar_int_string_tipo_manutencao(ordens.ordem[i].tipo_manutencao));
            printf ("Estado da mautenção: %s\n",passar_int_string_estado_tecnicos(ordens.ordem[i].estado));
            printf("Prioridade: %s\n", passar_int_string_prioridade_manutencao(ordens.ordem[i].prioridade));
        }
    }
}

/**
 * @brief Função que cria uma nova ordem/manutenção e associa um ativo.
 * @param ativos Apontador para a estrutura de ativos.
 * @param ordens Apontador para a estrutura de ordens.
 * @param departamentos Estrutura de departamentos (usada para listar ativos por departamento).
 * @note A função valida se existem ativos disponíveis e se o ID introduzido existe e está OPERACIONAL.
 * @warning Esta função altera o estado do ativo para EM_MANUTENCAO e decrementa o contador
 * ativosDisponiveis. Também incrementa os contadores de ordens.
 */
void criar_ordem (Ativos *ativos, Ordens *ordens, Departamentos departamentos) {
    if (ativos == NULL || ordens == NULL) return;

    if (!garantir_capacidade_ordens(ordens, ordens->contador + 1)) {
        pausar_ecra();
        return;
    }

    int idProcurado;
    int idEncontrado;

    printf ("\n===== REGISTAR MANUTENÇÃO =====\n");
    listar_ativos_por_departamento(departamentos, *ativos);
    if (ativos->ativosDisponiveis == 0) {
        printf ("Não existem ativos disponiveis para enviar para manutenção.\n");
        pausar_ecra();
        return;
    }

    do {
        idProcurado = obterIntIntervalado(0, 999999, "Indique o ID do ativo que deseja enviar para Manutenção.\n");
        idEncontrado = procurar_ativo_id(ativos, idProcurado);
        if (idEncontrado == -1 || ativos->ativo[idEncontrado].estado != OPERACIONAL) {
            printf ("O ID introduzido é inválido. Tente novamente\n");
            idEncontrado = -1;
        }
    } while (idEncontrado == -1);

    int idx = ordens->contador;
    ordens->ordem[idx].idOrdem = gerarProximoID(ordens);
    ordens->ordem[idx].idAtivo = ativos->ativo[idEncontrado].id;
    ordens->ordem[idx].idDepartamento = ativos->ativo[idEncontrado].idDepartamentoAssociado;
    ordens->ordem[idx].estado = PENDENTE;
    ordens->ordem[idx].prioridade = obterIntIntervalado(1, 3, "Introduza a prioridade da ordem:\n1 - Baixa\n2 - Média\n3 - Alta\n");
    ordens->ordem[idx].tipo_manutencao = obterIntIntervalado(1, 2, "Introduza o tipo de manutenção que vai realizar:\n1 - Preventiva\n2 - Corretiva\n");

    ativos->ativo[idEncontrado].estado = EM_MANUTENCAO;
    ativos->ativosDisponiveis--;

    ordens->contador++;
    ordens->ordensAtivas++;

    printf("Registo realizado com sucesso.\n");
    registar_log("Info: Foi criada uma nova ordem/manutenção e um ativo foi enviado para manutenção.");
    pausar_ecra();
}

/**
 * @brief Função que permite gerir uma ordem, dependendo do seu estado.
 * @details Caso a ordem esteja PENDENTE permite alocar um técnico e adicionar materiais.
 * Caso esteja em EXECUCAO permite cancelar ou concluir a manutenção.
 * @param ordens Apontador para a estrutura de ordens.
 * @param tecnicos Apontador para a estrutura de técnicos.
 * @param ativos Apontador para a estrutura de ativos.
 * @param materiais Apontador para a estrutura de materiais.
 * @warning A função altera estados e contadores em várias estruturas (ordens, ativos e tecnicos).
 */
void gerir_ordem (Ordens *ordens, Tecnicos *tecnicos, Ativos *ativos, Materiais *materiais) {
    int idProcurado, idEncontrado, maxIdOrdens;
    maxIdOrdens = obterMaiorIDOrdens(*ordens);
    idProcurado = obterIntIntervalado(0, maxIdOrdens, "Indique o ID da ordem que deseja gerir.");
    idEncontrado = procurar_ordens_id(ordens, idProcurado);

    if (idEncontrado == -1) {
        printf("Ordem inválida (não encontrada ou não está pendente/em execução).\n");
        registar_log("Aviso: Tentativa de gerir ordem inválida (não encontrada ou estado incompatível).");
        pausar_ecra();
        return;
    }

    int escolha;
    int maxIdTecnicos;
    int sair;
    maxIdTecnicos = obterMaiorIDTecnicos(*tecnicos);
    switch (ordens->ordem[idEncontrado].estado) {
        case PENDENTE:
        do {
            ordens->ordem[idEncontrado].idTecnico = obterIntIntervalado(
                    0, maxIdTecnicos,
                    "Indique o id do técnico que deseja alocar para esta manutenção.\n");

            int idxTec = procurar_tecnico_id(*tecnicos, ordens->ordem[idEncontrado].idTecnico);
            if (idxTec == -1) {
                printf("O ID do técnico é inválido, tente novamente.\n");
                continue;
            }

            if (tecnicos->tecnico[idxTec].estado_tecnico == INATIVO1) {
                printf("O técnico que selecionou está inativo, tente novamente.\n");
                continue;
            }
            int manutAtivas = numeroManutencoesTecnico(tecnicos->tecnico[idxTec], *ordens);
            if (manutAtivas >= 5) {
                printf("O técnico que selecionou já tem 5 manutenções ativas, selecione outro.\n");
                continue;
            }

            break;
        } while (1);

        do {
            adicionar_materiais(materiais, ordens->ordem[idEncontrado].idOrdem);
            sair = obterIntIntervalado(1, 2, "Deseja adicionar outro material? (1) Sim (2) Não\n");
        }while (sair == 2);
        ordens->ordem[idEncontrado].estado = EXECUCAO;
        {
            time_t agora = time(NULL);
            struct tm *tmLocal = localtime(&agora);
            if (tmLocal != NULL) {
                ordens->ordem[idEncontrado].diaInicio = tmLocal->tm_mday;
                ordens->ordem[idEncontrado].mesInicio = tmLocal->tm_mon + 1;
                ordens->ordem[idEncontrado].anoInicio = tmLocal->tm_year + 1900;
                ordens->ordem[idEncontrado].horaInicio = tmLocal->tm_hour;
                ordens->ordem[idEncontrado].minInicio = tmLocal->tm_min;
                ordens->ordem[idEncontrado].segInicio = tmLocal->tm_sec;
            } else {
                ordens->ordem[idEncontrado].diaInicio = 1;
                ordens->ordem[idEncontrado].mesInicio = 1;
                ordens->ordem[idEncontrado].anoInicio = 1970;
                ordens->ordem[idEncontrado].horaInicio = 0;
                ordens->ordem[idEncontrado].minInicio = 0;
                ordens->ordem[idEncontrado].segInicio = 0;
            }
        }

        printf ("Manutenção começada com sucesso.\n");
        registar_log("Info: Uma manutenção passou para o estado EM EXECUÇÃO.");
        pausar_ecra();
        break;

        case EXECUCAO:
        escolha = obterIntIntervalado(1,2, "Indique a operação que deseja realizar:\n1 - Cancelar Manutenção\n2 - Concluir execução\n");
        switch (escolha) {
            case 1: {
                ordens->ordem[idEncontrado].estado = CANCELADA;
                {
                    time_t agora = time(NULL);
                    struct tm *tmLocal = localtime(&agora);
                    if (tmLocal != NULL) {
                        ordens->ordem[idEncontrado].diaFim = tmLocal->tm_mday;
                        ordens->ordem[idEncontrado].mesFim = tmLocal->tm_mon + 1;
                        ordens->ordem[idEncontrado].anoFim = tmLocal->tm_year + 1900;
                        ordens->ordem[idEncontrado].horaFim = tmLocal->tm_hour;
                        ordens->ordem[idEncontrado].minFim = tmLocal->tm_min;
                        ordens->ordem[idEncontrado].segFim = tmLocal->tm_sec;
                    } else {
                        ordens->ordem[idEncontrado].diaFim = 1;
                        ordens->ordem[idEncontrado].mesFim = 1;
                        ordens->ordem[idEncontrado].anoFim = 1970;
                        ordens->ordem[idEncontrado].horaFim = 0;
                        ordens->ordem[idEncontrado].minFim = 0;
                        ordens->ordem[idEncontrado].segFim = 0;
                    }
                }

                int idxAtivo = procurar_ativo_id(ativos, ordens->ordem[idEncontrado].idAtivo);
                if (idxAtivo != -1) {
                    ativos->ativo[idxAtivo].estado = OPERACIONAL;
                    ativos->ativosDisponiveis++;
                }

                printf ("A manutenção foi cancelada com sucesso");
                registar_log("Info: Uma manutenção foi cancelada.");
                pausar_ecra();
                break;
            }

            case 2: {
                ordens->ordem[idEncontrado].estado = CONCLUIDA;
                {
                    time_t agora = time(NULL);
                    struct tm *tmLocal = localtime(&agora);
                    if (tmLocal != NULL) {
                        ordens->ordem[idEncontrado].diaFim = tmLocal->tm_mday;
                        ordens->ordem[idEncontrado].mesFim = tmLocal->tm_mon + 1;
                        ordens->ordem[idEncontrado].anoFim = tmLocal->tm_year + 1900;
                        ordens->ordem[idEncontrado].horaFim = tmLocal->tm_hour;
                        ordens->ordem[idEncontrado].minFim = tmLocal->tm_min;
                        ordens->ordem[idEncontrado].segFim = tmLocal->tm_sec;
                    } else {
                        ordens->ordem[idEncontrado].diaFim = 1;
                        ordens->ordem[idEncontrado].mesFim = 1;
                        ordens->ordem[idEncontrado].anoFim = 1970;
                        ordens->ordem[idEncontrado].horaFim = 0;
                        ordens->ordem[idEncontrado].minFim = 0;
                        ordens->ordem[idEncontrado].segFim = 0;
                    }
                }

                int idxTec = procurar_tecnico_id(*tecnicos, ordens->ordem[idEncontrado].idTecnico);
                if (idxTec != -1) {
                    tecnicos->tecnico[idxTec].estado_tecnico = ATIVO1;
                }

                int idxAtivo = procurar_ativo_id(ativos, ordens->ordem[idEncontrado].idAtivo);
                if (idxAtivo != -1) {
                    ativos->ativo[idxAtivo].estado = OPERACIONAL;
                    ativos->ativosDisponiveis++;
                }

                printf("A manutenção foi concluida com sucesso.\n");
                registar_log("Info: Uma manutenção foi concluída com sucesso.");
                pausar_ecra();
                break;
            }
            default:
                printf("Opção inválida.\n");
                pausar_ecra();
                break;
        }
        break;
            case CONCLUIDA:
                printf("A ordem que selecionou ja foi concluida.\n");
                pausar_ecra();
                break;

            case CANCELADA:
                printf("A ordem que selecionou já foi cancelada.\n");
                pausar_ecra();
                break;
            default:
                printf("Estado inválido.\n");
                pausar_ecra();
                break;
    }
}

/**
 * @brief Função que guarda as ordens num ficheiro binário.
 * @param ordens Apontador para a estrutura de ordens.
 * @warning Escreve no ficheiro "ordens.bin". Caso o ficheiro não possa ser aberto, a função termina.
 */
void guardarOrdens (Ordens *ordens) {
    FILE *fp = fopen("ordens.bin", "wb");

    if (fp == NULL) {
        printf ("Erro ao abrir ficheiro de ordens para escrita.\n");
        registar_log("Erro: Não foi possível abrir ordens.bin para escrita.");
        return;
    }

    fwrite (&ordens->contador, sizeof(int), 1, fp);
    fwrite(&ordens->ordensAtivas, sizeof(int), 1, fp);

    for (int i = 0; i < ordens->contador; i++) {
        fwrite (&ordens->ordem[i].idTecnico, sizeof(int), 1, fp);
        fwrite (&ordens->ordem[i].idOrdem, sizeof(int), 1, fp);
        fwrite (&ordens->ordem[i].idDepartamento, sizeof(int), 1, fp);
        fwrite (&ordens->ordem[i].idAtivo, sizeof(int), 1, fp);
        fwrite (&ordens->ordem[i].tipo_manutencao, sizeof(TipoManutencao), 1, fp);
        fwrite (&ordens->ordem[i].prioridade, sizeof(Prioridade), 1, fp);
        fwrite (&ordens->ordem[i].estado, sizeof(EstadoOrdem), 1, fp);

        fwrite(&ordens->ordem[i].diaInicio, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].mesInicio, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].anoInicio, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].horaInicio, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].minInicio, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].segInicio, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].diaFim, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].mesFim, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].anoFim, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].horaFim, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].minFim, sizeof(int), 1, fp);
        fwrite(&ordens->ordem[i].segFim, sizeof(int), 1, fp);
    }
    fclose (fp);
}

/**
 * @brief Função que carrega as ordens a partir de um ficheiro binário.
 * @param ordens Apontador para a estrutura de ordens.
 * @note Caso o ficheiro "ordens.bin" não exista, a função termina sem alterar nada.
 * @warning A função utiliza malloc para alocar memória para o array de ordens.
 */
void carregarOrdens (Ordens *ordens) {

    FILE *fp = fopen ("ordens.bin", "rb");

    if (fp == NULL) return;

    fread (&ordens->contador, sizeof(int), 1, fp);
    ordens->capacidade = ordens->contador;

    ordens->ordem = malloc(ordens->capacidade * sizeof(Ordem));
    if (ordens->ordem == NULL && ordens->contador > 0) {
        registar_log("Erro: Falha ao alocar memória ao carregar ordens (malloc devolveu NULL).");
        fclose(fp);
        return;
    }

    fread (&ordens->ordensAtivas, sizeof(int), 1, fp);

    for (int i = 0; i < ordens->contador; i++) {
        fread(&ordens->ordem[i].tipo_manutencao, sizeof(TipoManutencao), 1, fp);
        fread(&ordens->ordem[i].idTecnico, sizeof(int), 1, fp);
        fread(&ordens->ordem[i].idOrdem, sizeof(int), 1, fp);
        fread(&ordens->ordem[i].idDepartamento, sizeof(int), 1, fp);
        fread(&ordens->ordem[i].idAtivo, sizeof(int), 1, fp);
        fread(&ordens->ordem[i].prioridade, sizeof(Prioridade), 1, fp);
        fread(&ordens->ordem[i].estado, sizeof(EstadoOrdem), 1, fp);
        if (fread(&ordens->ordem[i].diaInicio, sizeof(int), 1, fp) == 1) {
            fread(&ordens->ordem[i].mesInicio, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].anoInicio, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].horaInicio, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].minInicio, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].segInicio, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].diaFim, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].mesFim, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].anoFim, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].horaFim, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].minFim, sizeof(int), 1, fp);
            fread(&ordens->ordem[i].segFim, sizeof(int), 1, fp);
        } else {
            ordens->ordem[i].diaInicio = 0;
            ordens->ordem[i].mesInicio = 0;
            ordens->ordem[i].anoInicio = 0;
            ordens->ordem[i].horaInicio = 0;
            ordens->ordem[i].minInicio = 0;
            ordens->ordem[i].segInicio = 0;
            ordens->ordem[i].diaFim = 0;
            ordens->ordem[i].mesFim = 0;
            ordens->ordem[i].anoFim = 0;
            ordens->ordem[i].horaFim = 0;
            ordens->ordem[i].minFim = 0;
            ordens->ordem[i].segFim = 0;
        }
    }
    fclose (fp);
}