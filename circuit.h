#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED
#include "voiture.h"
#include <semaphore.h>

/** simule un tour dans un circuit pour les qualif et les essais
*
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
*
* @return int               retourne le temps total qu'a pris la voiture pour faire
*                           un tour
*/
int tour(voiture *maVoiture);

/** simule le deroulement d'une session entiere d'une couse d'essais ou de qualification
*
* @param int chrono         le temps total que doit mettre au plus la voiture
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
*
*/
void essaiLibreQuali(int chrono,voiture *maVoiture);

/** simule un tour dans un circuit pour la course principale
*
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
*
* @return int               retourne le temps total qu'a pris la voiture pour faire
*                           un tour
*/
int tourCourse(voiture *maVoiture);

/** simule le deroulement de l'entierete de la course
*
* @param int tours        le nombre de tours que comporte la course
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
*
*/
void Course(int tours, voiture *mavoiture);

void refreshSecteurs(voiture *maVoiture);

#endif // CIRCUIT_H_INCLUDED
