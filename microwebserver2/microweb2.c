#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>


char *read_file(char *filename, char *outstr)
{
    char str[1024*1024] = {0};
    if (!filename || !outstr)
    {
        printf("input error\n");
        return NULL;
    }
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if(!fp)
    {
        return NULL;
    }
    while( fgets(str, 1024*1024, fp) )
    {
        strcat(outstr,str);
        memset(str, 0, 1024*1024);
    }

    fclose(fp);

    return outstr;
}

int main (int argc, char *argv[]) {

    int server_sockfd;
    int client_sockfd;
   int len, pid;
    struct sockaddr_in server_addr;   
    struct sockaddr_in remote_addr; 
    int sin_size;
    char buf[BUFSIZ];

  
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY; 



    server_addr.sin_port = htons(8888); 


    if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket failed\n");
        exit(0);
    }

   
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        printf("bind failed\n");
        exit(0);
    }


 
    listen(server_sockfd, 5);
    sin_size = sizeof(struct sockaddr_in);

   printf("En espera de conexiones..\n");

    while(1)
    {
      
        client_sockfd = accept(server_sockfd, (struct sockaddr *) &remote_addr, &sin_size);
       
       
        if (client_sockfd < 0)
        {
            perror("failed on accept");
        }

        pid = fork();
        if (pid < 0)
        {
            perror("failed on fork");
        }

        //El proceso hijo atiende la peticion web
        if (pid == 0)
        {
            close(server_sockfd);
            char header[] = "HTTP/1.0 200 OK\r\n\r\n";

            //crear arreglo del tamaño necesario
             //validar antes de leer
            char html[100000] = {0}; //antes 1024+1024*10
            

            recv(client_sockfd, buf, BUFSIZ, 0);
            printf("%s\n", buf);

           
            //leer el archivo enviado en el encabezado GET
            read_file("./datos.html", html);
            
            send(client_sockfd, header,strlen(header),0);
            send(client_sockfd, html, strlen(html), 0); 
           
            close(client_sockfd);
            exit(0);
        }

        if(pid>0){
          int status;
          //waitpid(pid, &status, 0);
          close(client_sockfd);
        }
    
    } 
    close(server_sockfd);



    return 0;
}
