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
#include "loading_config.h"

/** lis le fichier de configuration ligne par ligne, repere les caracteres servant
*   de balises et lance les fonctions s'y rapportant
*
* @param data* programmeData  pointeur vers l'emplacement en memoire partagee ou doivent
*                             etre stockees les donnees
* @param char* file           string contenant le chemin vers le fichier de configuration a lire
*
* @return int*                chaine d'int contenant les numeros des voitures qui vont participer
*                             a la course
*
*/
int* configuration(data *programmeData, char *file){

  char buffer[200];
  FILE* fichier = NULL;
  int nombreVoitures = 0;
  int incr = 0;
  int nextLoop;
  fichier = fopen(file, "r");
  int *liste;

  if (fichier == NULL){
    free(programmeData);
    programmeData = NULL;
    printf("%s\n", "le fichier de configuration n'existe, veuillez rentrer un fichier de configuration valide");
    return NULL;
  }

  while (fgets(buffer , 200, fichier) != NULL) {
    char *loop = buffer;
    nextLoop = FALSE;
    while (*loop != '\0') {
      if (nextLoop) {
        break;
      }
      switch (*loop) {
        case '/':
          loop ++;
          if (*loop == '*' || *loop == '/'){
            nextLoop = TRUE;
          }
          break;
        case '*':
          nextLoop = TRUE;
          break;
        case ':':
          loop ++;
          if (*loop == '{' || *(loop+1) == '{') {
            liste = readNumerosVoitures(loop+1, nombreVoitures);
            nextLoop = TRUE;
          }
          else{
            readOtherInformation(programmeData, loop, incr);
            if (incr == 0) {
              nombreVoitures = programmeData->nombreDeVoitures;
            }
            incr ++;
            nextLoop = TRUE;
          }
          break;
        default :
          loop++;
          break;
      }
    }
  }
  return liste;
}

/** va decomposer le string contenant les numeros en coupant au niveau des virgules,
*   et en "oubliant" les espaces.  La balise de fin est l'accolade fermante (ou quand
*   le nombre max de numero a ete lu =>  en cas d'erreur de l'utilisateur)
*
* @param char* buffer     String contenant les numeros a decrypter
* @param int tailleListe  nombre max de numero a retrouver
*
* @return int*            chaine d'int contenant les numeros des voitures qui vont participer
*                         a la course
*
*/
int *readNumerosVoitures(char *buffer, int tailleListe){
  int *liste = (int *) malloc(tailleListe * sizeof(int));
  int incr = 0;

  char *concat = (char*) malloc(sizeof(char)*10);
  int i = 0;
  strcpy(concat, "");

  while (*buffer != '}') {
    if (incr >= tailleListe) {
      break;
    }
    switch (*buffer) {
      case ' ':
        buffer++;
        break;
      case ',' :
        liste[incr] = atoi(concat);
        incr ++;
        buffer++;
        free(concat);
        concat = (char*) malloc(sizeof(char)*10);
        i = 0;
        break;
      default :
        concat[i] = *buffer;
        i++;
        buffer++;
        break;
    }
  }
  if (incr < tailleListe) {
    liste[incr] = atoi(concat);
  }

  free(concat);
  return liste;
}

/** permet de lire les differentes donnees du fichier de config.  Ignore les espaces
*   et les %, et utilise le caractere "line feed" pour savoir quand arreter de lire
*   (ou la fin buffer en cas d'erreur)
*
* @param data* programmeData  pointeur vers l'emplacement en memoire partagee ou doivent
*                             etre stockees les donnees
* @param char* ligne          String contenant le nombre decrypter
* @param int commande         int permettant de savoir ou mettre le nombre decrypte
*
*/
void readOtherInformation(data *programmeData, char* ligne, int commande){
  char *buffer = (char*) malloc(sizeof(char)*20);
  int i = 0;
  strcpy(buffer, "");
  while (*ligne !='\0' && *ligne != '\n' && (int)*ligne !=10) {
    if (*ligne == ' ' || *ligne == '%') {
      ligne++;
    }
    else{
      buffer[i] = *ligne;
      i++;
      ligne++;
    }
  }
  double convert;
  char *rest;
  switch (commande) {
    case 0 :
      programmeData->nombreDeVoitures = atoi(buffer);
      break;
    case 1 :
      programmeData->vitesseVoiture = atoi(buffer);
      break;
    case 2 :
      programmeData->qualifOffset = atoi(buffer);
      break;
    case 3 :
      programmeData->p1 = atoi(buffer);
      break;
    case 4 :
      programmeData->p2 = atoi(buffer);
      break;
    case 5 :
      programmeData->p3 = atoi(buffer);
      break;
    case 6 :
      programmeData->q1 = atoi(buffer);
      break;
    case 7 :
      programmeData->q2 = atoi(buffer);
      break;
    case 8 :
      programmeData->q3 = atoi(buffer);
      break;
    case 9 :
      programmeData->toursCourse = atoi(buffer);
      break;
    case 10 :
      programmeData->s1 = atoi(buffer);
      break;
    case 11 :
      programmeData->s2 = atoi(buffer);
      break;
    case 12 :
      programmeData->s3 = atoi(buffer);
      break;
    case 13 :
      programmeData->dureeStand = atoi(buffer);
      break;
    case 14 :
      convert = strtod(buffer, &rest);
      programmeData->chanceStand = (int) (convert*10000);
      break;
    case 15 :
      convert = strtod(buffer, &rest);
      programmeData->chanceCrash = (int) (convert*10000);
      break;
  }
  free(buffer);
}

/** configure la parametrisation du programme par defaut en allant chercher les
*   differentes valeurs contenues dans le fichier constantes.h
*
* @param data* programmeData  pointeur vers l'emplacement en memoire partagee ou doivent
*                             etre stockees les donnees
*
* @return int*                chaine d'int contenant les numeros des voitures qui vont participer
*                             a la course
*
*/
int* defaultConfiguration(data *programmeData){
  programmeData->nombreDeVoitures = NOMBRE_DE_VOITURE;
  programmeData->vitesseVoiture = VITESSE_VOITURE;
  programmeData->qualifOffset = QUALIF_OFFSET;
  programmeData->p1 = P1;
  programmeData->p2 = P2;
  programmeData->p3 = P3;
  programmeData->q1 = Q1;
  programmeData->q2 = Q2;
  programmeData->q3 = Q3;
  programmeData->toursCourse = TOURS_COURSE;
  programmeData->s1 = S1;
  programmeData->s2 = S2;
  programmeData->s3 = S2;
  programmeData->dureeStand = DUREE_STAND;
  programmeData->chanceStand = CHANCE_STAND;
  programmeData->chanceCrash = CHANCE_CRASH;
  int *liste = (int *) malloc(sizeof(int)*NOMBRE_DE_VOITURE);

  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    liste[i] = VOITURE_NUMBER[i];
  }
  return liste;
}
