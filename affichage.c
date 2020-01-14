#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "constantes.h"
#include "voiture.h"
#include "affichage.h"

#define LONGUEUR_DU_TABLEAU 69
#define LONGUEUR_DU_TABLEAU_COURSE 67


/** Fonction principale : permet de creer puis d'afficher le tableau dans le
*         terminal a l'aide d'une serie de concatenantions
*
* @param  voiture* classement[] un pointeur vers un tableau de voiture triees
* @param  int manche            un numero indiquant la partie de la serie de course
*                               dans laquelle on est
* @param gagnant* secteurs		    les trois voitures ayant ete les plus rapides dans un secteur
* @param int nombreDeVoitures   le nombre de voiture dans la course
*
*/
void afficherTableauScore(voiture *classement[], int manche, gagnant *secteurs, int nombreDeVoitures){
  system("clear");
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|------------|\n";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour | temps-tour |\n";
  char *titrePeriode = creationManche(manche);

  char *buffer = (char*) calloc(1, LONGUEUR_DU_TABLEAU*(nombreDeVoitures*2+5));

  strcpy(buffer, "|------------------------------------------------------------------|\n");
  strcat(buffer, titrePeriode);
  strcat(buffer, lineSeparator);
  strcat(buffer, titreTableau);
  strcat(buffer, lineSeparator);

  free(titrePeriode);  //libere la memoire qui n'est plus utile

  //  creation des lignes du tableau les unes apres les autres, et les ajoutes
  //  a la fin du buffer
  for (int i = 0; i < nombreDeVoitures; i++) {
    char *recup;
    char *ligne = calloc(1, strlen(lineSeparator));
    strcpy(ligne, "| voiture");

    recup = creationCelluleNombre(6, classement[i]->id,(int)floor(log10(classement[i]->id))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile
    recup = NULL;

    recup = creationCelluleNombre(8 , classement[i]->tempSecteur1, (int)floor(log10(classement[i]->tempSecteur1))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(8, classement[i]->tempSecteur2, (int)floor(log10(classement[i]->tempSecteur2 ))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(8, classement[i]->tempSecteur3, (int)floor(log10(classement[i]->tempSecteur3))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    strcat(ligne, decodageStatus(classement[i]->status, classement[i]->crash));

    recup = creationCelluleNombre(7, classement[i]->tours, (int)floor(log10(classement[i]->tours))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(13, classement[i]->meilleurTemps, (int)floor(log10(classement[i]->meilleurTemps))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    strcat(ligne, "\n");

    strcat(buffer, ligne);
    strcat(buffer, lineSeparator);

    free(ligne);  //libere la memoire occupee par la ligne car plus necessaire
  }
  printf("%s\n", buffer);
  free(buffer);  //libere la memoire occupee par le tableau car plus necessaire

  if (secteurs[0].voitureId == -1) {
    printf("meilleur temps dans le secteur 1 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 1 : %d\n", secteurs[0].voitureId);
    printf("meilleur temps dans le secteur 1 : %d\n", secteurs[0].voitureTemps);
  }
  if (secteurs[1].voitureId == -1) {
    printf("meilleur temps dans le secteur 2 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 2 : %d\n", secteurs[1].voitureId);
    printf("meilleur temps dans le secteur 2 : %d\n", secteurs[1].voitureTemps);
  }
  if (secteurs[2].voitureId == -1) {
    printf("meilleur temps dans le secteur 3 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 3 : %d\n", secteurs[2].voitureId);
    printf("meilleur temps dans le secteur 3 : %d\n", secteurs[2].voitureTemps);
  }
}

/* Fonction d'affichage  les qualifications : permet de creer puis d'afficher
*         le tableau dans le terminal a l'aide d'une serie de concatenantions
*
* @param tuple* classement[] un pointeur vers un tableau de voiture triees en memoire partagee et locale
* @param int manche          un numero indiquant la partie de la serie de course dans laquelle
*                           on est
* @param gagnant* secteurs  les trois voitures ayant ete les plus rapides dans un secteur
* @param int nombreDeVoitures   le nombre de voiture dans la course
*
* N.B: tuple utilise  gestion des pointeurs entre les pointeurs de la memoire locale et partagee
*/
void afficherTableauScoreQualif(tuple *classement[], int manche, gagnant *secteurs, int nombreDeVoitures){
  system("clear");
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|------------|\n";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour | temps-tour |\n";
  char *titrePeriode = creationManche(manche);

  char *buffer = (char*) calloc(1, LONGUEUR_DU_TABLEAU*(nombreDeVoitures*2+5));

  strcpy(buffer, "|------------------------------------------------------------------|\n");
  strcat(buffer, titrePeriode);
  strcat(buffer, lineSeparator);
  strcat(buffer, titreTableau);
  strcat(buffer, lineSeparator);

  free(titrePeriode);  //libere la memoire qui n'est plus utile

  //  creation des lignes du tableau les unes apres les autres, et les ajoutes
  //  a la fin du buffer
  for (int i = 0; i < nombreDeVoitures; i++) {
    char *recup;
    char *ligne = calloc(1, strlen(lineSeparator));
    strcpy(ligne, "| voiture");

    recup = creationCelluleNombre(6, classement[i]->local->id,(int)floor(log10(classement[i]->local->id))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile
    recup = NULL;

    recup = creationCelluleNombre(8 , classement[i]->local->tempSecteur1, (int)floor(log10(classement[i]->local->tempSecteur1))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(8, classement[i]->local->tempSecteur2, (int)floor(log10(classement[i]->local->tempSecteur2 ))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(8, classement[i]->local->tempSecteur3, (int)floor(log10(classement[i]->local->tempSecteur3))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    strcat(ligne, decodageStatus(classement[i]->local->status, classement[i]->local->crash));

    recup = creationCelluleNombre(7, classement[i]->local->tours, (int)floor(log10(classement[i]->local->tours))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(13, classement[i]->local->meilleurTemps, (int)floor(log10(classement[i]->local->meilleurTemps))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    strcat(ligne, "\n");

    strcat(buffer, ligne);
    strcat(buffer, lineSeparator);

    free(ligne);  //libere la memoire occupee par la ligne car plus necessaire
  }
  printf("%s\n", buffer);
  free(buffer);  //libere la memoire occupee par le tableau car plus necessaire

  if (secteurs[0].voitureId == -1) {
    printf("meilleur temps dans le secteur 1 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 1 : %d\n", secteurs[0].voitureId);
    printf("meilleur temps dans le secteur 1 : %d\n", secteurs[0].voitureTemps);
  }
  if (secteurs[1].voitureId == -1) {
    printf("meilleur temps dans le secteur 2 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 2 : %d\n", secteurs[1].voitureId);
    printf("meilleur temps dans le secteur 2 : %d\n", secteurs[1].voitureTemps);
  }
  if (secteurs[2].voitureId == -1) {
    printf("meilleur temps dans le secteur 3 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 3 : %d\n", secteurs[2].voitureId);
    printf("meilleur temps dans le secteur 3 : %d\n", secteurs[2].voitureTemps);
  }
}

/* Fonction d'affichage  la course : permet de creer puis d'afficher
*         le tableau dans le terminal a l'aide d'une serie de concatenantions
*
* @param  voiture* classement[] un pointeur vers un tableau de voiture triees
* @param  int manche            un numero indiquant la partie de la serie de course
*                               dans laquelle on est
* @param gagnant* secteurs		  les trois voitures ayant ete les plus rapides dans un secteur
*	@param int meilleurIdTemps	  le meilleur temps pour faire un tour de circuit
*	@param int meilleurId				  le numero de la voiture ayant fait le meilleur temps pour un tour
*															  de circuit
* @param int nombreDeVoitures   le nombre de voiture dans la course
*
*/
void afficherTableauScoreCourse(voiture *classement[], int manche, gagnant *secteurs, int meilleurId, int meilleurIdTemps, int nombreDeVoitures){
  system("clear");
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|---------|\n";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour |  temps  |\n";
  char *titrePeriode = creationManche(manche);

  char *buffer = (char*) calloc(1, LONGUEUR_DU_TABLEAU*(nombreDeVoitures*2+5));

  strcpy(buffer, "|---------------------------------------------------------------|\n");
strcat(buffer, titrePeriode);
  strcat(buffer, lineSeparator);
  strcat(buffer, titreTableau);
  strcat(buffer, lineSeparator);

  free(titrePeriode);  //libere la memoire qui n'est plus utile

  //  creation des lignes du tableau les unes apres les autres, et les ajoutes
  //  a la fin du buffer
  for (int i = 0; i < nombreDeVoitures; i++) {
    char *recup;
    char *ligne = calloc(1, strlen(lineSeparator));
    strcpy(ligne, "| voiture");

    recup = creationCelluleNombre(6, classement[i]->id,(int)floor(log10(classement[i]->id))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile
    recup = NULL;

    recup = creationCelluleNombre(8 , classement[i]->tempSecteur1, (int)floor(log10(classement[i]->tempSecteur1))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(8, classement[i]->tempSecteur2, (int)floor(log10(classement[i]->tempSecteur2 ))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(8, classement[i]->tempSecteur3, (int)floor(log10(classement[i]->tempSecteur3))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    strcat(ligne, decodageStatus(classement[i]->status, classement[i]->crash));

    recup = creationCelluleNombre(7, classement[i]->tours, (int)floor(log10(classement[i]->tours))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    recup = creationCelluleNombre(10, classement[i]->tempsTotal, (int)floor(log10(classement[i]->tempsTotal))+1);
    strcat(ligne, recup);
    free(recup); //libere la memoire qui n'est plus utile

    strcat(ligne, "\n");

    strcat(buffer, ligne);
    strcat(buffer, lineSeparator);

    free(ligne);  //libere la memoire occupee par la ligne car plus necessaire
  }
  printf("%s\n", buffer);
  free(buffer);  //libere la memoire occupee par le tableau car plus necessaire

  if (secteurs[0].voitureId == -1) {
    printf("meilleur temps dans le secteur 1 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 1 : %d\n", secteurs[0].voitureId);
    printf("meilleur temps dans le secteur 1 : %d\n", secteurs[0].voitureTemps);
  }
  if (secteurs[1].voitureId == -1) {
    printf("meilleur temps dans le secteur 2 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 2 : %d\n", secteurs[1].voitureId);
    printf("meilleur temps dans le secteur 2 : %d\n", secteurs[1].voitureTemps);
  }
  if (secteurs[2].voitureId == -1) {
    printf("meilleur temps dans le secteur 3 : %s\n", "NA");
  }
  else{
    printf("meilleur voiture dans le secteur 3 : %d\n", secteurs[2].voitureId);
    printf("meilleur temps dans le secteur 3 : %d\n", secteurs[2].voitureTemps);
  }
  if (meilleurId == -1) {
    printf("le tour le plus rapide : %s\n", "NA");
  }
  else{
    printf("le tour le plus rapide : %d pour la voiture %d\n", meilleurIdTemps, meilleurId);
  }
}

/** Permet de generer et de remplir un case  le tableau.
*  ATTENTION : necessite de faire un free apres utilisation car on utilise un malloc
*
* @param  int tailleCellule la taille de la cellule qu'on desire creer
* @param  int input         nombre que l'on desire introduire dans la cellule
*                           si le nombre == 0, on le remplacera par le string NA
* @param  int sizeInput     le nombre de chiffres constituant le parametre input
* @return                   renvoie un poiteur vers la zone memoire contenant notre cellule
*/
char* creationCelluleNombre(int tailleCellule, int input, int sizeInput){
  if (input <= 0 || input == INT_MAX) {                                //rien a afficher
    char *cell = malloc(tailleCellule);
    strcat(cell, " NA");
    for (int i = 0; i < tailleCellule-4; i++) {
      strcat(cell," ");
    }
    strcat(cell, "|");

    return cell;
  }
  else{                                            //on affiche l'input
    char pass[sizeInput];
    sprintf(pass, "%d", input);

    char *cell = malloc(tailleCellule);
    strcat(cell, " ");
    strcat(cell, pass);
    for (int i = 0; i < (tailleCellule-sizeInput-2); i++) {
      strcat(cell," ");
    }
    strcat(cell, "|");

    return cell;
  }
}

/** Permet de choisir entre les differents etats possibles  la voiture a savoir
*  OUT, P ou GO
*
* @param  int status  le status de la voiture sous format numerique
* @param  int crash   booleen permettant de savoir si la voiture est crashee
*
* @return             retourne une chaine de caracteres de bonne taille  la cellule
*/
char* decodageStatus(int status, int crash){
  if (crash) {
    return " OUT    |";
  }
  if (status == 1) {
    return " P      |";
  }
  if (status == 0){
    return " NO RUN |";
  }
  else{
    return " GO     |";
  }
}

/** Permet de creer le titre du tableau.  Selectionne le titre correcte  la
*  course en cours
*
* @param  int manche  un nombre permettant de determiner la course dans laquelle on est.
*                     Doit etre compris entre 1 et 7 inclus
* @return             renvoie un poiteur vers la zone memoire contenant notre ligne
*/
char* creationManche(int manche){
  char *retour = (char*) malloc(sizeof(char)*LONGUEUR_DU_TABLEAU);
  switch (manche) {
    case 1:
      strcpy(retour, "|                               P1                                 |\n");
      break;
    case 2:
      strcpy(retour, "|                               P2                                 |\n");
      break;
    case 3:
      strcpy(retour, "|                               P3                                 |\n");
      break;
    case 4:
      strcpy(retour, "|                               Q1                                 |\n");
      break;
    case 5:
      strcpy(retour, "|                               Q2                                 |\n");
      break;
    case 6:
      strcpy(retour, "|                               Q3                                 |\n");
      break;
    case 7:
      strcpy(retour, "|                            COURSE                             |\n");
      break;
  }
  return retour;
}
