#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "constantes.h"
//#include <windows.h>

void readMemory(int semid);

int main(int argc, char* argv[]){

  printf("semid : %s \n", argv[1]);

  sleep(1);
  readMemory(atoi(argv[1]));
}

void readMemory(int semid){

  char *shm_ptr = (char*) shmat(semid, NULL, 0);
  printf("%s\n", "j'ai lu :");

  while (*shm_ptr != '\0') {
        printf("%c", *shm_ptr);
        shm_ptr++;
  }
  printf("\n");
}
