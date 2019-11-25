#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED
#include "voiture.h"

/**
*@param maVoiture
*@param sem
*
*
*/
int tour(voiture *maVoiture, sem_t *sem);

/**
*@param chrono
*@param *maVoiture
*@param *sem         Semaphore
*
*@return renvoie le temps total d'un tour
*/
void essaiLibreQuali(int chrono,voiture *maVoiture, sem_t *sem);

/**
*@param *tours
*@param *mavoiture
*
*
*/
void Course(int tours, voiture *mavoiture);

#endif // CIRCUIT_H_INCLUDED
