// Client side C/C++ program to demonstrate Socket programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../common.h"

char* normalize_message(char* message_final, char* pseudo_param, char* buffer_param){
  char* espace = " : ";
  size_t ln = strlen(pseudo_param)-1;
  if (pseudo_param[ln] == '\n')
      pseudo_param[ln] = '\0';
  ln = strlen(buffer_param)-1;
  if (buffer_param[ln] == '\n')
      buffer_param[ln] = '\0';
  strcat(message_final, pseudo_param);
  strcat(message_final , espace);
  strcat(message_final, buffer_param);
  return message_final;
}

int main(int argc, char const *argv[]) {
  printf("quel est votre pseudo ?");
  char pseudo[1024];
  
  fgets(pseudo, 1024,  stdin);
  size_t ln = strlen(pseudo)-1;
  if (pseudo[ln] == '\n')
      pseudo[ln] = '\0';
  int sock = checked(socket(AF_INET, SOCK_STREAM, 0));
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);

  // Conversion de string vers IPv4 ou IPv6 en binaire
  checked(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr));

  checked(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));
  char buffer[1024];
  ssize_t nbytes = 1;
  while (nbytes > 0 && fgets(buffer, 1024, stdin)) {
    char message[1024]="";
    // Supprimer le \n
    strcpy(message,normalize_message(message, pseudo, buffer));
    size_t len = strlen(message);
    
    // On garde la même taille de string pour explicitement envoyer le '\0'
    nbytes = ssend(sock, message, len);
    if (nbytes > 0) {
      char *recvbuffer;
      nbytes = receive(sock, (void *)&recvbuffer);
      if (nbytes > 0) {
        printf("Client received %s\n", recvbuffer);
        free(recvbuffer);
      }
    }
  }
  return 0;
}
