#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <limits.h>
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
*	@param gagnant* secteurs		les trois voitures ayant ete les plus rapides dans un secteur
* @param int nombreDeVoitures	nombre de voitures dans la course
*
*/
void saveEssai(int p, int chrono, voiture **classement, gagnant *secteurs, int nombreDeVoitures)//Module de sauvegarde de partie
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
      for(i=0;i<nombreDeVoitures;i++){	//reutilisation du "i" pour economie memoire
				//sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
				sprintf(bufferID, "%d", classement[i]->id);
				if (classement[i]->meilleurTemps == INT_MAX) {
					fprintf(fichier, "Voiture %s : %s\n", bufferID, "OUT avant d'avoir fait un tour");
				}
				else{
					bufferTemps = convertion(classement[i]->meilleurTemps);
	        fprintf(fichier, "Voiture %s : %s\n",bufferID, bufferTemps);
					free(bufferTemps);
				}
      }
			for (int i = 0; i < 3; i++) {
				bufferTemps = convertion(secteurs[i].voitureTemps);
				fprintf(fichier, "le meilleur temps pour le secteur %d a été fait par la voiture numéro %d et est de %s \n", i+1,secteurs[i].voitureId, bufferTemps);
				free(bufferTemps);
			}
      fclose(fichier);//fermeture du fichier
    }
}

/** ecriture des resultats de la sceance de qualif dans un fichier .txt vierge
*
* @param tuple** classement 	pointeur de pointeurs permettant d'acceder a la liste des tuples
* 														contenant les voitures classee des qualifications venant
*															de se terminer
*	@param gagnant* secteurs		les trois voitures ayant ete les plus rapides dans un secteur
* @param int nombreDeVoitures	nombre de voitures dans la course
*
*/
void saveQuali(tuple **classement, gagnant *secteurs, int nombreDeVoitures)//Module de sauvegarde de partie
{
	char buffervoiture[50];
	char *time;

	int i;
	//sauvegarde de fichier
	FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
	fichier = fopen("F1_quali_save.txt", "w");//nom du fichier
	if (fichier != NULL){
      //ecriture dans le fichier
		fprintf(fichier, "<>", NULL);

		for (int i = 0; i <nombreDeVoitures-1; i++) {
			fprintf(fichier, "%d,", classement[i]->local->id);
		}
		fprintf(fichier, "%d", classement[nombreDeVoitures-1]->local->id);
		fprintf(fichier, "</>\n", NULL);

		fprintf(fichier, "classement des meilleurs temps des 3 période de qualification \n", NULL);
		for(i=0;i<nombreDeVoitures;i++){//reutilisation du "i" pour economie memoire
			fprintf(fichier, "%d : voiture numéro %d \n", i+1, classement[i]->local->id);
		}
		for (int i = 0; i < 3; i++) {
			time = convertion(secteurs[i].voitureTemps);
			fprintf(fichier, "le meilleur temps pour le secteur %d a été fait par la voiture numéro %d et est de %s \n", i+1,secteurs[i].voitureId, time);
			free(time);
		}
		fclose(fichier);//fermeture du fichier
	}
}

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
void saveCourse(voiture **classement, gagnant *secteurs, int meilleurTemps, int meilleurId, int nombreDeVoitures)//Module de sauvegarde de partie
{
	char bufferID[3];
	char *time;

    int i;
    //sauvegarde de fichier
    FILE* fichier = NULL;//creation du fichier text // initialisation du pointeur sur le fichier
    fichier = fopen("F1_course_save.txt", "w");//nom du fichier
    if (fichier != NULL){
        //ecriture dans le fichier
				fprintf(fichier, "classement des meilleurs temps de la course\n");
        for(i=0;i<nombreDeVoitures;i++){//reutilisation du "i" pour economie memoire
					sprintf(bufferID, "%d", classement[i]->id);
					if (classement[i]->tempsTotal == INT_MAX) {
						fprintf(fichier, "%d : voiture numéro %s : %s\n", i+1, bufferID,"crashée");
					}
					else{
						time = convertion(classement[i]->tempsTotal);
	          fprintf(fichier, "%d : voiture numéro %s : %s\n", i+1, bufferID,time);
						free(time);
					}
        }
				for (int i = 0; i < 3; i++) {
					time = convertion(secteurs[i].voitureTemps);
					fprintf(fichier, "le meilleur temps pour le secteur %d a été fait par la voiture numéro %d et est de %s \n", i+1,secteurs[i].voitureId, time);
					free(time);
				}
				if (meilleurTemps == INT_MAX) {
					fprintf(fichier, "le meilleur tour n'a été réalisé par aucunes voitures\n");
				}
				else{
					time = convertion(meilleurTemps);
					fprintf(fichier, "le meilleur tour a été réalisé pour la voiture %d avec un temps de : %s\n", meilleurId, time);
					free(time);
				}
        fclose(fichier);//fermeture du fichier
    }
}

/**chargement des resultats de la course depuis un fichier .txt deja rempli
*
* @param char* file 					nom du fichier a charger
* @param int nombreDeVoitures	nombre de voitures dans la course
*
* @return renvoie un pointeur vers un tableau contenant les id des voitures contenues
*					dans le fichier
*/
int *loading(char *file, int nombreDeVoitures){

	int *voitureNombre = (int *) malloc(sizeof(int)*nombreDeVoitures);
	char buffer[250];

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
			voitureNombre[compteur] = atoi(loopBuffer);
			compteur++;
			if (*loop == ',') {
				loop+=sizeof(char);
			}
		}
	}
	return voitureNombre;
}
