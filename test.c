#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>



int main(int argc, char const *argv[]) {
  char buf[150];
  time_t curtime;
  struct tm *loc_time;

  //temps actuel du systeme
  curtime = time (NULL);

  //convertie le temps systeme en date du jour
  loc_time = localtime (&curtime);

  //affiche la date
  printf("%s", asctime (loc_time));
  
  return 0;
}