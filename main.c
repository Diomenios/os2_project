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

int readMemory(int nombreEnfants, voiture *shm, int typeDeCourse);
int readQualifMemory(int nombreEnfants, voiture *shm, int *typeDeCourse, tuple **classementDuo, int size);
void initFork(int incr,char *semid, char *mode);
void save(int compteur);
int modeCourse(char *argument_entrer);
void redemarrerVoiture(tuple **classement, int nombreVoiture, int typeDeCourse, int offset);
int mycmp(const void *s1, const void *s2);
int mytuplecmp(const void *s1, const void *s2);
tuple * initTuple(voiture *local, voiture *memory);

voiture copieMemoire[NOMBRE_DE_VOITURE];
voiture *classement[NOMBRE_DE_VOITURE];
int periode[] = {P1, P2, P3};

/*
*   Le premier parametre permet de savoir le mode qu'on veut lancer :
*   P1 == essai 1, P2 == essai 2, P3 == essai 3, Q == qualification, Course == course finale
*   Le deuxieme parametre permet de savoir le nombre de kilometre que fait la partie S1
*   Le troisieme parametre permet de savoir le nombre de kilometre que fait la partie S2
*   Le quatrieme parametre permet de savoir le nombre de kilometre que fait la partie S3
*/
int main(int argc, char *argv[]){
  //TODO : retirer ce commentaire
  //qsort(tab, sizeof(tab)/sizeof(*tab), sizeof(*tab), mycmp);

  int typeDeCourse = modeCourse(argv[1]);
  if (typeDeCourse == -1) {
    perror("le paramètre rentré en premier au programme n'est pas conforme !");
    exit(EXIT_FAILURE);
  }

  //initialisation de la mémoire partagée + rattachement
  int semid = shmget(IPC_PRIVATE, sizeof(voiture)*NOMBRE_DE_VOITURE, IPC_CREAT | 0666);
  voiture *shm = (voiture *) shmat(semid, NULL, 0);



  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);

  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    classement[i] = &copieMemoire[i];
  }

  //convertis le semid en char* pour le passer aux fils
  //10 parce qu'un int n'est jamais plus grand qu'un nombre à 10 chiffes
  char pass[10];
  sprintf(pass, "%d", semid);

  //fork le nombre de fois spécifié
  initFork(NOMBRE_DE_VOITURE, pass, argv[1]);

  /****************************************************************************
  *                     gestion de la course en cours                         *
  ****************************************************************************/

  //le cas des courses d'essais 1, 2 ou 3
  if (typeDeCourse == 1 || typeDeCourse == 2 || typeDeCourse == 3) {
    while (readMemory(NOMBRE_DE_VOITURE, shm, typeDeCourse)) {
      sleep(1);
    }
    save(typeDeCourse);
  }
  //le cas des courses de qualification
  else if (typeDeCourse == 4){
    tuple *classementDuo[NOMBRE_DE_VOITURE];
    for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
      classementDuo[i] = initTuple(classement[i], &shm[i]);
      classementDuo[i]->local->id = i;
    }
    int size = sizeof(classementDuo);
    while(readQualifMemory(NOMBRE_DE_VOITURE, shm, &typeDeCourse, classementDuo, size)){
      sleep(1);
    }
    for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
      free(classementDuo[i]);
    }
  }
  //le cas des courses finales
  else {
    /*while (readMemory(NOMBRE_DE_VOITURE, shm, typeDeCourse)) {
      sleep(1);
    }*/
    int *listeVoiture = loading("F1_quali_save.txt");
    printf("%s\n", "lecture ok");
    for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
      printf("la voiture n° %d aura l'id : %d\n", i+1, listeVoiture[i]);
    }
  }

  shmdt(shm);
  exit(EXIT_SUCCESS);
}

/**********************************  fonctions auxiliaires  ******************************************/

int readMemory(int nombreEnfants, voiture *shm, int typeDeCourse){
  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);
  int sorting = FALSE;
  int saveStatus = TRUE;

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
  }

  if (sorting) {
    qsort(classement, sizeof(classement)/sizeof(*classement), sizeof(*classement), mycmp);
  }
  afficherTableauScore(classement, typeDeCourse);

  if (saveStatus) {
    system("clear");
    return FALSE;
  }
  return TRUE;
}

/*******************  QUALIF  *****************************/

