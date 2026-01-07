#include <stdio.h>
#include <stdlib.h>
#include "../include/menu.h"
#include "../include/tecnicos.h"
#include "../include/ordem.h"
#include "../include/ativos.h"
#include "../include/departamentos.h"
#include "../include/input.h"
#include "../include/materiais.h"
#include "../include/logs.h"
#include "../include/relatorios.h"


/**
 * @brief Ponto de entrada principal do Sistema de Gestão de Manutenção de Ativos.
 * @details Gere o ciclo de vida da aplicação, incluindo a inicialização das estruturas,
 * o carregamento dos dados a partir de ficheiros binários (persistência) e a
 * exibição do menu principal. No encerramento, garante a salvaguarda dos dados.
 * @return Retorna 0 após a execução bem-sucedida do programa.
 */
int main(void) {
    Departamentos *departamentos = malloc(sizeof(*departamentos));
    if (departamentos == NULL) {
        printf("ERRO: Falha ao alocar memoria para a estrutura Departamentos.\n");
        registar_log("Erro: Falha na alocação de memória para a estrutura de departamentos.");
        return 1;
    }
    departamentos->departamento = NULL;
    departamentos->contador = 0;
    departamentos->departamentosAtivos = 0;
    departamentos->capacidade = 0;

    Ativos *ativos = malloc(sizeof(*ativos));
    if (ativos == NULL) {
        printf("ERRO: Falha ao alocar memoria para a estrutura Ativos.\n");
        registar_log("Erro: Falha na alocação de memória para a estrutura de Ativos");
        free(departamentos);
        return 1;
    }
    ativos->ativo = NULL;
    ativos->contador = 0;
    ativos->ativosDisponiveis = 0;
    ativos->capacidade = 0;

    Tecnicos *tecnicos = malloc(sizeof(*tecnicos));
    if (tecnicos == NULL) {
        printf("ERRO: Falha ao alocar memoria para a estrutura Tecnicos.\n");
        registar_log("Erro: falha na alocação de memória para a estrutura de Técnicos");
        free(ativos);
        free(departamentos);
        return 1;
    }

    tecnicos->tecnico = NULL;
    tecnicos->contador = 0;
    tecnicos->tecnicosAtivos = 0;
    tecnicos->capacidade = 0;


    Ordens *ordens = malloc(sizeof(*ordens));
    if (ordens == NULL) {
        printf("ERRO: Falha ao alocar memoria para a estrutura Ordens.\n");
        registar_log("Erro: Falha na alocação de memória para a estrutura de Ordens");
        free(tecnicos);
        free(ativos);
        free(departamentos);
        return 1;
    }
    ordens->ordem = NULL;
    ordens->contador = 0;
    ordens->ordensAtivas = 0;
    ordens->capacidade = 0;

    Materiais *materiais = malloc(sizeof(*materiais));
    if (materiais == NULL) {
        printf("ERRO: Falha ao alocar memoria para a estrutura Materiais.\n");
        registar_log("Erro: Falha na alocação de memória para a estrutura de Materiais");
        free(ordens);
        free(tecnicos);
        free(ativos);
        free(departamentos);
        return 1;
    }
    materiais->material = NULL;
    materiais->contador = 0;
    materiais->capacidade = 0;

    carregarDepartamentos(departamentos);
    carregarAtivos(ativos);
    carregarTecnicos(tecnicos);
    carregarOrdens(ordens);
    carregarMateriais(materiais);
    int escolha, escolha_ativos, escolha_departamentos, escolha_tecnico, escolha_manutencoes, escolha_relatorios;
    int sair = 0;
    do {
        apresentar_menu();
        escolha = obterIntIntervalado(1,6,"Indique o menu que deseja consultar:\n");

        switch (escolha) {
            case 1:
                printf("\n===== GERIR ATIVOS =====\n");
                printf("1 - Adicionar ativo\n");
                printf("2 - Listar todos os ativos\n");
                printf("3 - Abater ativo\n");
                printf("4 - Pesquisar ativo\n");
                printf("5 - Voltar\n");
                escolha_ativos = obterIntIntervalado(1,4,"Indique qual opção deseja utilizar:\n");
                switch (escolha_ativos) {
                    case 1:
                        criar_ativo(ativos, departamentos);
                        break;
                    case 2:
                        listar_ativos(*ativos);
                        break;
                    case 3:
                        abater_ativo(ativos);
                        pausar_ecra();
                        break;
                    case 4:
                        listarComPesquisaInteligente(*ativos);
                        pausar_ecra();
                        break;
                    case 5:
                        pausar_ecra();
                        break;
                    default:
                        printf("Opção inválida.\n");
                        pausar_ecra();
                        break;
                }
                break;
            case 2:
                printf("\n===== GERIR DEPARTAMENTO =====\n");
                printf("1 - Criar departamento\n");
                printf("2 - Listar departamento\n");
                printf("3 - Atualizar departamento\n");
                printf("4 - Inativar departamento\n");
                printf("5 - Voltar\n");
                escolha_departamentos = obterIntIntervalado(1,5, "Indique qual opção deseja utilizar:\n");
                switch (escolha_departamentos) {
                    case 1:
                        criarDepartamento(departamentos);
                        break;
                    case 2:
                        listar_departamentos(*departamentos);
                        break;
                    case 3:
                        atualizar_departamento(departamentos);
                        break;
                    case 4:
                        inativar_Departamento(departamentos);
                        break;
                    case 5:
                        pausar_ecra();
                        break;
                    default:
                        printf("Opção inválida.\n");
                        pausar_ecra();
                        break;
                }
                break;
            case 3:
                printf("\n===== GERIR TÉCNICOS =====\n");
                printf("1 - Adicionar técnicos\n");
                printf("2 - Listar técnicos\n");
                printf("3 - Desativar técnicos\n");
                printf("4 - Voltar\n");
                escolha_tecnico = obterIntIntervalado(1,4, "Indique qual a opção que deseja usar\n");
                switch (escolha_tecnico) {
                    case 1:
                        criar_tecnico(tecnicos);
                        break;
                    case 2:
                        listar_tecnicos(*tecnicos);
                        break;
                    case 3:
                        desativar_tecnico(*tecnicos);
                        break;
                    case 4:
                        pausar_ecra();
                        break;
                    default:
                        printf("Opção inválida.\n");
                        pausar_ecra();
                        break;
                }
                break;
            case 4:
                printf("1 - Criar manutenção\n");
                printf("2 - Gerir manutenção\n");
                printf("3 - Listar manutenções\n");
                printf("4 - Voltar\n");
                escolha_manutencoes = obterIntIntervalado(1,4, "Indique qual opção deseja usar:\n");
                switch (escolha_manutencoes) {
                    case 1:
                        criar_ordem(ativos,ordens,*departamentos);
                        pausar_ecra();
                        break;
                    case 2:
                        gerir_ordem(ordens,tecnicos,ativos, materiais);
                        pausar_ecra();
                        break;
                    case 3:
                        listar_ordens(*ordens);
                        pausar_ecra();
                        break;
                    case 4:
                        pausar_ecra();
                        break;
                    default:
                        printf("Opção inválida.\n");
                        pausar_ecra();
                        break;
                }
                break;
            case 5:
                printf("1 - Ver Logs\n");
                printf("2 - Ver relatório de ativos\n");
                printf("3 - Ver relatório de departamentos\n");
                printf("4 - Ver relatório de técnicos\n");
                printf("5 - Ver relatório de ordens\n");
                printf("6 - Ver relatório de ativos instáveis\n");
                printf("7 - Ver relatório de problemas por local\n");
                printf("8 - Voltar\n");
                escolha_relatorios = obterIntIntervalado(1, 8, "Indique a opção que deseja utilizar\n");

                switch (escolha_relatorios) {
                    case 1:
                        consultar_logs();
                        pausar_ecra();
                        break;
                    case 2:
                        mostrarRelatorioAtivos(ativos);
                        pausar_ecra();
                        break;
                    case 3:
                        mostrarRelatorioDepartamentos(departamentos, ativos, ordens);
                        pausar_ecra();
                        break;
                    case 4:
                        mostrarRelatorioTecnicos(tecnicos, ordens);
                        pausar_ecra();
                        break;
                    case 5:
                        mostrarRelatorioOrdens(ordens, *materiais);
                        pausar_ecra();
                        break;
                    case 6:
                        relatorioAtivosInstaveis(*ativos, *ordens);
                        pausar_ecra();
                        break;
                    case 7:
                        relatorioProblemasPorLocal(*ativos, *ordens);
                        pausar_ecra();
                        break;
                    case 8:
                        pausar_ecra();
                        break;
                    default:
                        printf("Opção desconhecida");
                        pausar_ecra();
                        break;
                }
                break;
            case 6:
                sair = 1;
                printf("Volte sempre");
                break;
            default:
                printf("Opção inválida.\n");
                pausar_ecra();
                break;
        }

    } while (sair == 0);
    guardarDepartamentos(departamentos);
    guardarAtivos(ativos);
    guardarTecnicos(tecnicos);
    guardarOrdens(ordens);
    guardarMateriais(materiais);
    free(ordens);
    free(tecnicos);
    free(ativos);
    free(departamentos);
    free(materiais);
    return 0;
}
