#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED



#endif // SAVELOAD_H_INCLUDED

/**
*@param Id du tour Essai, nombre de voiture, temps du tour d'essai
*
*Ecriture des résultats du tour d'essai dans un fichier .txt vierge,
*
*
*/

void saveEssai(int p, int voiture, int chrono)//Module de sauvegarde de partie
{
	char bufferTemps[50];
	char bufferID[3];
    int i;
    //sauvegarde de fichier
    FILE* fichier = NULL;//création du fichier text // initialisation du pointeur sur le fichier
    fichier = fopen("F1_essai"+p+"_save.txt", "w");//nom du fichier
    if (fichier != NULL)
    {
        //écriture dans le fichier
		fprintf("classement des meilleurs temps de la periode d'essai %d\n",p);
		fprintf("temps du tour d'essai %d\n",chrono);
        for(i=0;i<voiture;i++)//réutilisation du "i" pour économie mémoire
            {
				sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
				sprintf(bufferID, "%d", classement[i]->id);
                fprintf("%d : %d\n",bufferID, bufferTemps);
            }
        fclose(fichier);//fermeture du fichier
    }
}

/**
*@param Id du tour Essai, nombre de voiture
*
*Ecriture des résultats de la sceance de qualif dans un fichier .txt vierge,
*
*
*/
void saveQuali(int p, int voiture)//Module de sauvegarde de partie
{
	char bufferTemps[50];
	char bufferID[3];
    int i;
    //sauvegarde de fichier
    FILE* fichier = NULL;//création du fichier text // initialisation du pointeur sur le fichier
    fichier = fopen("F1_quali"+p+"_save.txt", "w");//nom du fichier
    if (fichier != NULL)
    {
        //écriture dans le fichier
		fprintf("classement des meilleurs temps de la periode de qualification %d",p);
        for(i=0;i<voiture;i++)//réutilisation du "i" pour économie mémoire
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
*Ecriture des résultats de la course dans un fichier .txt vierge,
*
*
*/
void saveCourse(int voiture)//Module de sauvegarde de partie
{
    char bufferTemps[50];
	char bufferID[3];
    int i;
    //sauvegarde de fichier
    FILE* fichier = NULL;//création du fichier text // initialisation du pointeur sur le fichier
    fichier = fopen("F1_course_save.txt", "w");//nom du fichier
    if (fichier != NULL)
    {
        //écriture dans le fichier
		fprintf("classement des meilleurs temps de la course\n");
        for(i=0;i<voiture;i++)//réutilisation du "i" pour économie mémoire
            {
				sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
				sprintf(bufferID, "%d", classement[i]->id);
                fprintf("%d : %d\n",bufferID, bufferTemps);
            }
        fclose(fichier);//fermeture du fichier
    }
}