int readQualifMemory(int nombreEnfants, voiture *shm, int *typeDeCourse, tuple **classementDuo, int size){
  int voiture_en_course;
  int sorting = FALSE;
  int saveStatus = TRUE;

  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
  }

/***              Q1            ****/
  if (*typeDeCourse == 4) {
    if (sorting) {
      qsort(classementDuo, size/sizeof(*classementDuo), sizeof(*classementDuo), mytuplecmp);
    }
    afficherTableauScoreQualif(classementDuo, *typeDeCourse);

    if (saveStatus) {
      system("clear");
      redemarrerVoiture(classementDuo, nombreEnfants, *typeDeCourse, 3);
      *typeDeCourse +=1;
      return TRUE;
    }
    return TRUE;
  }
/***            Q2 et Q3        ****/
  else if (*typeDeCourse == 5) {
    voiture_en_course = 6;
  }
  else{
    voiture_en_course = 3;
    printf("%s\n", "on est en Q3");
    sleep(1);
  }

  tuple *voiture_qualif[voiture_en_course];

  memcpy(voiture_qualif, classementDuo, sizeof(tuple*)*voiture_en_course);
  if (sorting) {
    qsort(voiture_qualif, sizeof(voiture_qualif)/sizeof(*voiture_qualif), sizeof(*voiture_qualif), mytuplecmp);
  }
  qsort(voiture_qualif, sizeof(voiture_qualif)/sizeof(*voiture_qualif), sizeof(*voiture_qualif), mytuplecmp);
  memcpy(classementDuo, voiture_qualif, sizeof(tuple*)*voiture_en_course);

  afficherTableauScoreQualif(classementDuo, *typeDeCourse);

  if (saveStatus && *typeDeCourse == 6) {
    system("clear");
    printf("%s\n", "save qualif");
    saveQuali(classementDuo);
    return FALSE;
  }
  if (saveStatus) {
    system("clear");
    redemarrerVoiture(classementDuo, nombreEnfants, *typeDeCourse, 3);
    *typeDeCourse +=1;
    return TRUE;
  }
  return TRUE;
}

void initFork(int incr,char *semid, char *mode){

  for (int i = 0; i < incr; i++) {
    if (fork() == 0) {
      char pass[sizeof(i)];
      sprintf(pass, "%d", i);
      char *voiture[]= {"./voiture",semid,pass,mode,NULL};
      execvp(voiture[0], voiture);
    }
  }
}

void save(int compteur){
  switch (compteur) {
    case 1:
      saveEssai(compteur, P1, classement);
      break;
    case 2:
      saveEssai(compteur, P2, classement);
      break;
    case 3:
      saveEssai(compteur, P3, classement);
      break;
    case 6:
      break;
    case 7:
      saveCourse(classement);
      break;
    default :
      printf("%s\n", "erreur dans la sauvegarde");
      exit(EXIT_FAILURE);
  }
}

int modeCourse(char *argument_entrer){
  if (strcmp(argument_entrer, "P1") == 0) {
    return 1;
  }
  if (strcmp(argument_entrer, "P2") == 0) {
    return 2;
  }
  if (strcmp(argument_entrer, "P3") == 0) {
    return 3;
  }
  if (strcmp(argument_entrer, "Q") == 0) {
    return 4;
  }
  if (strcmp(argument_entrer, "Course") == 0) {
    return 7;
  }
  else{
    return -1;
  }
}

void redemarrerVoiture(tuple **classement, int nombreVoiture, int typeDeCourse, int offset){
  if (typeDeCourse == 4) {
    for (int i = 0; i < nombreVoiture; i++) {
      classement[i]->memory->ready = TRUE;
      if (i>=nombreVoiture-offset) {
        classement[i]->memory->status = -1;
      }
    }
  }
  else{
    for (int i = 0; i < nombreVoiture-offset; i++) {
      classement[i]->memory->ready = TRUE;
      if (i>=nombreVoiture-2*offset) {
        classement[i]->memory->status = -1;
      }
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

int mytuplecmp(const void *s1, const void *s2){
  const tuple *l = *(const tuple **)s1;
  const tuple *r = *(const tuple **)s2;

  int mtl = l->local->meilleurTemps;
  int mtr = r->local->meilleurTemps;

  if (mtl < mtr) return -1;
  if (mtl > mtr) return 1;
  return 0;
}

tuple * initTuple(voiture *local, voiture *memory){

  tuple *new = (tuple*) malloc(sizeof(tuple));

  new->local = local;
  new->memory = memory;

  return new;
}
