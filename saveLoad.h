#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED



#endif // SAVELOAD_H_INCLUDED
void saveEssai(int p, int voiture)//Module de sauvegarde de partie
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
		fprintf("classement des meilleurs temps de la periode d'essai %d",p);
        for(i=0;i<voiture;i++)//réutilisation du "i" pour économie mémoire
            {
				sprintf(bufferTemps, "%d", classement[i]->meilleurTemps);
				sprintf(bufferID, "%d", classement[i]->id);
                fprintf("%d : %d\n",bufferID, bufferTemps);
            }
        fclose(fichier);//fermeture du fichier
    }
}
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
}


































void Load()//Module pour Charger une partie
{
    int choix;
    int x;
    int Nombre;
    int Chiffre;
    char nomFichier[]="F1save";
    printf("voulez vous charger la derniere partie? 1 pour oui/0 pour non\n");
    scanf("%d",&choix);
    system("cls");
    if(choix==1)
        {//si choix oui ouverture du fichier .txt
        strcat(nomFichier,".txt");//permet la selection du fichier de score
        system("cls");
        clearStdin();
        FILE* fichier= NULL;// initialisation du pointeur sur le fichier
        fichier = fopen(nomFichier, "r");//lecture du fichier

        if(fichier != NULL)
            {
            for(x=0;x<16-1;x++)//passe dans toutes les \n du fichier pour obtenir les scores
                {
                tab[x].score=fgetc(fichier)-48;//fgetc est en ascii donc -48 pour avoir le premier chiffre de tab[x].score
                Nombre=fgetc(fichier)-48;
                if (Nombre>=0&&Nombre<=9)
                    {
                    tab[x].score = (tab[x].score)*10 + Nombre;//si caractere est un chiffre alors *10 pour le mettre en discaine et on l'affecte à tab[x].score

                    Nombre=fgetc(fichier);
                    }
                if(tab[x].score==-29)
                    {
                    tab[x].score=-1;
                    }
                }

                tab[x].score=fgetc(fichier)-48;//fgetc est en ascii donc -48 pour avoir le premier chiffre de tab[x].score
                Nombre=fgetc(fichier)-48;
                Chiffre=fgetc(fichier)-48;

                if (Nombre>=0&&Nombre<=9)
                    {
                    if(Chiffre>=0&&Chiffre<=9)
                        {
                        tab[x].score = (tab[x].score)*100 + Nombre*10 + Chiffre;//si caractere est un chiffre alors *100 pour le mettre en centaine et on l'affecte à tab[x].score
                        }
                    else
                        {
                        tab[x].score = (tab[x].score)*10 + Nombre;//si caractere est un chiffre alors *10 pour le mettre en discaine et on l'affecte à tab[x].score
                        }
                    }

            }
        else
            {//si erreur lors de l'ouverture du fichier, affichage d'une erreur
            printf("impossible d'ouvrir le fichier des scores\n");
            Load();//relancer le module
            }
        }
    else if(choix=0)
        {
        afficher ();//utilisation du module d'initialisation du tableau de score
        }
}
