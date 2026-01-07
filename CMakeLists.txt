cmake_minimum_required(VERSION 4.0)
project(lp_final C)

set(CMAKE_C_STANDARD 11)

add_executable(lp_final
        src/main.c
        src/input.c
        src/departamentos.c
        src/ativos.c
        src/tecnicos.c
        src/ordem.c
        src/menu.c
        src/materiais.c
        include/materiais.h
        src/logs.c
        include/logs.h
        src/relatorios.c
        include/relatorios.h
)
