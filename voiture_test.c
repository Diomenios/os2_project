#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "constantes.h"
#include "voiture.h"
//#include <windows.h>

int semid;
voiture *maVoiture;


int main(int argc, char* argv[]){

  printf("%s\n", "prout je marche");

  printf("%s\n", argv[1]);
  printf("%s\n", argv[2]);

  sleep(1);

  semid = atoi(argv[1]);
  maVoiture = (voiture*) shmat(semid, NULL, 0);

  for (int i = atoi(argv[2])+1; i < 10+atoi(argv[2]) ; i++) {

      maVoiture->classement = i;
      sleep(1);
  }
}
