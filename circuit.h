#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED
#include "voiture.h"
#include <semaphore.h>

int tour(voiture *maVoiture);
void essaiLibreQuali(int chrono,voiture *maVoiture);
int tourCourse(voiture *maVoiture);
int Course(int tours, voiture *mavoiture);

#endif // CIRCUIT_H_INCLUDED
/**
*
*simule un tour dans un circuit
*
*
*/
