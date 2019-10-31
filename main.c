#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <math.h>
#include "constantes.h"
#include "voiture.h"
#include "affichage.h"
//#include <windows.h>

void initFork(int incr,char *semid) ;
void readMemory(int nombreEnfants);
int mycmp(const void *s1, const void *s2);

voiture *shm;
voiture copieMemoire[NOMBRE_DE_VOITURE];
voiture *classement[NOMBRE_DE_VOITURE];
tuple meilleursTemps[3];

/*
*  Le premier paramètre permet de savoir le nombre de kilomètre que fait la partie S1
*  Le deuxième paramètre permet de savoir le nombre de kilomètre que fait la partie S2
*  Le troisième paramètre permet de savoir le nombre de kilomètre que fait la parti e S3
*/
int main(int argc, char *argv[]){
  //qsort(tab, sizeof(tab)/sizeof(*tab), sizeof(*tab), mycmp);

  //initialisation de la mémoire partagée + rattachement
  int semid = shmget(IPC_PRIVATE, sizeof(voiture)*NOMBRE_DE_VOITURE, IPC_CREAT | 0666);
  shm = (voiture *) shmat(semid, NULL, 0);

  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);

  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    classement[i] = &copieMemoire[i];
  }

  //convertis le semid en char* pour le passer aux fils
  //10 parce qu'un int n'est jamais plus grand qu'un nombre à 10 chiffes
  char pass[10];
  sprintf(pass, "%d", semid);

  //fork le nombre de fois spécifié
  initFork(NOMBRE_DE_VOITURE, pass);

  sleep(1);

  while (shm[0].status != -1 && shm[1].status != -1 ) {
      system("clear");
      readMemory(2);
      sleep(TEMPS_ATTENTE);
  }

  exit(EXIT_SUCCESS);
}

/**********************************  fonctions auxiliaires  ******************************************/

void readMemory(int nombreEnfants){

  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);
  int sorting = FALSE;
  if()
  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
  }
  if (sorting) {
    qsort(classement, sizeof(classement)/sizeof(*classement), sizeof(*classement), mycmp);
  }
  afficherTableauScore(classement);
  return;
}

void initFork(int incr,char *semid){

  for (int i = 0; i < incr; i++) {
    if (fork() == 0) {
      char pass[sizeof(i)];
      sprintf(pass, "%d", i);
      char *voiture[]= {"./voiture",semid,pass,NULL};
      execvp(voiture[0], voiture);
    }
  }
}

int mycmp(const void *s1, const void *s2) {
    const voiture *l = *(const voiture **)s1;
    const voiture *r = *(const voiture **)s2;

    int mtl = l->meilleurTemps;
    int mtr = r->meilleurTemps;

    if (mtl < mtr) return -1;
    if (mtl > mtr) return 1;
    return 0;
}
