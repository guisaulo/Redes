/*==================================================================================================================
Trabalho Prático 1 - Socket, medição de desempenho                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
client.c: Contém o codigo do cliente
====================================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "io.h"
#include "showip.h"
#include "time.h"

int main(int argc, char *argv[]){
    
	int sockfd, porta_servidor;
    int bytes_enviados = 0, total_bytes_enviados = 0; //armazena bytes enviados ao servidor
    double start, total_time, kbps; //tempo inicial para gettimeofday    
    char buffer[TAM_BUF]; //buffer para armazenar mensagens recebidas e enviadas
	FILE *file;	//arquivo que conterá a lista de arquivos de um diretorio
	
	//Testa se todos os argumentos foram passados corretamente
	if (argc < 4) {
		fprintf(stderr,"Digite: %s <host_do_servidor> <porta_servidor> <nome_diretorio>\n", argv[0]);
        exit(0);
    }
	
	printf("\e[2J\e[H"); //limpa a tela do terminal
	printf("## Cliente ##\n");
	printf("host_do_servidor: %s porta_servidor: %s nome_diretorio: %s\n", argv[1], argv[2], argv[3]);
	
	//Caso o protocolo seja IPv4
	if(especifica_ip(argv) == 4){
		
		struct sockaddr_in serv_addr;
    	struct hostent *server; //contem nome do host, alias, tipo do endereço (ipv6), o comprimento do endereço em bytes e lista de endereços
		
		//1 - Cria Socket
		porta_servidor = atoi(argv[2]); //Converte a porta de conexao de uma string para um inteiro
		sockfd = socket(AF_INET, SOCK_STREAM, 0); //(IPV4, TCP, 0)
		if (sockfd < 0) 
		    error("ERRO ao abrir socket");
		else
			printf("1 - Socket criado com sucesso.\n");			
		server = gethostbyname(argv[1]); //obtem nome host
		bzero((char *) &serv_addr, sizeof(serv_addr)); //Inicializa o buffer serv_addr com zero    
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //copia o endereço de s1 para s2
		serv_addr.sin_family = AF_INET; //define o dominio da estrutura do servidor como Ipv4
		serv_addr.sin_port = htons(porta_servidor); //define a porta do servidor por meio de uma conversao da porta em uma sequencia de bytes
	
		//2 - Estabelece conexão
		if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //descritor, endereço, tamanho do endereço
		    error("ERROR na conexão com o servidor.");
		else
			printf("2 - Conexao com o servidor estabelecida.\n");    
    }
    
    //Caso o protocolo seja IPv6 (a sintaxe de muitas estruturas mudam)
    else{
		
		struct sockaddr_in6 serv_addr;
    	struct hostent *server; //contem nome do host, alias, tipo do endereço (ipv6), o comprimento do endereço em bytes e lista de endereços
		
		//1 - Cria Socket
		porta_servidor = atoi(argv[2]); //Converte a porta de conexao de uma string para um inteiro
		sockfd = socket(AF_INET6, SOCK_STREAM, 0); //(IPV6, TCP, 0)
		if (sockfd < 0) 
		    error("ERRO ao abrir socket");
		else
			printf("1 - Socket criado com sucesso.\n");
		server = gethostbyname2(argv[1], AF_INET6); //obtem nome host
		bzero((char *) &serv_addr, sizeof(serv_addr)); //Inicializa o buffer serv_addr com zero    
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin6_addr, server->h_length); //copia o endereço de s1 para s2
		serv_addr.sin6_family = AF_INET6; //define o dominio da estrutura do servidor como Ipv6
		serv_addr.sin6_port = htons(porta_servidor); //define a porta do servidor por meio de uma conversao da porta em uma sequencia de bytes
		
		//2 - Estabelece conexão
		if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //descritor, endereço, tamanho do endereço
		    error("ERROR na conexão com o servidor.");
		else
			printf("2 - Conexao com o servidor estabelecida.\n"); 	
	}
     
    //Cliente envia READY ao servidor e começa a medir o tempo e bytes transmitidos
    bzero(buffer,TAM_BUF); //Inicializa o buffer com zero
    if (write(sockfd, "READY", 5) < 0)
    	error("ERRO ao mandar READY");
	
	//Pega o tempo inicial
	start = get_time();
	
    //Clinte recebe READY ACK do servidor
    bzero(buffer,TAM_BUF); //Inicializa o buffer com zero
 	if (read(sockfd, buffer, TAM_BUF) < 0)
 		error("ERRO ao receber READY ACK");
 	
 	//Cliente começa enviar a lista de arquivos ao servidor	     
    if(strcmp(buffer, "READY ACK") == 0){
		
		printf("%s recebido do servidor.\n", buffer);
		
		// Envia nome do diretorio para o servidor
		bzero(buffer,TAM_BUF); 	
    	if(strcmp(argv[3], ".")  == 0){
    		if (write(sockfd, "_dir_local.txt", 14) < 0)
        		error("ERROR ao enviar nome do diretorio");
    	}
    	else{
   			if (write(sockfd, "_outro_dir.txt", 14) < 0)
   				error("ERROR ao enviar nome do diretorio");    	
    	}
		
		//Obtem lista de arquivos de um diretorio passado por parametro e salva essa lista em um arquivo
		cria_lista_arquivos(argc, argv);
		
		// Abre o diretorio(arquivo) e envia lista de arquivos
	 	if(strcmp(argv[3], ".")  == 0){
		 	if ((file = fopen("_dir_local.txt", "r")) == NULL) {
				fprintf(stderr, "Arquivo _dir_local.txt não foi aberto.\n");
			 	close(sockfd); // Closing connection
			 	exit(EXIT_FAILURE);
		 	}
		}
		else{
			if ((file = fopen("_outro_dir.txt", "r")) == NULL) {
				fprintf(stderr, "Arquivo _outro_dir.txt não foi aberto.\n");
			 	close(sockfd); // Closing connection
			 	exit(EXIT_FAILURE);
			}
		}
					
    	//Cliente envia um arquivo de cada vez ao servidor
		printf("3 - Enviando lista de arquivos ao servidor...\n");
		bzero(buffer, TAM_BUF);
    	while(fread(buffer, 1, TAM_BUF, file) != 0) { //le uma linha de cada vez e envia para o cliente
			bytes_enviados = write(sockfd, buffer, strlen(buffer)); //contem o numero de bytes enviados
			total_bytes_enviados += bytes_enviados; 
			bytes_enviados = 0;
			bzero(buffer, TAM_BUF);
		}
    	fclose(file);
	} //Fim transmissao de arquivos ao servidor
    
    //Envia "bye" ao servidor e fecha a conexao (aqui ocorre um erro, ver documentacao)
    printf("Fim da transmissao. Enviando 'bye' ao servidor.\n");
  	/*bzero(buffer, TAM_BUF);
    if (write(sockfd, NULL, 0) < 0)
    	error("ERROR ao enviar bye ao servidor");
   	*/
   	 
    //4 - Fim conexão
    close(sockfd);
    printf("4 - Fim Conexão.\n\n");
    
    //Imprime informações
    total_time = get_time() - start;
    kbps = (total_bytes_enviados / total_time) / 1024;
    printf("Dados da execucao:\nBuffer: %d bytes\n%d bytes enviados em %3.6fs\nThoughput: %10.2f kbps\n", TAM_BUF, total_bytes_enviados, total_time, kbps);
    
    return 0;
}
