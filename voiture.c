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

void attente();

// définition des variables d'environnements
int nombreFiliale;
voiture *shm;
//définition des différentes méthodes

void initVoiture();

// Méthode principale : simule le fonctionnement d'une voiture de course
int main(int argc, char* argv[]){
  srand(time(NULL) ^ (getpid()<<16));

  //initialisation des différentes variables
  nombreFiliale = atoi(argv[2]);
  shm = (voiture*) shmat(atoi(argv[1]), NULL, 0);
  initVoiture();

  essaiLibreQuali(P1, &(shm[nombreFiliale]));
  attente();
  printf("%s\n", "debut essai-libre 2");

  essaiLibreQuali(P2, &shm[nombreFiliale]);
  attente();
  printf("%s\n", "debut essai-libre 3");

  essaiLibreQuali(P3, &shm[nombreFiliale]);
  attente();
  printf("%s\n", "fin essai-libre");

  //fin de la course, le fils indique au père qu'il a terminé, puis il s'arrête
  shm[nombreFiliale].status = -1;

  shmdt(shm);
  exit(EXIT_SUCCESS);
}

//permet d'accéder à la mémoire principale

void initVoiture(){

  shm[nombreFiliale].id = VOITURE_NUMBER[nombreFiliale];
  shm[nombreFiliale].tempSecteur1 = 0;
  shm[nombreFiliale].tempSecteur2 = 0;
  shm[nombreFiliale].tempSecteur3 = 0;
  shm[nombreFiliale].meilleurTemps = 0;
  shm[nombreFiliale].tours = 1;
  shm[nombreFiliale].status = 2;
  shm[nombreFiliale].ready = 0;
  shm[nombreFiliale].changeOrdre = FALSE;

  printf("init fini id : %d, temps1 %d, temps2 %d, temps 3 : %d, classement : %d \n", shm[nombreFiliale].id, shm[nombreFiliale].tempSecteur1, shm[nombreFiliale].tempSecteur2, shm[nombreFiliale].tempSecteur3, shm[nombreFiliale].status);

}

void attente(){
  while (shm[nombreFiliale].ready != TRUE) {
    sleep(TEMPS_ATTENTE);
  }
  return
}
