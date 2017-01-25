/*==================================================================================================================
Trabalho Prático 3 - Servidor para múltiplos clientes                 
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
cliente.c: Contém o codigo do cliente
====================================================================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include "io.h"

int main(int argc, char *argv[]){
 	
 	//Declaração das variaveis
	int sockfd, porto_servidor;
 	struct sockaddr_in servaddr; 
 	struct hostent *server;
 	char *host_do_servidor;
 	pid_t pid_cliente;
	
 	//Processa argumentos da linha de comando
 	processaArgumentosCliente(argc,argv,&host_do_servidor,&porto_servidor);

 	//Preparação dos endereços do socket
 	server = gethostbyname(host_do_servidor); //obtem nome host
 	memset(&servaddr, 0, sizeof(servaddr)); //Inicializa o buffer servaddr com zero    
 	bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length); //copia o endereço de s1 para s2
 	servaddr.sin_family = AF_INET; //define o dominio da estrutura do servidor como Ipv4
 	servaddr.sin_port =  htons(porto_servidor); //define a porta do servidor

    //Cria socket para o cliente (IPV4, TCP, 0)
 	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
  		perror("Error ao criar socket do cliente!\n");
  		return 1;
 	}
 	printf("sockfd: %d\n",sockfd);

 	//Estabelece conexões
 	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  		perror("Error na conexão com o servidor!\n");
  		return 1;
 	}
	
	pid_cliente=getpid(); //será usado como id do cliente
	printf("Id cliente: %d\n",pid_cliente);
	processamentoCliente(sockfd, pid_cliente);
	
 	close(sockfd);
    free(host_do_servidor);
 	return 0;
}