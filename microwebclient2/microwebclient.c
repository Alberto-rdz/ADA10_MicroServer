/* Cliente del micro servidor web 2 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include "uici.c"

#define BLKSIZE  1024

typedef struct{
  int id;
  char nombre[20];
  float precio;
} Producto;



void usr1handler(int s)
{
   fprintf(stderr, "SIGUSR1 signal caught\n");
}
 
void usr2handler(int s)
{
   fprintf(stderr, "SIGUSR2 signal caught\n");
}
 
 
void installusrhandlers()
{
    struct  sigaction  newact;
    newact.sa_handler = usr1handler;      /* set the new usr1 handler */
    sigemptyset(&newact.sa_mask);    /* no additional signals blocked */
    newact.sa_flags = 0;            /* nothing special on the options */
    if (sigaction(SIGUSR1, &newact, (struct sigaction *)NULL) == -1) {
       perror("Could not install SIGUSR1 signal handler");
       return;
    }
    newact.sa_handler = usr2handler;      /* set the new usr2 handler */
    if (sigaction(SIGUSR2, &newact, (struct sigaction *)NULL) == -1) {
       perror("Could not install SIGUSR2 signal handler");
       return;
    }
    fprintf(stderr,
       "Client process %ld set to use SIGUSR1 and SIGUSR2\n",
       (long)getpid());
}
 
int main(int argc, char *argv[])
/*
 *  This is a client test of UICI. It opens a connection
 *  to a machine specified by host and a port number.
 *  It reads a file from stdin in blocks of size BLKSIZE
 *  and outputs it to the connection.
 */
{
    u_port_t portnumber;
    int outfd;
    ssize_t bytesread;
    ssize_t byteswritten;
    char buf[BLKSIZE];
       
    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port file\n", argv[0]);
        exit(1);
    }
       
    installusrhandlers();
 
    portnumber = (u_port_t)atoi(argv[2]);
 
    if ((outfd = u_connect(portnumber, argv[1])) < 0) {
       u_error("Unable to establish an Internet connection");
       exit(1);
    }
       
    fprintf(stderr, "Connection has been made to %s\n", argv[1]);
 
  //enviar una petición
  char header[] = "GET / HTTP/1.1\r\n\r\n";
  char header2[250];

  char html[100000] = {0}; 

   byteswritten = u_write(outfd, &header, sizeof(header) );

   send(outfd, header,strlen(header),0);

   //bytesread = u_read(outfd, &header2, sizeof(200) );
   bytesread = u_read(outfd, &html, sizeof(1000) );
	
   recv(outfd, header2, BUFSIZ, 0);
   recv(outfd, html, BUFSIZ, 0); 


     /* Escribir en pantalla */    
   printf("Encabezado: %s \n",header2);   
   printf("Contenido: %s \n",html);


     u_close(outfd);
     exit(0);
}



