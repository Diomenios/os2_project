#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <fcntl.h>           /* Pour les constantes O_* */
#include <limits.h>
#include "circuit.h"
#include "secteurs.h"
#include "voiture.h"
#include "constantes.h"
#include "loading_config.h"

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
/*
*
* Ce fichier .c est compile puis lance lorsqu'un processus fils est cree
* il fait appel a des headers pour utiliser les fonctions de voitures, du circuits, ...
* il simule une course selon les parametres entree avec une voiture
*
*/

/******************  definition des variables globales  ***********************/
int nombreFiliale;

/****************  definition des methodes du processus  **********************/
void initVoiture(int stat, int read, int id, voiture *shm);
void attenteQuali(voiture *shm, int id, sem_t *sem);


/** Methode principale : simule le fonctionnement d'une voiture de course
*   divisee en 3 parties distinctes :
*
*     - les periodes d'essais
*         lance la periode d'essais choisie grace au nombre passer en parametre
*         au lancement du processus : 1 pour la periode 1, 2 pour la periode 2,
*         3 pour la periode 3
*     - les periodes de qualifications
*         chiffre en parametre 4 : enchaine 3 sessions de qualifacation a la suite.
*         la methode attenteQuali permet de synchroniser les processus pour
*         les relancer a peu pres en meme temps
*     - la course principle
*         chiffre en parametre 7 : lance simplement la methode Course()
*
*/
int main(int argc, char* argv[]){
  srand(time(NULL) ^ (getpid()<<16));

  sem_t *sem;
  voiture *shm;
  data *programmeData;
  int id;
  //initialisation des differentes variables
  nombreFiliale = atoi(argv[2]);
  sem = sem_open(argv[5], O_CREAT);
  id = atoi(argv[6]);

  shm = (voiture*) shmat(atoi(argv[1]), NULL, 0);
  programmeData = (data *) shmat(atoi(argv[4]), NULL, 0);

  sem_wait(sem);
  initVoiture(2, 0, id, shm);
  sem_post(sem);
/********************  Les periodes d'essais ********************************/

  if (strcmp(argv[3], "p1") == 0) {
    essaiLibreQuali(programmeData->p1, &shm[nombreFiliale], sem, programmeData);
  }
  else if (strcmp(argv[3], "p2") == 0) {
    essaiLibreQuali(programmeData->p2, &shm[nombreFiliale], sem, programmeData);
  }
  else if (strcmp(argv[3], "p3") == 0) {
    essaiLibreQuali(programmeData->p3, &shm[nombreFiliale], sem, programmeData);
  }

/********************** Les periodes de qualification ***********************/

  else if (strcmp(argv[3], "q") == 0) {
    essaiLibreQuali(programmeData->q1, &shm[nombreFiliale], sem, programmeData);
    attenteQuali(shm, id, sem);

    essaiLibreQuali(programmeData->q2, &shm[nombreFiliale], sem, programmeData);
    attenteQuali(shm, id, sem);

    essaiLibreQuali(programmeData->q3, &shm[nombreFiliale], sem, programmeData);
  }

/************************** La course principale ****************************/

  else{
    Course(programmeData->toursCourse, &shm[nombreFiliale], sem, programmeData);
  }

  /*************************** Fin du programme *******************************/

  shm[nombreFiliale].status = 0;

  shmdt(shm);
  shmdt(programmeData);
  exit(EXIT_SUCCESS);
}

/** initialisation d'une voiture avec toutes ses variables
*
* @param int stat     initialise la variable status de la voiture
                      valeur autorisee {-1,0,1,2}
* @param int read     initialise la variable ready de la voiture
                      valeur autorisee {-1,0,1,2}
* @param int id       initialise la variable id de la voiture
* @param voiture* shm pointeur vers l'emplacement en memoire partagee de la voiture
*
* N.B. pour plus d'infos sur les differents parametre aller voir le fichier voiture.h
*/
void initVoiture(int stat, int read, int id, voiture *shm){

  shm[nombreFiliale].id = id;
  shm[nombreFiliale].tempSecteur1 = 0;
  shm[nombreFiliale].tempSecteur2 = 0;
  shm[nombreFiliale].tempSecteur3 = 0;
  shm[nombreFiliale].meilleurTemps = INT_MAX;
  shm[nombreFiliale].tempsTotal = 0;
  shm[nombreFiliale].tours = 1;
  shm[nombreFiliale].status = stat;
  shm[nombreFiliale].ready = read;
  shm[nombreFiliale].changeOrdre = FALSE;
  shm[nombreFiliale].crash = FALSE;
  shm[nombreFiliale].passageAuStand = 0;
}

/**permet a la voiture d'attendre que le processus pere finisse de synchroniser les
*  voitures avant de lancer une nouvelle periode de qualification
*
*  ATTENTION : zone d'ecriture a risque en memoire partagee, d'ou l'usage
*              d'un semaphore
*
* @param voiture* shm pointeur vers l'emplacement en memoire partagee de la voiture
* @param int id       numero de la voiture
* @param sem_t* sem   pointeur vers le semaphore utilise par la voiture
*
*/
void attenteQuali(voiture *shm, int id, sem_t *sem){
  while (shm[nombreFiliale].ready == -1) { //mise en attente de de la voirure en attendant les autres
    sleep(TEMPS_ATTENTE);
  }
  if (shm[nombreFiliale].status == 0 || shm[nombreFiliale].crash == TRUE) {//fin du processus
    sem_wait(sem);
    initVoiture(0, -1, id, shm);
    sem_post(sem);

    shmdt(shm);
    exit(EXIT_SUCCESS);
  }
  else{	//mettre voiture dans l'etat course
    sem_wait(sem);
    initVoiture(2, 0, id, shm);
    sem_post(sem);
  }
  return;
}
