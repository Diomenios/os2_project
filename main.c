#include <stdio.h>
#include <stdlib.h>
#include "secteurs.h"//utilisation du header secteurs.h
#include "voiture.h"//utilisation du header voiture.h
#include "circuit.h" //utilisation du header circuit.h
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include <sys/types.h>
#define true 1  //permettre l'utilisation du terme "true"
#define false 0 //permettre l'utilisation du terme "false"





int main()
{
    srand(time(NULL));//initialisation PRNG
    printf("Bienvenue sur le projet C du groupe TEL (Tom-Emile-Louis)!\n");
    tour();//appel de la méthode tour
    return 0;
}

