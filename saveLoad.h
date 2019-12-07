#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED
#include "voiture.h"

/*
* ce header permet de faire appel aux fonctions du fichier .c
* les fonctions appeles servent a sauvegarder les resultats
* les fonctions font appel a des ressources qui viennent de "voiture.h"
*/

/**
*@param Id du tour Essai, nombre de voiture, temps du tour d'essai
*
*Ecriture des resultats du tour d'essai dans un fichier .txt vierge,
*/
void saveEssai(int p, int chrono, voiture **classement);//Module de sauvegarde de partie

/**
*@param Id du tour Essai, nombre de voiture
*
*Ecriture des resultats de la sceance de qualif dans un fichier .txt vierge,
*/
void saveQuali(tuple **classement);

/**
*@param nombre de voiture
*
*Ecriture des resultats de la course dans un fichier .txt vierge,
*/
void saveCourse(voiture **classement);

/**
*@param nom du fichier
*
*chargement des resultats de la course depuis un fichier .txt deja rempli
*/
int *loading(char *file);

#endif // SAVELOAD_H_INCLUDED
