Aluno: Guilherme Saulo Alves

######################### Como usar Programas ###################################

servidor (./servidor <porta_servidor>)
Exemplo:
./servidor 5000 //Servidor Multiplas Requisições
./servidor 80 //Página HTML é gerada no diretório

Clientes (./cliente <host_servidor> <porta_servidor>)
Exemplo:
./cliente <localhost ou 127.0.0.1> 5000

######################### Arquivos ###################################
Arquivos de programa:
servidor.c: Contém o codigo do servidor
cliente.c: Contém o codigo do cliente
io.c: Realiza a comunicação do programa com o ambiente
io.h: Contém as declarações das funções e bibliotecas utilizadas em io.c

Arquivos gerados:
index.html: navegador de gerencia do servidor
intervalos_atuais.txt: contém os intervalos sendo procurados no momento da visualização da pagina HTML
intervalos_procurados.txt: contém os intervalos já procurados
intervalos.txt: grava os intervalos já testados