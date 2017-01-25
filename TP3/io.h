/*==================================================================================================================
Trabalho Prático 3 - Servidor para múltiplos clientes                  
Redes de Computadores                               
Aluno: Guilherme Saulo Alves                         
io.h: Contém as declarações das funções e bibliotecas utilizadas em io.c
====================================================================================================================*/

#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <math.h>

//Processa argumentos da linha de comando do servidor e imprime informações na tela
void processaArgumentosServidor(int argc, char *argv[], int *porto_servidor);

//Processa argumentos da linha de comando do cliente e imprime informações na tela
void processaArgumentosCliente(int argc, char *argv[], char **host_do_servidor, int *porto_servidor);

//Realiza a transmissão de mensagens entre Servidor e Cliente
void processamentoServidor(int client_sock, int bmin, int bmax, int pmin, int pmax);

//Realiza a transmissão de mensagens entre Cliente e Servidor
void processamentoCliente(int sockfd, int id_cliente);

//Retorna uma pagina HTML com os intervalos já procurados e os que estão sendo procurados
void paginaHTML();

//Obtem um valor de intervalo válido que não foi procurado
void getIntervalo(int *bmin, int *bmax, int *pmin, int *pmax);

//Retorna 1 caso a equação possua solução e 0 caso não possua solução
int testaConjectura(int i, int j, int k, int l, int m, int n);

//Retorna o mdc de 3 numeros
int mdc(int n, int a, int b, int c);

//Algoritmo para encontrar o contra-exemplo
int getContraExemplo(int bmin, int bmax, int pmin, int pmax, int *a, int *b, int *c, int *x, int *y, int *z);

#endif