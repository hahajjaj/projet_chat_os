#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signal) {
    if (signal == SIGINT) {
        printf("Fermeture du serveur\n");
        exit(0);
    }
    return;
}

struct Message
{
  char pseudo[1024];
  time_t date_heure;
  size_t size_message;
  char message[1024];
  
};

int _checked(int ret, char* calling_function) {
  if (ret < 0) {
    perror(calling_function);
    exit(EXIT_FAILURE);
  }
  return ret;
}

// The macro allows us to retrieve the name of the calling function
#define checked(call) _checked(call, #call)

/**
 * @brief Send data under the form <size_t len><...data>
 * Function name is 'ssend' instead of 'send' because the latter already exists.
 */
int ssend(int sock, void* data, size_t len) {
  checked(write(sock, &len, sizeof(len)));
  return checked(write(sock, data, len));
}

/**
 * @brief Receive data under the form <size_t len><data...>.
 */
size_t receive(int sock, void** dest) {
  size_t nbytes_to_receive;
  if (checked(read(sock, &nbytes_to_receive, sizeof(nbytes_to_receive))) == 0) {
    // Connection closed
    return 0;
  };
  unsigned char* buffer = malloc(nbytes_to_receive);
  if (buffer == NULL) {
    fprintf(stderr, "malloc could not allocate %zd bytes", nbytes_to_receive);
    perror("");
    exit(1);
  }
  size_t total_received = 0;
  size_t received = 1;
  while (nbytes_to_receive > 0 && received > 0) {
    received = checked(read(sock, &buffer[total_received], nbytes_to_receive));
    total_received += received;
    nbytes_to_receive -= received;
  }
  *dest = buffer;
  return total_received;
}

#endif  // __COMMON_H
