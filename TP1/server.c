/*==================================================================================================================
Trabalho Prático 1 - Socket, medição de desempenho                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
server.c: client.c: Contém o codigo do servidor
====================================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "io.h"
#include "showip.h"
#include "time.h"

int main(int argc, char *argv[]) {
	
    int sockfd, client_sock, porta_servidor; //descritores de arquivos e numero da porta
    socklen_t clilen; //tamanho do endereço do cliente a ser usado na função accept
    char buffer[TAM_BUF]; //buffer para armazenar mensagens recebidas e enviadas 
    FILE *file; //arquivo que conterá a lista de arquivos de um diretorio
    	      
    //Testa se todos os argumentos foram passados, principalmente o host e a porta do servidor
    if (argc < 3) {
	    fprintf(stderr,"Digite: %s <host_do_servidor> <porta_servidor>\n", argv[0]);
        exit(0);
	}
     
    printf("\e[2J\e[H"); //limpa a tela do terminal
	printf("## Servidor ##\n");
	printf("host_do_servidor: %s porta_servidor: %s\n", argv[1], argv[2]);  
    
    //Caso o protocolo seja IPv4 
	if(especifica_ip(argv) == 4){
		
		struct sockaddr_in serv_addr, cli_addr; //cria estrutura que contém o endereço do servidor e do cliente
		
		//1 - Cria Socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0); //(IPV4, TCP, 0)
		if (sockfd < 0) 
			error("ERRO ao criar socket.");
		else
			printf("1 - Socket Criado com sucesso!\n");	 
		bzero((char *) &serv_addr, sizeof(serv_addr)); //Inicializa o buffer serv_addr com zero
		porta_servidor = atoi(argv[2]); //Converte a porta de conexao de uma string para um inteiro
	   	serv_addr.sin_family = AF_INET; //define o dominio da estrutura do servidor como Ipv4
	   	serv_addr.sin_addr.s_addr = INADDR_ANY; //define endereço do servidor. INADDR_ANY=localhost.
		serv_addr.sin_port = htons(porta_servidor); //define a porta do servidor por meio de uma conversao da porta em uma sequencia de bytes
		 
		//2 - Vincula socket para endereço do servidor
		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
			error("ERROR no bind.");
		else
			printf("2 - Socket vinculado ao endereço.\n");
		 	
		//3 - Escuta conexões
		printf("3 - Aguardando conexão com cliente...\n");
		listen(sockfd,1); //apenas 1 um cliente pode conectar
		clilen = sizeof(cli_addr); //tamanho do endereço do servidor      
		 
		//4 - Aceita conexão do cliente
		client_sock = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //acorda o processo quando uma conexão é estabelecida e recebe um novo descritor
		if (client_sock < 0) 
			error("ERROR no accept");
		else
			printf("4 - Conexão estabelecida.\n");
    }
    
    //Caso o protocolo seja IPv6 (a sintaxe de muitas estruturas mudam)
    else{
		
		struct sockaddr_in6 serv_addr, cli_addr; //cria estrutura que contém o endereço do servidor e do cliente
		
		//1 - Cria Socket
		sockfd = socket(AF_INET6, SOCK_STREAM, 0); //(IPV6, TCP, 0)
		if (sockfd < 0) 
			error("ERRO ao criar socket.");
		else
			printf("1 - Socket Criado com sucesso!\n");	 
		bzero((char *) &serv_addr, sizeof(serv_addr)); //Inicializa o buffer serv_addr com zero
		porta_servidor = atoi(argv[2]); //Converte a porta de conexao de uma string para um inteiro
	   	serv_addr.sin6_family = AF_INET6; //define o dominio da estrutura do servidor como Ipv6
	   	serv_addr.sin6_addr = in6addr_any; //define endereço do servidor. INADDR_ANY=::1
		serv_addr.sin6_port = htons(porta_servidor); //define a porta do servidor por meio de uma conversao da porta em uma sequencia de bytes
		 
		//2 - Vincula socket para endereço do servidor
		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
			error("ERROR no bind.");
		else
			printf("2 - Socket vinculado ao endereço.\n");
		 	
		//3 - Escuta conexões
		printf("3 - Aguardando conexão com cliente...\n");
		listen(sockfd,1); //apenas 1 um cliente pode conectar
		clilen = sizeof(cli_addr); //tamanho do endereço do servidor      
		 
		//4 - Aceita conexão do cliente
		client_sock = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //acorda o processo quando uma conexão é estabelecida e recebe um novo descritor
		if (client_sock < 0) 
			error("ERROR no accept");
		else
			printf("4 - Conexão com o cliente estabelecida.\n");
	}
         
	//Servidor recebe READY do cliente
    bzero(buffer,TAM_BUF); //Inicializa o buffer com zero
	if (read(client_sock, buffer, TAM_BUF) < 0)
		error("ERROR ao receber READY");
    
    //Servidor envia READY ACK ao cliente
    if (strcmp(buffer, "READY") == 0 ){
        printf("%s recebido do cliente.\n", buffer); 
    	bzero(buffer,TAM_BUF);
 	 	if (write(client_sock, "READY ACK", 9) < 0)
        	error("ERROR ao mandar READY ACK");
	}
     
	//Servidor recebe nome do diretorio a ser criado pelo cliente
    bzero(buffer, TAM_BUF); 
	if (read(client_sock, buffer, TAM_BUF) < 0)
		error("ERROR ao receber nome do diretorio");
  
	//Servidor cria diretorio (arquivo chamado <host_do_servidor><nome_diretorio>)
	if ((file = fopen(strcat(argv[1], buffer), "w")) == NULL) {
		fprintf(stderr, "Arquivo %s não criado.\n", strcat(argv[1], buffer));
		close(client_sock); // Closing connection
		exit(EXIT_FAILURE);
	}
	printf("Arquivo %s criado.\nArmazenando lista de arquivos...\n", strcat(argv[1], buffer));
	
	//Servidor armazena os nomes dos arquivos no diretorio (arquivo)
	bzero(buffer, TAM_BUF);
	while(read(client_sock, buffer, TAM_BUF) > 0) {
		fprintf(file, "%s", buffer); //armazena no diretorio (arquivo)
		bzero(buffer, TAM_BUF);
	}
    
    //recebe "bye" do cliente e salva arquivo (aqui ocorre um erro, ver documentacao)
	/*bzero(buffer,TAM_BUF); 
 	if (read(client_sock, buffer, TAM_BUF) < 0)
		error("ERROR ao receber bye");	     
    if(strcmp(buffer, "bye") == 0){
		printf("bye recebido. Salvando arquivo...\n");
	}*/
        
	//5 - Salva arquivo e fecha conexão
    printf("Salvando arquivo...\n");
    fclose(file);
    close(client_sock);
    close(sockfd);
    printf("5 - Fim Conexão.\n");
     
    return 0; 
}
