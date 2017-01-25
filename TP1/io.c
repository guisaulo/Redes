/*==================================================================================================================
Trabalho Prático 1 - Socket, medição de desempenho                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
io.c: Realiza a comunicação do programa com o ambiente, criando lista de arquivos de um diretorio em um arquivo.  
====================================================================================================================*/

#include "io.h"

//É chamada quando uma chamada de sistema falha. Exibe a mensagem e sai.
void error(const char *msg){
	perror(msg);
    exit(0);
}

//Obtem lista de arquivos de um diretorio passado por parametro e salva essa lista em um arquivo
void cria_lista_arquivos(int argc, char *argv[]){
	
	DIR *directory; // Variaveis utilizadas na leitura dos arquivos do diretorio
	struct dirent *entry;
	FILE *file;	
	
	if((directory = opendir(argv[3])) == NULL){
		fprintf(stderr, "Nao foi possivel abrir o diretorio %s\n", argv[3]);
		exit(1);
	}
	
	if(strcmp(argv[3], ".")  == 0){
		if ((file = fopen("_dir_local.txt", "w")) == NULL) {
			fprintf(stderr, "Arquivo %s não foi aberto.\n", argv[3]);
		 	exit(EXIT_FAILURE);
	 	}
 	}

 	else{
		if ((file = fopen("_outro_dir.txt", "w")) == NULL) {
			fprintf(stderr, "Arquivo %s não foi aberto.\n", argv[3]);
		 	exit(EXIT_FAILURE);
	 	}
	}
	
	// Enquanto o ponteiro 'directory' aponta para algum arquivo no diretorio
	while((entry = readdir(directory)) != NULL) {
		fprintf(file, "%s\n", entry->d_name); //armazena no diretorio (arquivo)
	}
	
	fclose(file);
	closedir(directory);
}
