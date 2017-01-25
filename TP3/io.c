/*==================================================================================================================
Trabalho Prático 3 - Servidor para múltiplos clientes                  
Redes de Computadores                               
Aluno: Guilherme Saulo Alves                         
io.c: Realiza a comunicação do programa com o ambiente
====================================================================================================================*/

#include "io.h"
#define TAMBUFFER 4096

void processaArgumentosServidor(int argc, char *argv[], int *porto_servidor){

	if(argc<2){
		printf("%d\n",argc);
        printf("Erro: Digite os parâmetros corretos!\n");
        printf("./servidor <porto_servidor>\n");
        exit(1);
    }
    
    else{
	    *porto_servidor=atoi(argv[1]);
		
		//Limpa tela do terminal e imprime algumas informaçoes
    	printf("\e[2J\e[H"); 
    	printf("## Servidor ##\n");
    	printf("porto_servidor: %d\n\n", *porto_servidor);
    }
}

void processaArgumentosCliente(int argc, char *argv[], char **host_do_servidor, int *porto_servidor){

	if(argc<3){
    	printf("Erro: Digite os parâmetros corretos!\n");
        printf("./cliente <host_do_servidor> <porto_servidor>\n");
        exit(1);
    } 
    else{
    	*host_do_servidor=(char*)malloc(sizeof(char)*strlen(argv[1]));
    	strcpy(*host_do_servidor, argv[1]);
   		*porto_servidor=atoi(argv[2]);

    	//Limpa tela do terminal e imprime algumas informaçoes
    	printf("\e[2J\e[H"); 
    	printf("## Cliente ##\n");
    	printf("host_do_servidor: %s porto_servidor: %d\n\n", *host_do_servidor,*porto_servidor);
    }
}

void processamentoServidor(int client_sock, int bmin, int bmax, int pmin, int pmax){

    int id_cliente, a, b, c, x, y, z;
    char sendline[TAMBUFFER],recvline[TAMBUFFER];

    if(recv(client_sock,recvline,TAMBUFFER,0)>0){
        
        //Recebeu id do cliente
        id_cliente=atoi(recvline);
        bzero(recvline,TAMBUFFER);
        printf("Id do cliente: %d\n", id_cliente);
        
        //envia intervalos
        sprintf(sendline, "%d,%d,%d,%d", bmin, bmax, pmin, pmax);
        send(client_sock, sendline, strlen(sendline), 0);
        printf("Intervalo enviado: [bmin:%d bmax:%d] [pmin:%d pmax:%d]\n", bmin, bmax, pmin, pmax);
        
        //Cria um arquivo com o intervalo atual com tags html
        FILE *atual;
        atual=fopen("intervalos_atuais.txt", "w+");
        if(atual==NULL){
            perror("Error ao abrir arquivo!\n");
            exit(1);
        }
        else{
            fprintf(atual,"<tr><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>Não Encontrado</td></tr>\n", bmin, bmax, pmin, pmax, id_cliente);   
        }
        fclose(atual);

        if((recv(client_sock,recvline,TAMBUFFER,0)>0)&&(strcmp(recvline,"ACK")==0)){
            
            //Recebeu confirmação dos intervalos
            printf("Cliente %d confirmou %s dos intervalos.\n", id_cliente, recvline);
            bzero(recvline,TAMBUFFER);
            
            if(recv(client_sock,recvline,TAMBUFFER,0)>0){
                
                //Recebe TRUE caso encontre um contra-exemplo e FALSE caso não encontre
                if((strcmp(recvline,"TRUE")==0) && (recv(client_sock, recvline,TAMBUFFER,0)>0)){
                    
                    //Encontrou Contra-Exemplo e recebeu valores de a,b,c,z,y,z
                    a=atoi(strtok(recvline,","));
                    b=atoi(strtok(NULL,","));
                    c=atoi(strtok(NULL,","));
                    x=atoi(strtok(NULL,","));
                    y=atoi(strtok(NULL,","));
                    z=atoi(strtok(NULL,","));
                    printf("Contra-exemplo encontrado:\na:%d b:%d c:%d x:%d y:%d z:%d\n", a,b,c,x,y,z);
                    
                    //Cria um arquivo com os dados dos intervalos procurados com tags html
                    FILE *procurados;
                    procurados=fopen("intervalos_procurados.txt", "a+");
                    if(procurados==NULL){
                        perror("Error ao abrir arquivo!\n");
                        exit(1);
                    }
                    else{
                        fprintf(procurados,"<tr><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>A:%dB:%dC:%dz:%dy:%dz:%d</td></tr>\n", bmin, bmax, pmin, pmax, id_cliente, a, b, c, x, y, z);   
                    }
                    fclose(procurados);   
                }

                else{
                    //Cria um arquivo com os dados dos intervalos procurados com tags html
                    printf("Contra-exemplo não encontrado.\n");
                    FILE *procurados;
                    procurados=fopen("intervalos_procurados.txt", "a+");
                    if(procurados==NULL){
                        perror("Error ao abrir arquivo!\n");
                        exit(1);
                    }
                    else{
                        fprintf(procurados,"<tr><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>Não Encontrado</td></tr>\n", bmin, bmax, pmin, pmax, id_cliente);   
                    }
                    fclose(procurados);
                }
            }

            else{
                perror("Error ao receber confirmações de contra-exemplos!\n");
                exit(1);
            }
        }

        else{
            perror("Error ao receber confirmação dos intervalos do cliente!\n");
            exit(1);
        }
    }

    else{
        perror("Error ao receber id do cliente!\n");
        exit(1);    
    }
}

