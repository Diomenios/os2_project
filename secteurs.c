#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"
#include "secteurs.h"


// generer un PRNG entre Min & Max


/**
*@param int min, int max
*min doit �tre plus grand que max
*@return int c
*nombre generer de maniere pseudo aleatoire
*/
int my_rand(int min, int max){
  int c = rand()%(max-min+1)+min;//creation du nombre aleatoire
  return c;
}

//temps d'un secteur
/**
*@param int taille1, int taille2
*taille1 doit �tre plus grand que taille2
*@return int temp
*temps que fait une voiture sur un secteur
*/
int secteur(int taille1, int taille2){

    int temp = 0;

    if(crash()== FALSE){//si il n'y a pas de crash
        temp = my_rand(taille1, taille2);//Generation du nombre aleatoire
        return temp;
    }
    return temp;//si il y a un crash
}

/**
*@return int
* return true(1) si il est plus petit que 49 sinon false(0)
*/
int crash(){//methode pour determiner si il y a un crash
  return FALSE;

  if(my_rand(1,5000) <= 49){
        return TRUE;
  }
}

int stand(){//passage au stand

    if(my_rand(1,20) >= 1){//1 chance sur 10 d'aller au stand
        return TRUE;
  }
  return FALSE;

}
