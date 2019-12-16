#include <stdio.h>
#include <stdlib.h>


char* convertion(int temps){

  int hours,minutes,seconds;
  int secondeRestante;
  char* phrase = (char*) malloc(sizeof(char)* 40);
  int secondsInHour = 3600;
  int secondsInMinute = 60;

  hours = (temps/secondsInHour); //Nombre d'heure
  secondeRestante = temps - (hours * secondsInHour); //Temps restant sans les heurs
  minutes = secondeRestante/secondsInMinute; //Nombre minutes
  secondeRestante = secondeRestante - (minutes*secondsInMinute); //Temps restant sans les minutes
  seconds = secondeRestante; //Nombre de secondes

  sprintf(phrase,"%d hour, %d minutes and %d seconds",hours,minutes,seconds);
  return phrase;
}
