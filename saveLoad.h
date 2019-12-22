#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED
#include "voiture.h"

/*
* ce header permet de faire appel aux fonctions du fichier .c
* les fonctions appeles servent a sauvegarder les resultats
* les fonctions font appel a des ressources qui viennent de "voiture.h"
*/

/** Ecriture des resultats du tour d'essai dans un fichier .txt vierge
*
* @param int p 								numero de la seance d'essai (1, 2 ou 3)
* @param int chrono						le temps qu'a dure l'essai
* @param voiture** classement pointeur de pointeurs permettant d'acceder aux voitures
*															triees de l'essai venant de se terminer
*
*/
void saveEssai(int p, int chrono, voiture **classement, gagnant *secteurs);//Module de sauvegarde de partie

/** ecriture des resultats de la sceance de qualif dans un fichier .txt vierge
*
* @param tuple** classement pointeur de pointeurs permettant d'acceder a la liste des tuples
* 													contenant les voitures classee des qualifications venant
*														de se terminer
*
*/
void saveQuali(tuple **classement, gagnant *secteurs);

/**ecriture des resultats de la course dans un fichier .txt vierge
*
* @param voiture** classement pointeur de pointeurs permettant d'acceder aux voitures
*															triees de la course venant de se terminer
*
*
*/
void saveCourse(voiture **classement, gagnant *secteurs, int meilleurTemps, int meilleurId);

/**chargement des resultats de la course depuis un fichier .txt deja rempli
*
* @param char* file nom du fichier a charger
*
* @return renvoie un pointeur vers un tableau contenant les id des voitures contenues
*					dans le fichier
*/
int *loading(char *file);

#endif // SAVELOAD_H_INCLUDED
