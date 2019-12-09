#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "saveLoad.h"
#include "voiture.h"
#include "constantes.h"
#include "helper.h"

/** Ecriture des resultats du tour d'essai dans un fichier .txt vierge
*
* @param int p 								numero de la seance d'essai (1, 2 ou 3)
* @param int chrono						le temps qu'a dure l'essai
* @param voiture** classement pointeur de pointeurs permettant d'acceder aux voitures
*															triees de l'essai venant de se terminer
*
*/
void saveEssai(int p, int chrono, voiture **classement)//Module de sauvegarde de partie
{
	char* bufferTemps;
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
			fprintf(fichier, "temps de la période d'essai : %d\n",chrono);
      for(i=0;i<NOMBRE_DE_VOITURE;i++)//reutilisation du "i" pour economie memoire
          {
			
			//sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
			sprintf(bufferID, "%d", classement[i]->id);
			bufferTemps = convertion(classement[i]->meilleurTemps)
            fprintf(fichier, "Voiture %s : %s\n",bufferID, bufferTemps);
          }
      fclose(fichier);//fermeture du fichier
    }
}

/** ecriture des resultats de la sceance de qualif dans un fichier .txt vierge
*
* @param tuple** classement pointeur de pointeurs permettant d'acceder a la liste des tuples
* 													contenant les voitures classee des qualifications venant
*														de se terminer
*
*/
void saveQuali(tuple **classement)//Module de sauvegarde de partie
{
	char bufferTemps[50];
	char bufferID[3];

	int i;
	//sauvegarde de fichier
	FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
	fichier = fopen("F1_quali_save.txt", "w");//nom du fichier
	if (fichier != NULL){
      //ecriture dans le fichier
		fprintf(fichier, "<>", NULL);

		for (int i = 0; i <NOMBRE_DE_VOITURE-1; i++) {
			fprintf(fichier, "%d,", classement[i]->local->id);
		}
		fprintf(fichier, "%d", NOMBRE_DE_VOITURE-1);
		fprintf(fichier, "</>\n", NULL);

		fprintf(fichier, "classement des meilleurs temps des 3 période de qualification \n", NULL);
		for(i=0;i<NOMBRE_DE_VOITURE;i++)//reutilisation du "i" pour economie memoire
			{
				sprintf(bufferTemps, "voiture n° : %d", classement[i]->local->id);
				sprintf(bufferID, "%d", i+1);
				fprintf(fichier, "%s : %s\n",bufferID, bufferTemps);
			}
		fclose(fichier);//fermeture du fichier
	}
}

/**ecriture des resultats de la course dans un fichier .txt vierge
*
* @param voiture** classement pointeur de pointeurs permettant d'acceder aux voitures
*															triees de la course venant de se terminer
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
    if (fichier != NULL){
        //ecriture dans le fichier
		fprintf(fichier, "classement des meilleurs temps de la course\n");
        for(i=0;i<NOMBRE_DE_VOITURE;i++)//reutilisation du "i" pour economie memoire
            {
				sprintf(bufferTemps, "%d", classement[i]->tempsTotal);
				sprintf(bufferID, "%d", classement[i]->id);
                fprintf(fichier, "%s : %s²\n",bufferID, bufferTemps);
            }
        fclose(fichier);//fermeture du fichier
    }
}

/**chargement des resultats de la course depuis un fichier .txt deja rempli
*
* @param char* file nom du fichier a charger
*
* @return renvoie un pointeur vers un tableau contenant les id des voitures contenues
*					dans le fichier
*/
int *loading(char *file){

	int *voitureNombre = (int *) malloc(sizeof(int)*NOMBRE_DE_VOITURE);
	char buffer[50];

	FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
	fichier = fopen(file, "r");//nom du fichier
	if (fichier == NULL){
		*voitureNombre = -1;

		return voitureNombre;
	}
	fgets(buffer, sizeof(buffer), fichier);

	char *loop = buffer;
	int compteur = 0;
	while ( *loop != '\0') {
		if (*loop == '<') {
			if (*(loop+sizeof(char)) =='>') {
				loop +=2*sizeof(char);
			}
			else if (*(loop+sizeof(char)) =='/' && *(loop+2*sizeof(char)) =='>') {
				break;
			}
			else{
			}
		}
		else{
			char loopBuffer[5] = "";
			int j = 0;
			while (*loop != ',' && *loop != '<') {
				loopBuffer[j] = *loop;
				j++;
				loop += sizeof(char);
			}
			printf("le number est : %d\n", atoi(loopBuffer));
			voitureNombre[compteur] = atoi(loopBuffer);
			compteur++;
			if (*loop == ',') {
				loop+=sizeof(char);
			}
		}
	}
	return voitureNombre;
}
