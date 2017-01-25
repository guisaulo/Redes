/*==================================================================================================================
Trabalho Prático 1 - Socket, medição de desempenho                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
showip.h: Exibe os endereços IPs ipv4 e ipv6 para um dado nome de hospedeiro. 
====================================================================================================================*/

#ifndef SHOWIP_H
#define SHOWIP_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//Retorna 4 caso o ip seja ipv4 ou 6 caso o ip seja ipv6
int especifica_ip(char *argv[]);

#endif
