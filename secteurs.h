#ifndef SECTEURS_H_INCLUDED
#define SECTEURS_H_INCLUDED
#define true 1
#define false 0


#endif // SECTEURS_H_INCLUDED

// generer un PRNG entre Min & Max
int my_rand(int min, int max){
  int c = rand()%(max-min+1)+min;//creation du nombre aleatoire
  return c;
}

//temps d'un secteur
int secteur(int taille1, int taille2){
    int temp = 0;
    if(crash()== false){
        temp = my_rand(taille1, taille2);//Generation du nombre aleatoire
        return temp;
    }
    return temp;
}


int crash(){
  if(my_rand(1,5000) < 49){
        return true;
  }
  return false;
}
