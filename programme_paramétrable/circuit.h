#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED
#include <semaphore.h>
#include "voiture.h"
#include "loading_config.h"

/** simule un tour dans un circuit pour les qualif et les essais
*
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
* @param sem_t* sem         semaphore de la voiture permettant de garder les
*                           zones d'ecriture a risque
* @param int tempsS1        temps moyen pour le secteur 1
* @param int tempsS2        temps moyen pour le secteur 2
* @param int tempsS3        temps moyen pour le secteur 3
* @param int tempsStand     temps durant lequel la voiture sera au stand
* @param int chanceStand    probabilite que la voiture doive aller au stand
* @param int chanceCrash    probabilite que la voiture se crashe
*
* @return int               retourne le temps total qu'a pris la voiture pour faire
*                           un tour
*/
int tour(voiture *maVoiture, sem_t *sem, int tempsS1, int tempsS2, int tempsS3, int tempsStand, int chanceStand, int chanceCrash);

/** simule le deroulement d'une session entiere d'une couse d'essais ou de qualification
*
* @param int chrono           le temps total que doit mettre au plus la voiture
* @param voiture* mavoiture   pointeur vers l'emplacement memoire de la voiture
*                             simulee par le processus
* @param sem_t* sem           semaphore de la voiture permettant de garder les
*                             zones d'ecriture a risque
* @param data* programmeData  pointeur vers la zone de memoire partagee contenant les
*                             donnee de parametrisation de la course
*
*/
void essaiLibreQuali(int chrono, voiture *maVoiture, sem_t *sem, data *programmeData);

/** simule un tour dans un circuit pour la course principale
*
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
* @param int numeroTour     permet de savoir a quel tour on est
* @param int tourMax        nombre de tour max de la course
* @param sem_t* sem         semaphore de la voiture permettant de garder les
*                           zones d'ecriture a risque
* @param int tempsS1        temps moyen pour le secteur 1
* @param int tempsS2        temps moyen pour le secteur 2
* @param int tempsS3        temps moyen pour le secteur 3
* @param int tempsStand     temps durant lequel la voiture sera au stand
* @param int chanceStand    probabilite que la voiture doive aller au stand
* @param int chanceCrash    probabilite que la voiture se crashe
*
* @return int               retourne le temps total qu'a pris la voiture pour faire
*                           un tour
*/
int tourCourse(voiture *maVoiture, int numeroTour, int tourMax, sem_t *sem, int tempsS1, int tempsS2, int tempsS3, int tempsStand, int chanceStand, int chanceCrash);

/** simule le deroulement de l'entierete de la course
*
* @param int tours          le nombre de tours que comporte la course
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
* @param sem_t* sem         semaphore de la voiture permettant de garder les
*                           zones d'ecriture a risque
* @param data* programmeData  pointeur vers la zone de memoire partagee contenant les
*                             donnee de parametrisation de la course
*
*/
void Course(int tours, voiture *maVoiture, sem_t *sem, data *programmeData);

/** remets les secteurs de la voiture a zero.  Cela permet de simuler la fin d'un tour
*   sur le circuit
*
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
*
*/
void refreshSecteurs(voiture *maVoiture);

#endif // CIRCUIT_H_INCLUDED
