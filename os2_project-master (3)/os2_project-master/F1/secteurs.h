#ifndef SECTEURS_H_INCLUDED
#define SECTEURS_H_INCLUDED
#define true 1
#define false 0


#endif // SECTEURS_Hhttps://github.cohttps://github.com/Diomenios/os2_project.gitm/Diomenios/os2_project.git_INCLUDED

// generer un PRNG entre Min & Max


/**
*@param int min, int max
*min doit être plus grand que max
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
*taille1 doit être plus grand que taille2
*@return int temp
*temps que fait une voiture sur un secteur
*/
int secteur(int taille1, int taille2){

    int temp = 0;

    if(crash()== false){//si il n'y a pas de crash
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

  if(my_rand(1,5000) <= 49){
        return true;
  }
  return false;
}
int stand(){
    if(my_rand(1,10) >= 1){
        return true;
  }
  return false;

}
