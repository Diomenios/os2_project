#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
//#include <windows.h>
#include "circuit.h"
#include "secteurs.h"
#include "voiture.h"
#include "constantes.h"

/*
* 
* Ce fichier .c est compile puis lance lorsqu'un processus fils est cree
* il fait appel a des headers pour utiliser les fonctions de voitures, du circuits, ...
* il simule une course selon les parametres entree avec une voiture 
* 
*/




// definition des variables d'environnements
int nombreFiliale;

//definition des differentes methodes
void initVoiture(int stat, int read, int id, voiture *shm);
void attenteQuali(voiture *shm, int id);

// Methode principale : simule le fonctionnement d'une voiture de course
int main(int argc, char* argv[]){
  srand(time(NULL) ^ (getpid()<<16));

  sem_t *sem;
  voiture *shm;
  //initialisation des differentes variables
  nombreFiliale = atoi(argv[2]);
  int id = atoi(argv[4]);

  shm = (voiture*) shmat(atoi(argv[1]), NULL, 0);
  initVoiture(2, 0, id, shm);

/********************  Les periodes d'essais ********************************/

  if (strcmp(argv[3], "P1") == 0) {
    essaiLibreQuali(P1, &shm[nombreFiliale]);
  }
  else if (strcmp(argv[3], "P2") == 0) {
    essaiLibreQuali(P2, &shm[nombreFiliale]);
  }
  else if (strcmp(argv[3], "P3") == 0) {
    essaiLibreQuali(P3, &shm[nombreFiliale]);
  }

/********************** Les periodes de qualification ***********************/

  else if (strcmp(argv[3], "Q") == 0) {
    essaiLibreQuali(Q1, &shm[nombreFiliale]);
    attenteQuali(shm, id);

    essaiLibreQuali(Q2, &shm[nombreFiliale]);
    attenteQuali(shm, id);

    essaiLibreQuali(Q3, &shm[nombreFiliale]);
  }

/************************** La course principale ****************************/

  else{
    Course(TOURS_COURSE, &shm[nombreFiliale]);
  }

  /*************************** Fin du programme *******************************/

  shm[nombreFiliale].status = -1;

  shmdt(shm);
  exit(EXIT_SUCCESS);
}

//permet d'acceder à la memoire principale
/**
*@param int stat, int read, int id, voiture *shm
*
* Initialisation d'une voiture avec toutes ses variables 
* 
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

  printf("init fini id : %d, temps1 %d, temps2 %d, temps 3 : %d, classement : %d \n", shm[nombreFiliale].id, shm[nombreFiliale].tempSecteur1, shm[nombreFiliale].tempSecteur2, shm[nombreFiliale].tempSecteur3, shm[nombreFiliale].status);

}

/**
*@param int stat, int read, int id, voiture *shm
*
* changement d'etat de la voitures entre chaques etapes 
* 
*/
void attenteQuali(voiture *shm, int id){
  while (shm[nombreFiliale].ready == -1) { //mise en attente de de la voirure en attendant les autres
    sleep(TEMPS_ATTENTE); 
  }
  if (shm[nombreFiliale].status == -1) {//fin du processus
    initVoiture(-1, -1, id, shm);
    shmdt(shm);
    printf("%s\n", "fin des qualif");
    exit(EXIT_SUCCESS);
  }
  else{	//mettre voiture dans l'etat course
    initVoiture(2, 0, id, shm);
    printf("%s\n", "gogogo");
  }
  return;
}
