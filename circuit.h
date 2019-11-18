#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED
#include "voiture.h"

int tour(voiture *maVoiture, sem_t *sem);
void essaiLibreQuali(int chrono,voiture *maVoiture, sem_t *sem);
int Course(int tours, voiture *mavoiture);

#endif // CIRCUIT_H_INCLUDED
/**
*
*simule un tour dans un circuit
*
*
*/
