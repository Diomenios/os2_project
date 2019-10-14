#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "constantes.h"
#include "saveLoad.h"
//#include <windows.h>

#define true 1  //permettre l'utilisation du terme "true"
#define false 0 //permettre l'utilisation du terme "false"

void initFork(int incr,char *semid) ;
void initMemory(int semid,char *msg, int size);

char *shm;
/*
*  Le premier paramètre permet de savoir le nombre de kilomètre que fait la partie S1
*  Le deuxième paramètre permet de savoir le nombre de kilomètre que fait la partie S2
*  Le troisième paramètre permet de savoir le nombre de kilomètre
e que fait la parti e S3
*/
int main(int argc, char *argv[]){

char msg[] = "Bonjour Olivier, je suis passé par la MP";

int semid = shmget(IPC_PRIVATE, strlen(msg), IPC_CREAT | 0666);

initMemory(semid, msg, strlen(msg));

//convertis le semid en char* pour le passer aux fils
char pass[sizeof(semid)];
sprintf(pass, "%d", semid);

//fork le nombre de fois spécifié
initFork(2, pass);

sleep(5);

exit(EXIT_SUCCESS);
}

void initMemory(int semid,char *msg, int size){

  shm = (char *) shmat(semid, NULL, 0);

  memcpy(shm, msg, size);
}

void initFork(int incr,char *semid){

  for (int i = 0; i < incr; i++) {
    if (fork() == 0) {
      char *voiture[]= {"./voiture_test", semid, NULL};
      execvp(voiture[0], voiture);
    }
  }

}

// TODO : à faire en dernier
// public void dynamicTableGenerator(String title)
