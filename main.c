#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "constantes.h"
#include "voiture.h"
//#include <windows.h>

void initFork(int incr,char *semid) ;
void readMemory(int nombreEnfants);

voiture *shm;
/*
*  Le premier paramètre permet de savoir le nombre de kilomètre que fait la partie S1
*  Le deuxième paramètre permet de savoir le nombre de kilomètre que fait la partie S2
*  Le troisième paramètre permet de savoir le nombre de kilomètre que fait la parti e S3
*/
int main(int argc, char *argv[]){

  //initialisation de la mémoire partagée + rattachement
  int semid = shmget(IPC_PRIVATE, sizeof(voiture)*20, IPC_CREAT | 0666);
  shm = (voiture *) shmat(semid, NULL, 0);

  //convertis le semid en char* pour le passer aux fils
  char pass[sizeof(semid)];
  sprintf(pass, "%d", semid);

  //fork le nombre de fois spécifié
  initFork(1, pass);

  sleep(5);
  while (shm[0].status != -1) {
      readMemory(1);
      sleep(1);
  }

  exit(EXIT_SUCCESS);
}

void readMemory(int nombreEnfants){
  for (int i = 0; i < nombreEnfants; i++) {
    printf("tempSecteur1 : %f\n", shm[i].tempSecteur1);
    printf("tempSecteur2 : %f\n", shm[i].tempSecteur2);
    printf("tempSecteur3 : %f\n", shm[i].tempSecteur3);
    printf("status : %d\n", shm[i].status);
  }
}

void initFork(int incr,char *semid){

  for (int i = 0; i < incr; i++) {
    if (fork() == 0) {
      char pass[sizeof(i)];
      sprintf(pass, "%d", (i));
      char *voiture[]= {"./voiture",semid,pass,NULL};
      execvp(voiture[0], voiture);
    }
  }
}


// TODO : à faire en dernier
// public void dynamicTableGenerator(String title)
