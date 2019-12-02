#ifndef SECTEURS_H_INCLUDED
#define SECTEURS_H_INCLUDED


// generer un PRNG entre Min & Max


/**
* nombre generer de maniere pseudo aleatoire
* ATTENTION : il faut utiliser une seed sur rand() prealablement pour eviter que
* les voitures sortent toujours les memes nombres au meme moment
*
* @param int min
* @param int max
* NB : min doit etre plus grand que max
*
* @return int c
*/
int my_rand(int min, int max);

/**
* temps que fait une voiture sur un secteur
*
*@param int taille1, int taille2
*taille1 doit etre plus grand que taille2
*@return int temp
*/
int secteur(int taille1, int taille2);

/**
* return true(1) si il est plus petit que 49 sinon false(0)
*
* @return int
*/
int crash();

int stand();


#endif // SECTEURS_H_INCLUDED
