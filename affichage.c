#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "constantes.h"
#include "voiture.h"
#include "affichage.h"

#define LONGUEUR_DU_TABLEAU 69
#define LONGUEUR_DU_TABLEAU_COURSE 67


/* Fonction principale : permet de creer puis d'afficher le tableau dans le
*         terminal a l'aide d'une serie de concatenantions
*
* @param  classement un pointeur vers un tableau de voiture triees
* @param  manche     un numero indiquant la partie de la serie de course dans laquelle
*                    on est
*/

void afficherTableauScore(voiture *classement[], int manche){
  system("clear");
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|------------|\n";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour | temps-tour |\n";
  char *titrePeriode = creationManche(manche);

  char *buffer = (char*) calloc(1, LONGUEUR_DU_TABLEAU*(NOMBRE_DE_VOITURE*2+5));

  strcpy(buffer, "|------------------------------------------------------------------|\n");
  strcat(buffer, titrePeriode);
  strcat(buffer, lineSeparator);
  strcat(buffer, titreTableau);
  strcat(buffer, lineSeparator);

  free(titrePeriode);  //libere la memoire qui n'est plus utile

  //  creation des lignes du tableau les unes apres les autres, et les ajoutes
  //  a la fin du buffer
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
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

    strcat(ligne, decodageStatus(classement[i]->status));

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
}

void afficherTableauScoreQualif(tuple *classement[], int manche){
  system("clear");
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|------------|\n";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour | temps-tour |\n";
  char *titrePeriode = creationManche(manche);

  char *buffer = (char*) calloc(1, LONGUEUR_DU_TABLEAU*(NOMBRE_DE_VOITURE*2+5));

  strcpy(buffer, "|------------------------------------------------------------------|\n");
  strcat(buffer, titrePeriode);
  strcat(buffer, lineSeparator);
  strcat(buffer, titreTableau);
  strcat(buffer, lineSeparator);

  free(titrePeriode);  //libere la memoire qui n'est plus utile

  //  creation des lignes du tableau les unes apres les autres, et les ajoutes
  //  a la fin du buffer
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
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

    strcat(ligne, decodageStatus(classement[i]->local->status));

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
}

void afficherTableauScoreCourse(voiture *classement[], int manche){
  system("clear");
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|---------|\n";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour |  temps  |\n";
  char *titrePeriode = creationManche(manche);

  char *buffer = (char*) calloc(1, LONGUEUR_DU_TABLEAU*(NOMBRE_DE_VOITURE*2+5));

  strcpy(buffer, "|---------------------------------------------------------------|\n");
  strcat(buffer, titrePeriode);
  strcat(buffer, lineSeparator);
  strcat(buffer, titreTableau);
  strcat(buffer, lineSeparator);

  free(titrePeriode);  //libere la memoire qui n'est plus utile

  //  creation des lignes du tableau les unes apres les autres, et les ajoutes
  //  a la fin du buffer
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
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

    strcat(ligne, decodageStatus(classement[i]->status));

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
}

/* Permet de generer et de remplir un case pour le tableau.
*  ATTENTION : necessite de faire un free apres utilisation car on utilise un malloc
*
* @param  tailleCellule  la taille de la cellule qu'on desire creer
* @param  input      nombre que l'on desire introduire dans la cellule
*                    si le nombre == 0, on le remplacera par le string NA
* @param  sizeInput  le nombre de chiffres constituant le parametre input
* @return            renvoie un poiteur vers la zone memoire contenant notre cellule
*/
//Attention cette méthode nécessite d'utiliser un free car elle est le résultat d'un malloc
char* creationCelluleNombre(int tailleCellule, int input, int sizeInput){
  if (input == 0) {
    char *cell = malloc(tailleCellule);
    strcat(cell, " NA");
    for (int i = 0; i < tailleCellule-4; i++) {
      strcat(cell," ");
    }
    strcat(cell, "|");

    return cell;
  }
  else{
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

/* Permet de choisir entre les differents etats possibles pour la voiture a savoir
*  OUT, P ou GO
*
* @param  status  le status de la voiture sous format numerique
* @return         retourne une chaine de caracteres de bonne taille pour la cellule
*/
char* decodageStatus(int status){
  if (status == 0) {
    return " OUT    |";
  }
  if (status == 1) {
    return " P      |";
  }
  if (status == -1){
    return " NO RUN |";
  }
  else{
    return " GO     |";
  }
}

/* Permet de creer le titre du tableau.  Selectionne le titre correcte pour la
*  course en cours
*
* @param  manche  un nombre permettant de determiner la course dans laquelle on est.
*                 Doit etre compris entre 1 et 7 inclus
* @return         renvoie un poiteur vers la zone memoire contenant notre ligne
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
