//
//  FileHelper.c
//  Lumiar-Final
//
//  Created by Denis on 21/06/2016.
//
//

#include "FileHelper.h"
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>

// getcurrentdir()
//      Retorna o diretorio corrente atual
//      Entrada: buffer para string e tamanho deste buffer
//      Saida: ponteiro para string

char* getCurrentDir(char *path, int pathsize) {
    char *p;
    
    p = getcwd(path,pathsize);
    if (p == NULL)
        perror("Erro na chamada getcwd");
    return(p);
}

int composePath(char *oldpath,char *path,char *newpath) {
    strcpy(newpath, oldpath);
    strcpy(&newpath[strlen(oldpath)], path);
    return 0;
}

// transferfile()
//    Realiza a leitura do conteudo de um arquivo, identificado
//    por seu caminho (path), transferindo seu conteudo para
//    outro arquivo ou socket identificado pelo descritor "outfd"

int transferFile(char *path, int output_fd, char *msgbuffer) {
    int          input_fd;     // input file descriptor
    int          status;
    long          n;
    char         buffer[BUFFERSIZE];
    char         str[32];
    struct stat  statp;
    
    input_fd = open(path,O_RDONLY);
    if (input_fd < 0)
    {
        perror("ERRO chamada open(): Erro na abertura do arquivo: ");
        return(-1);
    }
    
    // obtem tamanho do arquivo
    status = fstat(input_fd,&statp);
    if (status != 0)
    {
        perror("ERRO chamada stat(): Erro no acesso ao arquivo: ");
        status = close(input_fd);
        return(-1);
    }
    //sprintf(str, "SIZE=%lld\n", statp.st_size);
    
    
    sprintf(msgbuffer, "%s%s%lld\r\n\r\n", msgbuffer, "Content-Length: ", statp.st_size);
    status = write(output_fd, msgbuffer, strlen(msgbuffer)+1);
    
//    write(output_fd,str,strlen(str));
    
    // le arquivo , por partes
    do
    {
        n = read(input_fd,buffer,BUFFERSIZE);
        if (n<0)
        {
            perror("ERRO: chamada read(): Erro na leitura do arquivo: ");
            status = close(input_fd);
            return(-1);
        }
        write(output_fd,buffer,n);
    }
    while(n>0);
    
    status = close(input_fd);
    if (status == -1)
	   {
           perror("ERRO: chamada close(): Erro no fechamento do arquivo: " );
           return(-1);
       }
    return(0);
}
