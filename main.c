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
#include "saveLoad.h"
//#include <windows.h>

void initFork(int incr,char *semid) ;
int readMemory(int nombreEnfants);
int mycmp(const void *s1, const void *s2);
void save(int compteur);
int compare();
int finish();

voiture *shm;
voiture copieMemoire[NOMBRE_DE_VOITURE];
voiture *classement[NOMBRE_DE_VOITURE];
int periode[] = {P1, P2, P3};
tuple meilleursTemps[3];
int compteurDeCourses;

/*
*  Le premier paramètre permet de savoir le nombre de kilomètre que fait la partie S1
*  Le deuxième paramètre permet de savoir le nombre de kilomètre que fait la partie S2
*  Le troisième paramètre permet de savoir le nombre de kilomètre que fait la parti e S3
*/
int main(int argc, char *argv[]){
  //qsort(tab, sizeof(tab)/sizeof(*tab), sizeof(*tab), mycmp);

  printf("%s %f\n", "temps d'attente : ", TEMPS_ATTENTE);

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

  compteurDeCourses = 1;
  while (readMemory(NOMBRE_DE_VOITURE)) {
      sleep(1);
  }

  exit(EXIT_SUCCESS);
}

/**********************************  fonctions auxiliaires  ******************************************/

int readMemory(int nombreEnfants){

  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);
  int sorting = FALSE;
  int saveStatus = TRUE;
  int finishStatus = TRUE;

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
    if (finishStatus == TRUE && copieMemoire[i].status != -1) {
      finishStatus = FALSE;
    }
  }
  if (sorting) {
    qsort(classement, sizeof(classement)/sizeof(*classement), sizeof(*classement), mycmp);
  }
  afficherTableauScore(classement, compteurDeCourses);
  if (saveStatus && finishStatus) {
    save(compteurDeCourses);
    return FALSE;
  }
  if (saveStatus) {
    system("clear");
    save(compteurDeCourses);
    for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
      printf("%s\n", "réveil");
      shm[i].ready = TRUE;
    }
    compteurDeCourses++;
    return TRUE;
  }
  return TRUE;
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

//  TODO : Finir cette fonction
void save(int compteur){
  if (compteur == 7) {
    /* course principale */
  }
  else if (compteur>3) {
    /* qualification */
  }
  else{
    //TODO : retirer le if
    printf("%s\n", "j'ai sauvé");
    sleep(2);
    saveEssai(compteur, periode[compteur-1], classement);
    if (compteur == 3) {
      exit(EXIT_SUCCESS);
    }
  }
}

int compare(){
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {

  }
  printf("%s\n", "sauvagarde");
  return TRUE;
}

int finish(){
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    if (classement[i]->status != -1) {
      return TRUE;
    }
  }
  printf("%s\n", "fin du programme");
  return FALSE;
}
