#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
//#include <windows.h>
#include <sys/types.h>
#define true 1  //permettre l'utilisation du terme "true"
#define false 0 //permettre l'utilisation du terme "false"


void forkBomb(int decrem, pid_t cpid[]);

/*  argv[0] doit être le nombre de voitures lancées sur le circuit
*
*/
int max;

int main(int argc, char *argv[]){

  max = 5;
  //(int)*argv[0]
  pid_t cpid[5];

  forkBomb(5, cpid);

  sleep(2);

  exit(EXIT_SUCCESS);
}

void forkBomb(int decrem, pid_t cpid[]) {
  if (decrem == 0) {
    return ;
  }
   cpid[max-decrem] = fork();

   if(cpid[max-decrem] == -1){
     perror("fork");
     exit(EXIT_FAILURE);
   }

   //child process
   if (cpid[max-decrem] == 0) {
     printf("vous êtes dans la voiture numéro : %d \n", max-decrem);
     char *voiture[]= {"./voiture_test", NULL};
     execvp(voiture[0], voiture); // execute the command pass in argument
   }
   //parent process
   else{
     forkBomb(decrem-1, cpid);
   }

}
