#include <stdio.h>
#include <stdlib.h>

#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

/** Permet de convertir un temps donne en seconde, en un temps de type :
*   XX hour, XX minutes and XX seconds
*
* @param int temps     le temps en secondes qui va etre convertis
*
* @return char* temps  la conversion, etant sauvegardee en memoire.  Necessitera un
*                      free() par la suite
*
*/
char* convertion(int temps){

  int hours,minutes,seconds;
  int secondeRestante;
  char* phrase = (char*) malloc(sizeof(char)* 40);

  hours = (temps/SECONDS_IN_HOUR); //Nombre d'heure
  secondeRestante = temps - (hours * SECONDS_IN_HOUR); //Temps restant sans les heurs
  minutes = secondeRestante/SECONDS_IN_MINUTE; //Nombre minutes
  secondeRestante = secondeRestante - (minutes*SECONDS_IN_MINUTE); //Temps restant sans les minutes
  seconds = secondeRestante; //Nombre de secondes

  sprintf(phrase,"%d hour, %d minutes and %d seconds",hours,minutes,seconds);
  return phrase;
}
