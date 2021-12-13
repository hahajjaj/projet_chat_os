// Client side C/C++ program to demonstrate Socket programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"



struct Message
{
   size_t len;
   char *buffer;
   int fd;
   ssize_t *nbytes;
};


void* read_function(void* socket){
  while (1){
   int* sock = (int*)socket;
   char *recvbuffer;
   receive(*sock, (void *)&recvbuffer);
   printf("%s\n", recvbuffer);
   memset(&recvbuffer,0,  sizeof recvbuffer);
   free(recvbuffer);
   }
  }

int main(int argc, char const *argv[]) {
  if (argc == 4){  
     char *pseudo = (char *)argv[1];
     const char *ip = argv[2];
     const int port = atoi(argv[3]);
     char format[] = " : ";
     char pseudo_with_format[strlen(pseudo)+strlen(format)];
    strcpy(pseudo_with_format,pseudo);
    strcat(pseudo_with_format,format);

  int sock = checked(socket(AF_INET, SOCK_STREAM, 0));
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // Conversion de string vers IPv4 ou IPv6 en binaire
  checked(inet_pton(AF_INET, ip, &serv_addr.sin_addr));

  checked(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));

   //On envoi le pseudo après la connexion
  ssend(sock, pseudo, strlen(pseudo));

  char message[1024];
  char buffer[1024];
  ssize_t nbytes;
  nbytes = 1;

  pthread_t tids;
  pthread_create(&tids, NULL, read_function, &sock);
  while (nbytes > 0 && fgets(message, 1024, stdin)) {
    // Supprimer le \n
    size_t len = strlen(message);
    message[len - 1] = '\0';
    // On garde la même taille de string pour explicitement envoyer le '\0'
    strcpy(buffer,pseudo_with_format);
    
    strcat(buffer,message);
    size_t len_total = strlen(buffer);
    
    nbytes = ssend(sock, buffer, len_total);
  }}

  else {
    printf("Veuillez appeler le script : ./client <pseudo> <ip> <port> \n");
  }


  return 0;
}
