#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saveLoad.h"
#include "voiture.h"
#include "constantes.h"

/**
*@param Id du tour Essai, nombre de voiture, temps du tour d'essai
*
*Ecriture des resultats du tour d'essai dans un fichier .txt vierge,
*
*
*/
void saveEssai(int p, int chrono, voiture **classement)//Module de sauvegarde de partie
{
	char bufferTemps[50];
  char bufferSaveName[50];
	char bufferID[3];
	char number[2];

  sprintf(number, "%d", p);

  strcpy(bufferSaveName, "F1_essai");
  strcat(bufferSaveName, number);
  strcat(bufferSaveName, "_save.txt");

    int i;
    //sauvegarde de fichier
		FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
    fichier = fopen(bufferSaveName, "w");//nom du fichier
    if (fichier != NULL)
    {
        //ecriture dans le fichier
			fprintf(fichier, "classement des meilleurs temps de la periode d'essai %d\n",p);
			fprintf(fichier, "temps du tour d'essai %d\n",chrono);
      for(i=0;i<NOMBRE_DE_VOITURE;i++)//reutilisation du "i" pour economie memoire
          {

							sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
							sprintf(bufferID, "%d", classement[i]->id);
              fprintf(fichier, "Voiture %s : %s\n",bufferID, bufferTemps);
          }
      fclose(fichier);//fermeture du fichier
    }
}

/**
*@param Id du tour Essai, nombre de voiture
*
*Ecriture des resultats de la sceance de qualif dans un fichier .txt vierge,
*
*
*/
void saveQuali(int p, voiture **classement)//Module de sauvegarde de partie
{
	char bufferTemps[50];
  char bufferSaveName[50];
	char bufferID[3];
	char number[2];

	sprintf(number, "%d", p);

  strcpy(bufferSaveName, "F1_quali");
  strcat(bufferSaveName, number);
  strcat(bufferSaveName, "_save.txt");


  int i;
  //sauvegarde de fichier
  FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
  fichier = fopen(bufferSaveName, "w");//nom du fichier
  if (fichier != NULL)
  {
      //ecriture dans le fichier
	fprintf(fichier, "classement des meilleurs temps de la periode de qualification %d",p);
      for(i=0;i<NOMBRE_DE_VOITURE;i++)//reutilisation du "i" pour economie memoire
          {
			sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
			sprintf(bufferID, "%d", classement[i]->id);
              fprintf(fichier, "%s : %s\n",bufferID, bufferTemps);
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
void saveCourse(voiture **classement)//Module de sauvegarde de partie
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
		fprintf(fichier, "classement des meilleurs temps de la course\n");
        for(i=0;i<NOMBRE_DE_VOITURE;i++)//reutilisation du "i" pour economie memoire
            {
				sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
				sprintf(bufferID, "%d", classement[i]->id);
                fprintf(fichier, "%s : %s²\n",bufferID, bufferTemps);
            }
        fclose(fichier);//fermeture du fichier
    }
}

/*char *concatBuffer(char *first, int secund, char *third){
	char *bufferSaveName = (char *) calloc();
	char number[2];

	sprintf(number, "%d", p);

  strcpy(bufferSaveName, "F1_quali");
  strcat(bufferSaveName, number);
  strcat(bufferSaveName, "_save.txt");



}*/
