/*==================================================================================================================
Trabalho Prático 1 - Socket, medição de desempenho                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
time.c: Define a função gettimeofday
====================================================================================================================*/

#include "time.h"

//Obtem um certo tempo do relogio do computador e retorna em segundos
double get_time() {
	
struct timeval time;
	gettimeofday(&time, NULL);
	return time.tv_sec + (time.tv_usec/1000000.0);
}
