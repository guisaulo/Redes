/*==================================================================================================================
Trabalho Prático 1 - Socket, medição de desempenho                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
io.h: Contém as declarações das funções e bibliotecas utilizadas em io.c
====================================================================================================================*/

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <dirent.h> // Biblioteca usada para listar os arquivos do diretorio

#define TAM_BUF 512

//É chamada quando uma chamada de sistema falha. Exibe a mensagem e sai.
void error(const char *msg);

//Obtem lista de arquivos de um diretorio passado por parametro e salva essa lista em um arqui
void cria_lista_arquivos(int argc, char *argv[]);

#endif

