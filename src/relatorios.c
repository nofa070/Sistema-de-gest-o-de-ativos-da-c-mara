/**
 * @file relatorios.c
 * @brief Ficheiro que contém funções de relatórios/análises do sistema.
 * @author Francisco Alves
 */

#include <stdio.h>
#include <string.h>
#include "../include/ativos.h"
#include "../include/departamentos.h"
#include "../include/ordem.h"
#include "../include/materiais.h"
#include <time.h>

/**
 * @brief Conta quantos ativos existem num determinado estado.
 * @param ativos Estrutura que contém a lista de ativos.
 * @param estado_ativo Estado que pretende contar.
 * @return Retorna o número de ativos com o estado indicado.
 */
int contadorPorEstado (Ativos ativos, EstadoAtivo estado_ativo) {
    int contador = 0;
    for (int i = 0; i<ativos.contador; i++) {
        if (ativos.ativo[i].estado == estado_ativo) {
            contador++;
        }
    }
    return contador;
}



/**
 * @brief Função que exibe o ranking de desempenho dos técnicos de acordo com o número de ordens concluídas.
 * @param tecnicos Estrutura que contém a lista de tecnicos e contador.
 * @param ordens Estrutura que contém a lista de ordens e contador.
 */
void mostrarRankingDesempenho(Tecnicos tecnicos, Ordens ordens) {
    typedef struct {
        int id;
        char nome[50];
        int totalConcluidas;
    } AuxRanking;

    AuxRanking ranking[tecnicos.contador];

    for (int i = 0; i < tecnicos.contador; i++) {
        ranking[i].id = tecnicos.tecnico[i].idTecnico;
        strcpy(ranking[i].nome, tecnicos.tecnico[i].nome);

        int concluidas = 0;
        for (int j = 0; j < ordens.contador; j++) {
            if (ordens.ordem[j].idTecnico == ranking[i].id &&
                strcmp(passar_int_string_estado_tecnicos(ordens.ordem[j].estado), "Concluída") == 0) {
                concluidas++;
                }
        }
        ranking[i].totalConcluidas = concluidas;
    }
    for (int i = 0; i < ordens.contador - 1; i++) {
        for (int j = 0; j < ordens.contador - i - 1; j++) {
            if (ranking[j].totalConcluidas < ranking[j + 1].totalConcluidas) {
                AuxRanking temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }
    printf("\n===== RANKING DE DESEMPENHO =====\n");

    for (int i = 0; i < tecnicos.contador; i++) {
        printf("%d. %s - %d ordens concluidas\n",
               i + 1,
               ranking[i].nome,
               ranking[i].totalConcluidas);
    }
}

void listarTecnciosOcupados (Tecnicos *tecnicos, Ordens *ordens) {
    printf ("\n===== TECNICOS OCUPADOS =====\n");
    for (int i=0; i < tecnicos->contador; i++) {
        if (tecnicos->tecnico[i].estado_tecnico == OCUPADO)
            printf ("ID: %d", tecnicos->tecnico[i].idTecnico);
        printf ("Nome: %s\n", tecnicos->tecnico[i].nome);
        printf ("Especialidade: %s\n",passar_int_string_especialidade(tecnicos->tecnico[i].especialidade));
        printf ("Estado: %s\n", passar_int_string_estado(tecnicos->tecnico[i].estado_tecnico));
        printf("Taxa ocupação: %d", mostrarTaxaOcupacaoTecnico(&tecnicos->tecnico[i], ordens));
    }
}

void listarTecnicosEspecialidade (Tecnicos *tecnicos, Especialidade especialidade, Ordens *ordens) {
    printf("\n===== TECNICOS POR ESPECIALIDADE =====\n");
    for (int i = 0; i < tecnicos->contador; i++) {
        if (tecnicos->tecnico[i].especialidade == especialidade) {
            printf("Nome: %s\n", tecnicos->tecnico[i].nome);
            printf("Especialidade: %s\n", passar_int_string_especialidade(tecnicos->tecnico[i].especialidade));
            printf("Estado: %s\n", passar_int_string_estado(tecnicos->tecnico[i].estado_tecnico));
            printf("Taxa de ocupação: %d", mostrarTaxaOcupacaoTecnico(&tecnicos->tecnico[i], ordens));
        }
    }
}

/**
 * @brief Função que lista apenas os técnicos ativos.
 * @param tecnicos Estrutura com o array de técnicos e contador.
 * @note Apenas são listados os técnicos com estado ATIVO1.
 */
static void listar_tecnicos_ativos_relatorio (Tecnicos *tecnicos, Ordens *ordens) {
    printf ("\n===== TECNICOS ATIVOS =====\n");
    for (int i=0; i < tecnicos->contador; i++) {
        if (tecnicos->tecnico[i].estado_tecnico == ATIVO1)
            printf ("ID: %d", tecnicos->tecnico[i].idTecnico);
        printf ("Nome: %s\n", tecnicos->tecnico[i].nome);
        printf ("Especialidade: %s",passar_int_string_especialidade(tecnicos->tecnico[i].especialidade));
        printf ("Estado: %s\n", passar_int_string_estado(tecnicos->tecnico[i].estado_tecnico));
        printf("Taxa de ocupação: %d\n", mostrarTaxaOcupacaoTecnico(&tecnicos->tecnico[i], ordens));
    }
}

/**
 * @brief Conta quantos ativos existem numa determinada categoria.
 * @param ativos Estrutura que contém a lista de ativos.
 * @param categoria_ativo Categoria que pretende contar.
 * @return Retorna o número de ativos na categoria indicada.
 */
int contadorPorCategoria (Ativos ativos, CategoriaAtivo categoria_ativo) {
    int contador = 0;
    for (int i = 0; i<ativos.contador; i++) {
        if (ativos.ativo[i].categoria == categoria_ativo) {
            contador++;
        }
    }
    return contador;
}

/**
 * @brief Calcula o tempo médio de resolução das ordens concluídas.
 * @details A função calcula a diferença (em segundos) entre a data/hora de início e fim de cada
 * ordem concluída e devolve a média.
 * @param ordens Apontador para a estrutura que contém as ordens.
 * @return Retorna o tempo médio (em segundos). Caso não existam ordens concluídas retorna 0.
 */
float tempoMedioResolucaoOrdens (Ordens *ordens) {
    if (ordens == NULL) {
        return 0;
    }

    if (ordens->contador == 0) {
        return 0;
    }

    float soma = 0;
    int total = 0;

    for (int i = 0; i < ordens->contador; i++) {
        if (ordens->ordem[i].estado == CONCLUIDA) {
            if (ordens->ordem[i].anoInicio == 0 || ordens->ordem[i].anoFim == 0) {
                continue;
            }

            struct tm inicio = {
                .tm_mday = ordens->ordem[i].diaInicio,
                .tm_mon = ordens->ordem[i].mesInicio - 1,
                .tm_year = ordens->ordem[i].anoInicio - 1900,
                .tm_hour = ordens->ordem[i].horaInicio,
                .tm_min = ordens->ordem[i].minInicio,
                .tm_sec = ordens->ordem[i].segInicio
        };

            struct tm fim = {
                .tm_mday = ordens->ordem[i].diaFim,
                .tm_mon = ordens->ordem[i].mesFim - 1,
                .tm_year = ordens->ordem[i].anoFim - 1900,
                .tm_hour = ordens->ordem[i].horaFim,
                .tm_min = ordens->ordem[i].minFim,
                .tm_sec = ordens->ordem[i].segFim
        };

            time_t tInicio = mktime(&inicio);
            time_t tFim = mktime(&fim);

            if (tInicio == (time_t)-1 || tFim == (time_t)-1) {
                continue;
            }

            float diferenca = (float)difftime(tFim, tInicio);
            if (diferenca < 0) {
                continue;
            }

            soma += diferenca;
            total++;
        }
    }

    if (total == 0) {
        return 0;
    }

    return soma / (float)total;
}


int procurarIndiceMaisCorretivas (Ativos ativos) {
    int maior = 0;
    int indice =0;
    for (int i = 0; i < ativos.contador; i++) {
        if (ativos.ativo[i].contagemManutencoesCorretivas > maior) {
            maior = ativos.ativo[i].contagemManutencoesCorretivas;
            indice = i;
        }
    }
    return indice;
}

char *departamentosMaisUrgentes (Departamentos *departamentos, Ordens *ordens) {
    int indice_maior = 0;
    int total = 0;

    if (ordens->contador == 0) {
        return "n/a";
    }
    for (int i = 0; i<departamentos->contador; i++) {
        int soma = 0;
        for (int j = 0; j < ordens->contador; j++) {
            if (departamentos->departamento[i].idDepartamento == ordens->ordem[j].idDepartamento) {
                switch (ordens->ordem[j].prioridade) {
                    case BAIXA: {
                        soma += 1;
                        break;
                    }
                    case MEDIA: {
                        soma += 3;
                        break;
                    }
                    case ALTA: {
                        soma += 5;
                        break;
                    }
                }
                }
            }
        if (soma > total) {
            total = soma;
            indice_maior = i;
        }
    }
    return departamentos->departamento[indice_maior].nomeDepartamento;
}

void mostrarRelatorioAtivos(Ativos *ativos) {
    printf("\n==== RELATÓRIO DE ATIVOS ====\n");
    printf("Numero total de ativos: %d\n", ativos->contador);
    printf("Numero de ativos no sistema (não inclui os ativos previamente abatidos): %d\n", ativos->ativosDisponiveis);
    printf("Número de ativos operacionais: %d\t\tNumero de viaturas: %d\t\t Numero de ferramentas: %d\n", contadorPorEstado(*ativos,OPERACIONAL), contadorPorCategoria(*ativos, VIATURA), contadorPorCategoria(*ativos, FERRAMENTA));
    printf("Numero de ativos em manutenção: %d\t\tNumero de itens de informática: %d\t\tOutros tipos de ativos: %d\n", contadorPorEstado(*ativos, EM_MANUTENCAO), contadorPorCategoria(*ativos, INFORMATICA), contadorPorCategoria(*ativos,OUTRO));
    printf("Numero de ativos abatidos: %d\t\tNumero de itens de mobiliário: %d\n", contadorPorEstado(*ativos, ABATIDO), contadorPorCategoria(*ativos, MOBILIARIO));
    if (ativos->ativo[procurarIndiceMaisCorretivas(*ativos)].contagemManutencoesCorretivas == 0) {
        printf("Ativo com mais manutenções Corretivas: n/a Nº de correções: n/a\n");
    }
    if (ativos->ativo[procurarIndiceMaisCorretivas(*ativos)].contagemManutencoesCorretivas > 0)
    printf("Ativo com mais manutenções Corretivas: %s Nº de correções: %d\n", ativos->ativo[procurarIndiceMaisCorretivas(*ativos)].designacao, ativos->ativo[procurarIndiceMaisCorretivas(*ativos)].contagemManutencoesCorretivas);
}

void mostrarRelatorioDepartamentos (Departamentos *departamentos, Ativos *ativos, Ordens *ordens) {
    printf("\n==== RELATÓRIO DE DEPARTAMENTOS ====\n");
    printf("Numero total de departamentos: %d\n", departamentos->contador);
    printf("Numero de departamentos ativos: %d\n", departamentos->departamentosAtivos);
    printf("Numero de departamentos inativos: %d\n", (departamentos->contador - departamentos->departamentosAtivos));
    listar_ativos_por_departamento(*departamentos, *ativos);
    printf("Departamento com mais pedidos de manutenção urgentes: %s\n", departamentosMaisUrgentes(departamentos, ordens));
}

void mostrarRelatorioTecnicos (Tecnicos *tecnicos, Ordens *ordens) {
    listar_tecnicos_ativos_relatorio(tecnicos, ordens);
    listarTecnciosOcupados(tecnicos, ordens);
    listarTecnicosEspecialidade(tecnicos, TECNICO_TI, ordens);
    listarTecnicosEspecialidade(tecnicos, MECANICO, ordens);
    listarTecnicosEspecialidade(tecnicos, ELETRICISTA, ordens);
    listarTecnicosEspecialidade(tecnicos, MANUTENCAO_GERAL, ordens);
    listarTecnicosEspecialidade(tecnicos, OUTROS, ordens);
    mostrarRankingDesempenho(*tecnicos, *ordens);
}

void mostrarRelatorioOrdens (Ordens *ordens, Materiais materiais) {
    listarOrdensPrioridade(ordens, BAIXA, materiais);
    listarOrdensPrioridade(ordens, MEDIA, materiais);
    listarOrdensPrioridade(ordens, ALTA, materiais);
    listarOrdensEstado(ordens, PENDENTE, materiais);
    listarOrdensEstado(ordens, EXECUCAO, materiais);
    listarOrdensEstado(ordens, CONCLUIDA, materiais);
    listarOrdensEstado(ordens, CANCELADA, materiais);
    listarOrdensTipo(ordens, PREVENTIVA, materiais);
    listarOrdensTipo(ordens, CORRETIVA, materiais);
    printf("Tempo médio de resolução: %f", tempoMedioResolucaoOrdens(ordens));
}

/**
 * @brief Mostra um alerta com os ativos que têm muitas ocorrências associadas.
 * @details Percorre os ativos e conta quantas ordens estão associadas a cada um.
 * Se um ativo tiver 5 ou mais ocorrências, é apresentado como "instável".
 * @param ativos Estrutura com a lista de ativos.
 * @param ordens Estrutura com a lista de ordens.
 */
void relatorioAtivosInstaveis(Ativos ativos, Ordens ordens) {
    int encontrou = 0;

    printf("\n===== ALERTA: ATIVOS INSTÁVEIS =====\n");

    if (ativos.contador == 0 || ativos.ativo == NULL) {
        printf("Não existem ativos registados.\n");
        return;
    }

    if (ordens.contador == 0 || ordens.ordem == NULL) {
        printf("Não existem ordens registadas.\n");
        return;
    }

    for (int i = 0; i < ativos.contador; i++) {
        int contagem = 0;

        for (int j = 0; j < ordens.contador; j++) {
            if (ordens.ordem[j].idAtivo == ativos.ativo[i].id) {
                contagem++;
            }
        }

        if (contagem >= 5) {
            printf("%s (ID %d) - %d ocorrências registadas!\n",
                   ativos.ativo[i].designacao ? ativos.ativo[i].designacao : "(sem designação)",
                   ativos.ativo[i].id,
                   contagem);
            encontrou = 1;
        }
    }

    if (encontrou == 0) {
        printf("Não existem ativos instáveis (5 ou mais ocorrências).\n");
    }
}

/**
 * @brief Mostra uma análise de incidências agrupada por local.
 * @details Para cada ativo, conta quantas ordens existem e soma o total no local do ativo.
 * No fim apresenta o número de ordens por local.
 * @param ativos Estrutura com a lista de ativos.
 * @param ordens Estrutura com a lista de ordens.
 */
void relatorioProblemasPorLocal(Ativos ativos, Ordens ordens) {
    printf("\n===== ANÁLISE DE INCIDÊNCIAS POR LOCAL =====\n");

    if (ativos.contador == 0 || ativos.ativo == NULL) {
        printf("Não existem ativos registados.\n");
        return;
    }

    if (ordens.contador == 0 || ordens.ordem == NULL) {
        printf("Não existem ordens registadas.\n");
        return;
    }
    const char *locais[ativos.contador];
    int contagens[ativos.contador];
    int totalLocais = 0;

    for (int i = 0; i < ativos.contador; i++) {
        int contagemAtivo = 0;

        for (int j = 0; j < ordens.contador; j++) {
            if (ordens.ordem[j].idAtivo == ativos.ativo[i].id) {
                contagemAtivo++;
            }
        }

        if (contagemAtivo == 0) {
            continue;
        }

        const char *local = ativos.ativo[i].localizacao;
        if (local == NULL || local[0] == '\0') {
            local = "(sem local)";
        }

        int indice = -1;
        for (int k = 0; k < totalLocais; k++) {
            if (strcmp(locais[k], local) == 0) {
                indice = k;
                break;
            }
        }

        if (indice == -1) {
            locais[totalLocais] = local;
            contagens[totalLocais] = contagemAtivo;
            totalLocais++;
        } else {
            contagens[indice] += contagemAtivo;
        }
    }

    if (totalLocais == 0) {
        printf("Não existem incidências associadas a nenhum local.\n");
        return;
    }

    for (int i = 0; i < totalLocais; i++) {
        printf("Local: %s - %d ordens\n", locais[i], contagens[i]);
    }
}