void processamentoCliente(int sockfd, int id_cliente){
    
    int bmin, bmax, pmin, pmax, a, b, c, x, y, z;
    char sendline[TAMBUFFER],recvline[TAMBUFFER];

    //Envia id do cliente ao servidor
    sprintf(sendline, "%d", id_cliente);
    send(sockfd, sendline, strlen(sendline), 0);

    if(recv(sockfd,recvline,TAMBUFFER,0)>0){
        
        //Recebeu intervalos do servidor
        bmin=atoi(strtok(recvline,","));
        bmax=atoi(strtok(NULL,","));
        pmin=atoi(strtok(NULL,","));
        pmax=atoi(strtok(NULL,","));
        printf("Intervalo recebido: [bmin:%d bmax:%d] [pmin:%d pmax:%d]\n", bmin, bmax, pmin, pmax);
        send(sockfd, "ACK", 3, 0); //envia confirmação de recebimento correto de intervalo
        
        if(getContraExemplo(bmin,bmax,pmin,pmax,&a,&b,&c,&x,&y,&z)){
            //Um contra-exemplo foi encontrado e o cliente envia os valores de a,b,c,x,y,z ao servidor
            printf("Contra-Exemplo encontrado: a:%d b:%d c:%d x:%d y:%d z:%d\n", a,b,c,x,y,z);
            send(sockfd, "TRUE", 4, 0);
            bzero(sendline,TAMBUFFER);
            sprintf(sendline, "%d,%d,%d,%d,%d,%d", a, b, c, x, y, z);
            send(sockfd, sendline, strlen(sendline),0);
            printf("Valores enviados ao servidor!\n");
        }
        else{
            //Contra-exemplo nao encontrado
            bzero(sendline,TAMBUFFER);
            send(sockfd, "FALSE", 4, 0);
            printf("Contra-exemplo não encontrado.\n");
        }
    }

    else{
        perror("Error ao receber intervalos do servidor!\n");
        exit(1);
    }
}

void paginaHTML(){

    static char *html_part1="<!DOCTYPE html><html><head><meta charset='UTF-8'><title>TP3 - Servidor</title><style>table,th,td{border:1px solid black;border-collapse:collapse;}th,td{padding:5px;}th{text-align:left;}</style></head>";
    static char *html_part2="<body><h1>Status do Servidor</h1><h3>Intervalos procurados</h3><table><tr><th>bmin</th><th>bmax</th><th>pmin</th><th>pmax</th><th>id_cliente</th><th>Contra-Exemplos</th></tr>";
    static char *html_part3="</table><h3>Intervalos sendo processados</h3><table><tr><th>bmin</th><th>bmax</th><th>pmin</th><th>pmax</th><th>id_cliente</th><th>Contra-Exemplos</th></tr>";
    static char *html_part4="</table></body></html>";
    
    char buf[TAMBUFFER];
    char bufprocurados[TAMBUFFER];

    FILE *html;
    html=fopen("index.html", "w+");
    if(html==NULL){
        perror("Error ao abrir arquivo!\n");
        exit(1);
    }
    else{
        //abre arquivo de intervalos ja procurados
        FILE *procurados;
        procurados=fopen("intervalos_procurados.txt", "r+");
        if(procurados==NULL){
            perror("Error ao abrir arquivo!\n");
            exit(1);
        }
        else{
            while(fgets(buf, TAMBUFFER, procurados) != NULL){
                strcat(bufprocurados, buf);        
            }
            fprintf(html,"%s%s%s%s", html_part1, html_part2, bufprocurados, html_part3);
        }
       
        fclose(procurados);
        bzero(bufprocurados,TAMBUFFER);
        bzero(buf,TAMBUFFER);

        //abre arquivo de intervalos sendo procurados no momento
        FILE *atual;
        atual=fopen("intervalos_atuais.txt", "r");
        if(atual==NULL){
            perror("Error ao abrir arquivo!\n");
            exit(1);
        }
        else{
            while(fgets(buf, TAMBUFFER, atual) != NULL){
                strcat(bufprocurados, buf);        
            }
            fprintf(html,"%s%s", bufprocurados, html_part4);   
        }
        fclose(atual);
    }
    fclose(html);
}

