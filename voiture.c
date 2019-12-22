#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <fcntl.h>           /* Pour les constantes O_* */
#include "circuit.h"
#include "secteurs.h"
#include "voiture.h"
#include "constantes.h"

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
  int id;
  //initialisation des differentes variables
  nombreFiliale = atoi(argv[2]);
  id = atoi(argv[4]);
  sem = sem_open(argv[5], O_CREAT);

  shm = (voiture*) shmat(atoi(argv[1]), NULL, 0);

  sem_wait(sem);
  initVoiture(2, 0, id, shm);
  sem_post(sem);
/********************  Les periodes d'essais ********************************/

  if (strcmp(argv[3], "p  1") == 0) {
    essaiLibreQuali(P1, &shm[nombreFiliale], sem);
  }
  else if (strcmp(argv[3], "p2") == 0) {
    essaiLibreQuali(P2, &shm[nombreFiliale], sem);
  }
  else if (strcmp(argv[3], "p3") == 0) {
    essaiLibreQuali(P3, &shm[nombreFiliale], sem);
  }

/********************** Les periodes de qualification ***********************/

  else if (strcmp(argv[3], "q") == 0) {
    essaiLibreQuali(Q1, &shm[nombreFiliale], sem);
    attenteQuali(shm, id, sem);

    essaiLibreQuali(Q2, &shm[nombreFiliale], sem);
    attenteQuali(shm, id, sem);

    essaiLibreQuali(Q3, &shm[nombreFiliale], sem);
  }

/************************** La course principale ****************************/

  else{
    Course(TOURS_COURSE, &shm[nombreFiliale], sem);
  }

  /*************************** Fin du programme *******************************/

  shm[nombreFiliale].status = 0;

  shmdt(shm);
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
  shm[nombreFiliale].meilleurTemps = 0;
  shm[nombreFiliale].tempsTotal = 0;
  shm[nombreFiliale].tours = 1;
  shm[nombreFiliale].status = stat;
  shm[nombreFiliale].ready = read;
  shm[nombreFiliale].changeOrdre = FALSE;
  shm[nombreFiliale].crash = FALSE;
}

/**permet a la voiture d'attendre que le processus pere finisse de synchroniser les
*  voitures avant de lancer une nouvelle periode de qualification
*
* @param voiture* shm pointeur vers l'emplacement en memoire partagee de la voiture
* @param int id       numero de la voiture
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
