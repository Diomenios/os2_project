#ifndef SECTEURS_H_INCLUDED
#define SECTEURS_H_INCLUDED


// generer un PRNG entre Min & Max

/*
* ce header permet de faire appel aux fonctions du fichier secteurs.c
*
*/

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

/** temps que fait une voiture sur un secteur
*
* @param int taille1, int taille2
* @param int chance   la probabilite d'avoir un crash
*
*N.B. : taille1 doit etre plus grand que taille2
*
*@return int temp
*/
int secteur(int taille1, int taille2, int chance);

/** permet de savoir si la voiture c'est crashee
*
* @param int chance   la probabilite d'avoir un crash
* return true(1) si il est plus petit que 49 sinon false(0)
*
* @return int retourne true(1) si il y a crash, sinon false(0)
*/
int crash(int chance);

/** permet de savoir si la voiture doit aller au stand
*
* @param int chance   la probabilite d'avoir un crash
*
* @return int retourne true(1) si il y a crash, sinon false(0)
*
*/
int stand(int chance);


#endif // SECTEURS_H_INCLUDED
