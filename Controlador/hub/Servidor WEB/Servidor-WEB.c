//
//  Servidor-WEB.c
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#include "Servidor-WEB.h"
#include "RequestHelper.h"

/*
 Funcionalidades a serem suportadas:
     Implementado em um thread
     Interface web realizada através da porta TCP 8080
     Requisições e respostas HTTP
         Versão: HTTP 1.0
         Métodos: HTTP GET
     Objetos
         Páginas HTML
         Arquivos texto
         Imagens formato JPEG (JPG)
 */

/*
 O servidor WEB deve apresentar uma página WEB
 contendo:
    - Os valores atuais dos parâmetros e variáveis relevantes (temperatura, luminosidade e intensidade do LED)
    - Interface para alteração dos valores dos parâmetros
 */



//##########################################################
// main
//##########################################################

void main_web() {
    int                sd;                 // socket descriptor
    int                newsd;              // novo socket descriptor
    int                status;             // estado
    unsigned long      size;               // tamanho
    char               buffer[BUFFERSIZE]; // buffer temporario para dados
    char               msg[MSGSIZE];       // buffer temporario para dados
    int                serverport;         // porta (formato host)
    char             * statusp;
    struct sockaddr_in serversocketaddr;   // end. socket (IP+porta) do servidor
    struct sockaddr_in clientaddr;         // end. socket (IP+porta) do cliente
    struct servent   * serventryp;         // host entry (p/ traducao nome<->ip)
    
    serversocketaddr.sin_family      = AF_INET;    //address family internet
    serversocketaddr.sin_addr.s_addr = INADDR_ANY; //todos ends. IPs do servidor
    
    //####################
    // servico/porta
    //####################
    
    // Requisita nome do servico
    //printf("SERVICENAME: ");
    //scanf("%s",buffer);
    
    // Verifica se string informada representa um inteiro
    //serverport = atoi(buffer); <==== UNCOMMENT THIS
    serverport = 8081;        // <==== COMMENT THIS
    if (serverport > 65535) {
        printf("Valor de porta invalido. \n");
        exit(1);
    }
    else if (serverport > 0)
        serversocketaddr.sin_port = htons((unsigned short int) serverport);
    else if (serverport <= 0) {
        // String informada nao e' um inteiro
        serventryp = getservbyname(buffer,"tcp");
        if (serventryp != NULL) {
            serversocketaddr.sin_port = serventryp->s_port;
        }
        else {
            printf("Nome do servico (ou porta) invalido. \n");
            exit(1);
        }
    }
    
    //####################
    // imprime IP e porta
    //####################
    statusp = inet_ntop(AF_INET,&serversocketaddr.sin_addr,buffer,STRINGSIZE);
    printf("Endereco IP do servidor = %s \n",buffer);
    printf("Porta do servidor = %hu \n",ntohs(serversocketaddr.sin_port));
    
    
    /****************************************************************/
    /* Socket(): Criacao do socket                                  */
    /****************************************************************/
    sd = socket(PF_INET,SOCK_STREAM,6); // 6="tcp"
    if (sd < 0) {
        printf("Erro na criação do socket. \n");
        perror("Descricao do erro");
        exit(1);
    }
    
    /****************************************************************/
    /* Bind(): associa o socket a um IP e a uma porta               */
    /****************************************************************/
    int iSetOption = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
               sizeof(iSetOption));
    
    status=bind(sd,(struct sockaddr *)&serversocketaddr,sizeof(struct sockaddr_in));
    if (status < 0) {
        perror("Erro na chamada bind()");
        exit(1);
    }
    
    /********************************************************************/
    /* listen(): Ativa o socket no modo passivo para aceite de conexoes */
    /********************************************************************/
    status = listen(sd,QLEN);
    if (status != 0)
    {
        perror("Erro na chamada listen()");
        exit(1);
    }
    
    /****************************************************************/
    /* accept(): aceita uma conexao da fila de conexoes pendentes   */
    /****************************************************************/
    while (1)
    {
        size = sizeof(clientaddr);
        newsd = accept(sd,(struct sockaddr *) &clientaddr,(socklen_t *) &size);
        if (newsd < 0) {
            perror("Erro na chamada accept()");
            exit(1);
        }
        
        signal(SIGPIPE, perdaconexao);
        
        /****************************************************************/
        /* read() & write():                */
        /****************************************************************/
        unsigned long len = read(newsd,buffer,BUFFERSIZE);
        if (len > 0) {
            buffer[len] = '\0';
        }
        status = len > 0;
        
        if (!status)
            perror("ERRO no recebimento de segmento TCP \n");
        
        printf("MENSAGEM RECEBIDA:\n%s\n------------------\n\n",buffer);
        
        RESPOSTA responseType = interpret(buffer, msg);
        
        // Obtém o tamanho do documento HTML
        size = strlen(msg) + 1;
        
        // Formata a mensagem HTTP com o documento HTML
        
        switch (responseType) {
            case R200:
                sprintf(buffer,"%s\r\n%s\r\n%s%lu\r\n%s\r\n\r\n%s",
                        "HTTP/1.0 200 OK",
                        "Server: MiniWebServer 1.0",
                        "Content-Length: ", size,
                        "Content-Type: text/html",
                        msg);
                break;
            case R404:
                sprintf(buffer,"%s\r\n%s\r\n%s%lu\r\n%s\r\n\r\n%s",
                        "HTTP/1.1 404 Not Found",
                        "Server: MiniWebServer 1.0",
                        "Content-Length: ", size,
                        "Content-Type: text/html",
                        msg);
                break;
            case JSON:
                sprintf(buffer, "%s\r\n%s\r\n%s%lu\r\n%s\r\n\r\n%s\r\n",
                        "HTTP/1.1 201 Created",
                        "Content-Type: application/json",
                        "Content-Length: ", size,
                        "Connection: close",
                        msg);
            case IMG:
                sprintf(buffer, "%s\r\n%s\r\n%s\r\n",
                        "HTTP/1.1 200 OK",
                        "Server: MiniWebServer 1.0\r\nConnection: Keep-Alive",
                        "Content-Type: image/jpeg");
                break;
            default:
                sprintf(buffer, "%s", "error");
                break;
        }         
        
        printf("MENSAGEM TRANSMITIDA:\n\n-------------\n");
        printf("%s\n",buffer);
        printf("-----------------------------------\n");
        
        switch (responseType) {
            case IMG:
                transferFile(msg, newsd, buffer);
                break;
            default:
                status = write(newsd,buffer,strlen(buffer)+1);
                if (status < 0)
                    printf("ERRO no envio de segmento TCP \n");
                strcpy(msg, "");
                break;
        }
        
        close(newsd);
    }        
}


void perdaconexao() {
    printf("Perda conexao \n");
}

