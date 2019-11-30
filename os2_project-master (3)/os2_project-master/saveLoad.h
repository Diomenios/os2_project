#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED
#include "voiture.h"

void saveEssai(int p, int chrono, voiture **classement);//Module de sauvegarde de partie
void saveQuali(int p, voiture **classement);
void saveCourse(voiture **classement);

#endif // SAVELOAD_H_INCLUDED

/**
*@param Id du tour Essai, nombre de voiture, temps du tour d'essai
*
*Ecriture des resultats du tour d'essai dans un fichier .txt vierge,
*
*

*/
/**
*@param Id du tour Essai, nombre de voiture
*
*Ecriture des resultats de la sceance de qualif dans un fichier .txt vierge,
*
*
*/
/*void saveQuali(int p, int voiture)//Module de sauvegarde de partie
{
	char bufferTemps[50];
	char bufferID[3];
    int i;
    //sauvegarde de fichier
    FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
    fichier = fopen("F1_quali"+p+"_save.txt", "w");//nom du fichier
    if (fichier != NULL)
    {
        //ecriture dans le fichier
		fprintf("classement des meilleurs temps de la periode de qualification %d",p);
        for(i=0;i<voiture;i++)//reutilisation du "i" pour economie memoire
            {
				sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
				sprintf(bufferID, "%d", classement[i]->id);
                fprintf("%d : %d\n",bufferID, bufferTemps);
            }
        fclose(fichier);//fermeture du fichier
    }
}
/**
*@param nombre de voiture
*
*Ecriture des resultats de la course dans un fichier .txt vierge,
*
*
*/
/*void saveCourse(int voiture)//Module de sauvegarde de partie
{
    char bufferTemps[50];
	char bufferID[3];
    int i;
    //sauvegarde de fichier
    FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
    fichier = fopen("F1_course_save.txt", "w");//nom du fichier
    if (fichier != NULL)
    {
        //ecriture dans le fichier
		fprintf("classement des meilleurs temps de la course\n");
        for(i=0;i<voiture;i++)//reutilisation du "i" pour economie memoire
            {
				sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
				sprintf(bufferID, "%d", classement[i]->id);
                fprintf("%d : %d\n",bufferID, bufferTemps);
            }
        fclose(fichier);//fermeture du fichier
    }
}
*/
