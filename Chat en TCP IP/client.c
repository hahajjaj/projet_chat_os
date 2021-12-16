/*
Hamza Hajjaj 000461105
Safouan Ehlalouch 000514145

Projet de chat en TCP/IP pour le cours de système d'exploitation.

*/

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"


//Fonction qui permet de receptionner le message du serveur. Elle est appelé dans un thread
void* read_function(void* socket){
  int* sock = (int*)socket;
  struct tm *loc_time;
  struct Message *msg_receved;
  int fin_de_serveur;
  while (1){
    //reception du message ainsi que la reponse du ckeck qui teste l'ouverture du serveur.
    fin_de_serveur = receive(*sock, (void *)&msg_receved);
    //récupération du timestamp dans la structure du message.
    loc_time = localtime (&msg_receved->date_heure);

    //Teste si le serveur répond otujours ou non.
    if (fin_de_serveur == 0){
      printf("Connexion perdue avec le serveur.\n");
      exit(1);
    }

    //Affiche le timestamp sous forme lisible
    printf("%s", asctime(loc_time));
    //Affichage du pseudo ainsi que du message
    printf("%s : %s\n",msg_receved->pseudo, msg_receved->message);
    free(msg_receved);
    }
  }

int main(int argc, char const *argv[]) {
  if (argc == 4){  
    char *pseudo = (char *)argv[1];
    const char *ip = argv[2];
    const int port = atoi(argv[3]);

    struct Message msg;   // Structure de message
    ssize_t nbytes = 1;   // Permet de savoir si le serveur est lancé ou non.
    time_t timestamp;   // Permet de stocker le timestamp

    int sock = checked(socket(AF_INET, SOCK_STREAM, 0));
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Conversion de string vers IPv4 ou IPv6 en binaire
    checked(inet_pton(AF_INET, ip, &serv_addr.sin_addr));

    checked(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));

    //On envoi le pseudo après la connexion
    ssend(sock, pseudo, strlen(pseudo));

    //Ajout du pseudo dans la structure message
    strcpy(msg.pseudo, pseudo);

    pthread_t tids;
    pthread_create(&tids, NULL, read_function, &sock);
    while (nbytes > 0 && fgets(msg.message, 1024, stdin)) {
      // Supprimer le \n
      size_t len = strlen(msg.message);
      msg.message[len - 1] = '\0';

      msg.size_message = len;

      timestamp = time (NULL);   // Récupération du timestamp actuel
      msg.date_heure = timestamp;   // Ajout du timestamp dans la structure message
    
      nbytes = ssend(sock, &msg, sizeof(msg));   // Envoi du message au serveur
    }
  }

  else {
    printf("Veuillez appeler le script : ./client <pseudo> <ip> <port> \n");
  }

  printf("Fermeture du client\n");

  return 0;
}
