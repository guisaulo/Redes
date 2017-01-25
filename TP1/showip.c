/*==================================================================================================================
Trabalho Prático 1 - Socket, medição de desempenho                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
showip.c: Exibe os endereços IPs ipv4 e ipv6 para um dado nome de hospedeiro. 
====================================================================================================================*/

#include "showip.h"

//Retorna 4 caso o ip seja ipv4 ou 6 caso o ip seja ipv6
int especifica_ip(char *argv[]) {

    struct addrinfo aux, *ip, *n;
    int status;
	
    memset(&aux, 0, sizeof aux);
    aux.ai_family = AF_UNSPEC; 
    aux.ai_socktype = SOCK_STREAM;  

    if ((status = getaddrinfo(argv[1], NULL, &aux, &ip)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 0;
    }

    for(n=ip; n!=NULL; n=n->ai_next) {
        if (n->ai_family == AF_INET) { // IPv4
            printf("Protocolo IPv4 detectado.\n\n");
            freeaddrinfo(ip); // Libera a lista de endereços struct addrinfo
            return 4;
        }
        else { // IPv6
            printf("Protocolo IPv6 detectado.\n\n");
            freeaddrinfo(ip); // Libera a lista de endereços struct addrinfo
            return 6;
        }
    }
    
	return 0;
}
