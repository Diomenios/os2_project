#ifndef SECTEURS_H_INCLUDED
#define SECTEURS_H_INCLUDED



#endif // SECTEURS_H_INCLUDED
int secteur(int taille1, int taille2){
    int temp = 0;
    temp = my_rand(taille1, taille2);//Generation du nombre aleatoire
    //printf("tour en %ds\n",temp);
    return temp;
}
// generer un PRNG entre Min et Max
int my_rand(int min, int max){
  int c = rand()%(max-min+1)+min;//creation du nombre aleatoire
  return c;
}
