#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//#include <windows.h>
#include "circuit.h"
#include "secteurs.h"
#include "voiture.h"
#include "constantes.h"

// définition des variables d'environnements
int nombreFiliale;
voiture *shm;
//définition des différentes méthodes

void initVoiture();

// Méthode principale : simule le fonctionnement d'une voiture de course
int main(int argc, char* argv[]){

  printf("Mon nombre filiale est : %s\n", argv[2]);
  //initialisation des différentes variables
  nombreFiliale = atoi(argv[2]);
  shm = (voiture*) shmat(atoi(argv[1]), NULL, 0);
  initVoiture();

  sleep(3);
  printf("essais en cours \n");

  essaiLibreQuali(P1, &shm[nombreFiliale]);

  shm[nombreFiliale].status = -1;

  exit(EXIT_SUCCESS);
}

//permet d'accéder à la mémoire principale

void initVoiture(){

  shm[nombreFiliale].id = VOITURE_NUMBER[nombreFiliale];
  printf("essais\n");
  shm[nombreFiliale].tempSecteur1 = 0.0;
  printf("essais\n");
  shm[nombreFiliale].tempSecteur2 = 0.0;
  printf("essais\n");
  shm[nombreFiliale].tempSecteur3 = 0.0;
  printf("essais\n");
  shm[nombreFiliale].classement = nombreFiliale+1;
  shm[nombreFiliale].status = 0;
  shm[nombreFiliale].ready = TRUE;

  printf("init fini id : %d, temps1 %f, temps2 %f, temps 3 : %f, classement : %d \n", shm[nombreFiliale].id, shm[nombreFiliale].tempSecteur1, shm[nombreFiliale].tempSecteur2, shm[nombreFiliale].tempSecteur3, shm[nombreFiliale].status);

}
