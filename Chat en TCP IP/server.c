/*
Hamza Hajjaj 000461105
Safouan Ehlalouch 000514145

Projet de chat en TCP/IP pour le cours de système d'exploitation.

*/

#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h>
#include <sys/time.h> 
#include <unistd.h>
#include <signal.h>

#include "common.h"

int main(int argc, char *argv[]) {
  if (argc == 2){
    const int port = atoi(argv[1]);
    signal(SIGINT, handler);   // Création de la gestion de signal pour le ctrl-c
    int opt = 1;
    int master_socket = checked(socket(AF_INET, SOCK_STREAM, 0));
    checked(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)));

    // type of socket created
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    checked(bind(master_socket, (struct sockaddr *)&address, sizeof(address)));
    checked(listen(master_socket, 3));

    size_t addrlen = sizeof(address);

    fd_set readfds;
    int clients[1024];
    int nclients = 0;

    while (true) {
      FD_ZERO(&readfds);
      FD_SET(master_socket, &readfds);
      int max_fd = master_socket;
      for (int i = 0; i < nclients; i++) {
        FD_SET(clients[i], &readfds);
        if (clients[i] > max_fd) {
          max_fd = clients[i];
        }
      }
      // wait for an activity on one of the sockets, timeout is NULL
      select(max_fd + 1, &readfds, NULL, NULL, NULL);

      // Si c'est le master socket qui a des donnees, c'est une nouvele connexion.
      if (FD_ISSET(master_socket, &readfds)) {
        clients[nclients] = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        char *pseudo;
        receive(clients[nclients], (void *)&pseudo);
        printf("%s s'est connecté au chat\n", pseudo);
        nclients++;
      } else {
        // Sinon, c'est un message d'un client
        for (int i = 0; i < nclients; i++) {
          if (FD_ISSET(clients[i], &readfds)) {
            struct Message *msg_receved;
            size_t nbytes = receive(clients[i], (void *)&msg_receved);
            if (nbytes > 0) {  // closed
              // envoi du message à tout les clients qui se sont connectés
              for (int i = 0; i < nclients; i++) {
                ssend(clients[i], msg_receved, nbytes);
              }
            free(msg_receved);
            } else {
              close(clients[i]);
              // On deplace le dernier socket a la place de libre pur ne pas faire de trou.
              clients[i] = clients[nclients - 1];
              nclients--;
            }
          }
        }
      }
    }
  } else {
    printf("Veuillez appeler le script : ./serveur <port> \n");
  }

  return 0;
}