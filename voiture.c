#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "constantes.h"
//#include <windows.h>
#include "circuit.h"
#include "secteurs.h"
#include "voiture.h"
#include "constantes.h"

// définition des variables d'environnements
struct voiture maVoiture;
char *shm_ptr;
//définition des différentes méthodes

void readMemory(int semid);

// Méthode principale : simule le fonctionnement d'une voiture de course
int main(int argc, char* argv[]){

  shm_ptr = (char*) shmat(atoi(argv[1]), NULL, 0);

  essaiLibreQuali(P1);

  while (mavoiture.ready == 0) {
    sleep 0.5;
  }

  essaiLibreQuali(P2);

  while (mavoiture.ready == 0) {
    sleep 0.5;
  }

  essaiLibreQuali(P3);

  while (mavoiture.ready == 0) {
    sleep 0.5;
  }

  if(maVoiture.classement >= 15  )
}

//permet d'accéder à la mémoire principale

void readMemory(int semid){

  char *shm_ptr = (char*) shmat(semid, NULL, 0);
  printf("%s\n", "j'ai lu :");

  while (*shm_ptr != '\0') {
        printf("%c", *shm_ptr);
        shm_ptr++;
  }
  printf("\n");
}
