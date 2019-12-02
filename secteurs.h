#ifndef SECTEURS_H_INCLUDED
#define SECTEURS_H_INCLUDED


// generer un PRNG entre Min & Max


/**
*@param int min, int max
*min doit �tre plus grand que max
*@return int c
*nombre generer de maniere pseudo aleatoire
*/
int my_rand(int min, int max);

//temps d'un secteur
/**
*@param int taille1, int taille2
*taille1 doit �tre plus grand que taille2
*@return int temp
*temps que fait une voiture sur un secteur
*/
int secteur(int taille1, int taille2);

/**
*@return int
* return true(1) si il est plus petit que 49 sinon false(0)
*/
int crash();

/**
*@return int
* return true(1) si il est egal a 1 sinon false(0)
*/
int stand();


#endif // SECTEURS_H_INCLUDED
