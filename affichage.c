#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "constantes.h"
#include "voiture.h"
#include "affichage.h"

void afficherTableauScore(voiture *classement[]){
  char lineSeparator[] = "|-------------|-------|-------|-------|--------|------|-------|";
  char titreTableau[] =  "| Période     |  S1   |  S2   |  S3   | status | tour | temps |";

  printf("%s\n", lineSeparator);
  printf("%s\n", titreTableau);
  printf("%s\n", lineSeparator);
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

    printf("%s\n", ligne);
    printf("%s\n", lineSeparator);

    free(ligne);
  }

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
