#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "constantes.h"
#include "voiture.h"
#include "affichage.h"

// TODO : resoudre le bug graphique qui apparait toujours a la deuxieme frame

void afficherTableauScore(voiture *classement[], int manche){
  system("clear");
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|-------|\n";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour | temps |\n";
  int length = strlen(lineSeparator);
  char *titrePeriode = creationManche(manche, length);

  char *buffer = (char*) calloc(1, length*(NOMBRE_DE_VOITURE*2+5));

  strcat(buffer, "|-------------------------------------------------------------|\n");
  strcat(buffer, titrePeriode);
  strcat(buffer, lineSeparator);
  strcat(buffer, titreTableau);
  strcat(buffer, lineSeparator);

  free(titrePeriode);

  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    char *recup;
    char *ligne = calloc(1, strlen(lineSeparator));
    strcat(ligne, "| voiture");

    recup = creationCelluleNombre(6, classement[i]->id,(int)floor(log10(classement[i]->id))+1);
    strcat(ligne, recup);
    free(recup);
    recup = NULL;

    recup = creationCelluleNombre(8 , classement[i]->tempSecteur1, (int)floor(log10(classement[i]->tempSecteur1))+1);
    strcat(ligne, recup);
    free(recup);

    recup = creationCelluleNombre(8, classement[i]->tempSecteur2, (int)floor(log10(classement[i]->tempSecteur2 ))+1);
    strcat(ligne, recup);
    free(recup);

    recup = creationCelluleNombre(8, classement[i]->tempSecteur3, (int)floor(log10(classement[i]->tempSecteur3))+1);
    strcat(ligne, recup);
    free(recup);

    strcat(ligne, decodageStatus(classement[i]->status));

    recup = creationCelluleNombre(7, classement[i]->tours, (int)floor(log10(classement[i]->tours))+1);
    strcat(ligne, recup);
    free(recup);

    recup = creationCelluleNombre(8, classement[i]->meilleurTemps, (int)floor(log10(classement[i]->meilleurTemps))+1);
    strcat(ligne, recup);
    free(recup);

    strcat(ligne, "\n");

    strcat(buffer, ligne);
    strcat(buffer, lineSeparator);

    free(ligne);
  }
  printf("%s\n", buffer);
  free(buffer);
}

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

char* decodageStatus(int status){
  if (status == 0) {
    return " OUT    |";
  }
  if (status == 1) {
    return " P      |";
  }
  else{
    return " GO     |";
  }
}

char* creationManche(int manche, int len){
  char *retour = (char*) malloc(sizeof(char)*len);
  switch (manche) {
    case 1:
      strcat(retour, "|                             P1                              |\n");
      break;
    case 2:
      strcat(retour, "|                             P2                              |\n");
      break;
    case 3:
      strcat(retour, "|                             P3                              |\n");
      break;
    case 4:
      strcat(retour, "|                             Q1                              |\n");
      break;
    case 5:
      strcat(retour, "|                             Q2                              |\n");
      break;
    case 6:
      strcat(retour, "|                             Q3                              |\n");
      break;
    case 7:
      strcat(retour, "|                           COURSE                            |\n");
      break;
  }
  return retour;
}
