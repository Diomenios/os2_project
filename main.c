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
//#include <windows.h>

void initFork(int incr,char *semid) ;
void readMemory(int nombreEnfants);
int mycmp(const void *s1, const void *s2);
void afficherTableauScore();
char* creationCelluleNombre(int tailleCellule, int input, int sizeInput);
char* decodageStatus(int status);

voiture *shm;
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
  int semid = shmget(IPC_PRIVATE, sizeof(voiture)*2, IPC_CREAT | 0666);
  shm = (voiture *) shmat(semid, NULL, 0);

  //initialise la liste de pointeur vers la structure en mémoire partagée
  //permettra de plus tard faire un tri pour les classer selon leur temps le plus rapide
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    classement[i] = &shm[i];
  }

  //convertis le semid en char* pour le passer aux fils
  //10 parce qu'un int n'est jamais plus grand qu'un nombre à 10 chiffes
  char pass[10];
  sprintf(pass, "%d", semid);

  //fork le nombre de fois spécifié
  initFork(NOMBRE_DE_VOITURE, pass);

  sleep(2);

  while (shm[0].status != -1 && shm[1].status != -1 ) {
      readMemory(2);
      sleep(1);
  }

  exit(EXIT_SUCCESS);
}

void readMemory(int nombreEnfants){
  for (int i = 0; i < nombreEnfants; i++) {
    if(shm[i].changeOrdre){
      shm[i].changeOrdre = FALSE;
      qsort(classement, sizeof(classement)/sizeof(*classement), sizeof(*classement), mycmp);
    }
  }

  afficherTableauScore();
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

    double mtl = l->meilleurTemps;
    double mtr = r->meilleurTemps;

    if (mtl < mtr) return -1;
    if (mtl > mtr) return 1;
    return 0;
}

void afficherTableauScore(){
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|-------|";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour | temps |";

  printf("%s\n", lineSeparator);
  printf("%s\n", titreTableau);
  printf("%s\n", lineSeparator);
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    char *recup;
    char *ligne = calloc(1, strlen(lineSeparator));
    strcat(ligne, "| voiture");

    recup = creationCelluleNombre(6, classement[i]->id,(int)floor(log10(classement[i]->id))+1);
    strcat(ligne, recup);
    free(recup);
    recup = NULL;

    recup = creationCelluleNombre(8 , classement[i]->tempSecteur1, (int)floor(log10(classement[i]->tempSecteur1))+1);
    strcat(ligne, recup);
    free(recup);

    recup = creationCelluleNombre(8, classement[i]->tempSecteur2, (int)floor(log10(classement[i]->tempSecteur2 ))+1);
    strcat(ligne, recup);
    free(recup);

    recup = creationCelluleNombre(8, classement[i]->tempSecteur3, (int)floor(log10(classement[i]->tempSecteur3))+1);
    strcat(ligne, recup);
    free(recup);

    strcat(ligne, decodageStatus(classement[i]->status));

    recup = creationCelluleNombre(7, classement[i]->tours, (int)floor(log10(classement[i]->tours))+1);
    strcat(ligne, recup);
    free(recup);

    recup = creationCelluleNombre(8, classement[i]->meilleurTemps, (int)floor(log10(classement[i]->meilleurTemps))+1);
    strcat(ligne, recup);
    free(recup);

    printf("%s\n", ligne);
    printf("%s\n", lineSeparator);

    free(ligne);
  }

}

//Attention cette méthode nécessite d'utiliser un free car elle est le résultat d'un malloc
char* creationCelluleNombre(int tailleCellule, int input, int sizeInput){
  if (input == 0) {
    char *cell = malloc(tailleCellule);
    strcat(cell, " NA");
    for (int i = 0; i < tailleCellule-4; i++) {
      strcat(cell," ");
    }
    strcat(cell, "|");

    return cell;
  }
  else{
    char pass[sizeInput];
    sprintf(pass, "%d", input);

    char *cell = malloc(tailleCellule);
    strcat(cell, " ");
    strcat(cell, pass);
    for (int i = 0; i < (tailleCellule-sizeInput-2); i++) {
      strcat(cell," ");
    }
    strcat(cell, "|");

    return cell;
  }
}

char* decodageStatus(int status){
  if (status == 0) {
    return " OUT    |";
  }
  if (status == 1) {
    return " P      |";
  }
  else{
    return " GO     |";
  }
}

// TODO : à faire en dernier
// public void dynamicTableGenerator(String title)
