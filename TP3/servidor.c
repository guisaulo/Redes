/*==================================================================================================================
Trabalho Prático 3 - Servidor para múltiplos clientes                 
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
servidor.c: Contém o codigo do servidor
====================================================================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include "io.h"

#define NUMCONEXOES 20

//Manipulador de sinal para colher processos zumbis
static void wait_for_child(int sig) {

    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]){

    //Declaração das variaveis
    int server_sock, client_sock, porto_servidor;
    int bmin=0, bmax=0, pmin=2, pmax=0; //valores iniciais de intervalos
    pid_t pid, pid_cliente, pid_pai;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr; 
    struct sigaction sa;

	//Processa argumentos da linha de comando
    processaArgumentosServidor(argc,argv,&porto_servidor);

	if(porto_servidor == 80){
        //Retorna pagina arquivo HTML no diretorio
        paginaHTML();

    }
    else{
        //Obtem informações dos endereços
    	servaddr.sin_family=AF_INET; //define o dominio da estrutura do servidor como Ipv4
    	servaddr.sin_addr.s_addr=INADDR_ANY; //define endereço do servidor como localhost
    	servaddr.sin_port=htons(porto_servidor); //define a porta do servidor

        //Cria socket para o servidor (IPV4, TCP, 0)
    	if((server_sock=socket(AF_INET,SOCK_STREAM,0))<0){
            perror("Error ao criar socket do servidor!\n");
            return 1;
        }

    	//Vincula socket para endereço do servidor
        if(bind(server_sock,(struct sockaddr *) &servaddr,sizeof(servaddr))<0){
            perror("Error no bind!\n");
            return 1; 		
        }

    	//Escuta conexões atraves de um fila de conexoes
        if(listen(server_sock,NUMCONEXOES)<0){
            perror("Error no listen!\n");
            return 1; 	
        }
        else
            printf("Servidor aguardando conexões..\n");

        //Configura o manipulador de sinal
        sa.sa_handler=wait_for_child;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if(sigaction(SIGCHLD, &sa, NULL)<0){
            perror("sigaction");
            return 1;
        }

    	//Servidor roda infinitamente tratando conexões
        while(1){
            
            //Aceita conexão do cliente
    		clilen=sizeof(cliaddr); //tamanho do endereço do servidor  
    		client_sock=accept(server_sock, (struct sockaddr *) &cliaddr, &clilen); //recebe um novo descritor
    		printf("\nUma nova conexão com um cliente foi estabelecida.\n");
            
            //Gera intervalo valido para os clientes
            getIntervalo(&bmin,&bmax,&pmin,&pmax);  
           
            pid=fork(); //cria processo filho
            if(pid==0){ 
      		    //se for 0 é processo filho
                pid_cliente=getpid();
                pid_pai=getppid();
                printf ("Processo filho %d criado.\n", pid_cliente);
                close(server_sock); //fecha listening socket
                processamentoServidor(client_sock,bmin,bmax,pmin,pmax);
                return 1;	
            }
            else{
      		    //É processo pai
                if(pid==-1){ 
                    printf("Error no fork.\n");
                    return 1;
                }
                else {
                    pid_pai=getppid();
                    printf("Id processo pai: %d\n", pid_pai);
                    close(client_sock);
                }
            }
        }
        
        close(server_sock);
    }

    return 0;
}