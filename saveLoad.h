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
*	@param gagnant* secteurs		les trois voitures ayant ete les plus rapides dans un secteur
* @param int nombreDeVoitures	nombre de voitures dans la course
*
*/
void saveEssai(int p, int chrono, voiture **classement, gagnant *secteurs, int nombreDeVoitures);//Module de sauvegarde de partie

/** ecriture des resultats de la sceance de qualif dans un fichier .txt vierge
*
* @param tuple** classement    pointeur de pointeurs permettant d'acceder a la liste des tuples
* 													   contenant les voitures classee des qualifications venant
*														   de se terminer
*	@param gagnant* secteurs	   les trois voitures ayant ete les plus rapides dans un secteur
* @param int nombreDeVoitures	 nombre de voitures dans la course
*
*/
void saveQuali(tuple **classement, gagnant *secteurs, int nombreDeVoitures);

/**ecriture des resultats de la course dans un fichier .txt vierge
*
* @param voiture** classement pointeur de pointeurs permettant d'acceder aux voitures
*															triees de la course venant de se terminer
*	@param gagnant* secteurs		les trois voitures ayant ete les plus rapides dans un secteur
*	@param int meilleurIdTemps	le meilleur temps pour faire un tour de circuit
*	@param int meilleurId				le numero de la voiture ayant fait le meilleur temps pour un tour
*															de circuit
* @param int nombreDeVoitures	nombre de voitures dans la course
*
*/
void saveCourse(voiture **classement, gagnant *secteurs, int meilleurTemps, int meilleurId, int nombreDeVoitures);

/**chargement des resultats de la course depuis un fichier .txt deja rempli
*
* @param char* file            nom du fichier a charger
* @param int nombreDeVoitures	 nombre de voitures dans la course
*
* @return renvoie un pointeur  vers un tableau contenant les id des voitures contenues
*					dans le fichier
*/
int *loading(char *file, int nombreDeVoitures);

#endif // SAVELOAD_H_INCLUDED