void getIntervalo(int *bmin, int *bmax, int *pmin, int *pmax){ 

    int a,b,c,d, existe=0;
    char buf[TAMBUFFER];

    //cria arquivo para salvar estado dos intervalos
    FILE *intervalo;
    intervalo=fopen("intervalo.txt", "a+");
    if(intervalo==NULL){
        perror("Error ao abrir arquivo!\n");
        exit(1);
    }
    else{
        //Cria intervalos (9 numero para base e 7 numeros para expoentes)
        *bmin=(*bmin)+1;
        *bmax=(*bmin)+9; //mudar aqui
        *pmin=(*pmin)+1;
        *pmax=(*pmin)+7; //mudar aqui
        
        while(fgets(buf, TAMBUFFER, intervalo) != NULL){
           //Carrega valores de intervalos já procurados
            a=atoi(strtok(buf,","));
            b=atoi(strtok(NULL,","));
            c=atoi(strtok(NULL,","));
            d=atoi(strtok(NULL,",")); 
           
            if((*bmin == a) && (*bmax == b) && (*pmin == c) && (*pmax == d))
                existe++;
        }
        
        if(existe > 0){
            //pega o proximo intervalo depois do ultimo procurado
            *bmin=a+1;
            *bmax=(*bmin)+9; //mudar aqui
            *pmin=c+1;
            *pmax=(*pmin)+7; //mudar aqui
        }

        fprintf(intervalo, "%d,%d,%d,%d\n",*bmin,*bmax,*pmin,*pmax); //salva o intervalo no arquivo
    }
    fclose(intervalo);
}

int testaConjectura(int i, int j, int k, int l, int m, int n){

    //verificica se a equacao tem solucao
    if((fabs(pow(i,l) + pow(j,m))) == (fabs(pow(k,n)))){
        printf("%d^%d(%.0f) + %d^%d(%.0f) == %d^%d(%.0f) valido!\n",i,l,pow(i,l),j,m,pow(j,m),k,n,pow(k,n));
        return 1;    
    }
    else{
        //printf("%d^%d(%.0f) + %d^%d(%.0f) != %d^%d(%.0f) invalido!\n",i,l,pow(i,l),j,m,pow(j,m),k,n,pow(k,n));
        return 0;    
    }
}

int mdc(int n, int a, int b, int c){

    int i,j,flag=0,small,*data;

    data=(int *)malloc(sizeof(int)*n);

    //obtem os numeros
    data[0]=a;
    data[1]=b;
    data[2]=c;

    //encontra o menor dos n numeros
    small=data[0];
    for(i=1; i<n; i++){
        if(data[i]<small)
            small=data[i];
    }

    //começa pelo menor até 1 se o valor divide por todas entradas validas
    for(i=small; i>0; i--){
        for(j=0; j<n; j++){
            if(data[j]%i != 0){
                flag=1;
            }
        }
        //imprime o resultado
        if(!flag){
            //printf("mdc(%d,%d,%d)=%d\n", data[0],data[1],data[2],i);
            break;
        }
        flag=0;
    }
    return i;    
}

int getContraExemplo(int bmin, int bmax, int pmin, int pmax, int *a, int *b, int *c, int *x, int *y, int *z){
    
    int i,j,k,l,m,n,encontrou=0;

    for(i=bmin; i<=bmax; i++){
        for(j=bmin;j<=bmax; j++){
            for(k=bmin; k<=bmax; k++){
                if(mdc(3,i,j,k)==1){
                    for(l=pmin; l<=pmax; l++){
                        for(m=pmin; m<=pmax; m++){
                            for(n=pmin; n<=pmax; n++){
                                if(testaConjectura(i,j,k,l,m,n)==1){
                                    encontrou++;
                                    printf("contra exemplo encontrado!\n");
                                    *a=i;
                                    *b=j;
                                    *c=k;
                                    *x=l;
                                    *y=m;
                                    *z=n;
                                }
                            }
                        }
                    }    
                }
            }
        }
    }

    if(encontrou>0)
        return 1;
    else
        return 0;        
}