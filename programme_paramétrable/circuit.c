#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>
#include <limits.h>
#include "constantes.h"
#include "voiture.h"
#include "secteurs.h"
#include "affichage.h"
#include "circuit.h"
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
int tour(voiture *maVoiture, sem_t *sem, int tempsS1, int tempsS2, int tempsS3, int tempsStand, int chanceStand, int chanceCrash){

    int total=0;                    //temps total pour le tour
    int s = 0;                      //temps pour un secteur
    int i = 1;

    while (i<=3){

        switch (i) {
          case 1:
            s = secteur(tempsS1-30, tempsS1+70, chanceCrash);
            break;
          case 2:
            s = secteur(tempsS2-30,tempsS2+70, chanceCrash);
            break;
          case 3:
            s = secteur(tempsS3-30,tempsS3+70, chanceCrash);
            break;
        }

        sleep((s*10)/500);         // endormir le processus pendant s*10 milliseconde
        if (i == 1) {
          sem_wait(sem);
          refreshSecteurs(maVoiture);
          sem_post(sem);
        }
        if(s == 0){                 //test si il y a un crash
          sem_wait(sem);
          maVoiture->status = 0;
          maVoiture->crash = TRUE;
          maVoiture->changeOrdre = TRUE;
          refreshSecteurs(maVoiture);
          sem_post(sem);
          return 0;
        }
        if((i%2)==0){             //si il passe dans le secteur 2
          sem_wait(sem);
          maVoiture->tempSecteur2 = s;
          sem_post(sem);
        }
        else if((i%3)==0){        //si il passe dans le secteur 3
          if(stand(chanceStand)){
            s += tempsStand;

            sem_wait(sem);
            maVoiture->status=1;
            sem_post(sem);

            sleep(tempsStand/100);         // endormir le processus pendant s*10 milliseconde

          }
          sem_wait(sem);
          maVoiture->status=2;
          maVoiture->tempSecteur3 = s;
          sem_post(sem);
        }
        else{                     //si il passe dans le secteur 1
          sem_wait(sem);
          maVoiture->tempSecteur1 = s;
          sem_post(sem);
        }

        total += s;               //ajout au temps total de la voiture dans le circuit
        i++;
    }

    return total;
}

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
void essaiLibreQuali(int chrono, voiture *maVoiture, sem_t *sem, data *programmeData){
    int tempSecteur1 = (int)(programmeData->s1/programmeData->vitesseVoiture);
    int tempSecteur2 = (int)(programmeData->s2/programmeData->vitesseVoiture);
    int tempSecteur3 = (int)(programmeData->s3/programmeData->vitesseVoiture);

    int temps1 = 0;
    int temps2 = 0;
    int j = 1;
    do{
        //effectue un tour puis incremente le temps total que la voiture aura passe en course
        temps1 = tour(maVoiture, sem, tempSecteur1, tempSecteur2, tempSecteur3, programmeData->dureeStand, programmeData->chanceStand, programmeData->chanceCrash);
        temps2 += temps1;

        if(temps1==0){
          sem_wait(sem);
          maVoiture->ready = -1;
          sem_post(sem);
          return;
        }

        sem_wait(sem);
        maVoiture->tours += 1;
        //verifie si la voiture a fait un meilleur temps que ce qu'elle avait precedemment fait
        if (maVoiture->meilleurTemps > temps1 || maVoiture->meilleurTemps == 0) {
          maVoiture->meilleurTemps = temps1;        //sauvegarde la valeur en memoire partagee
          if (!maVoiture->changeOrdre) {
            maVoiture->changeOrdre = TRUE;          //indique que le temps de la voiture a changeOrdre
          }
        }
        sem_post(sem);

    }while(temps2<chrono && temps1 !=0);
    sem_wait(sem);
    maVoiture->ready = -1;
    sem_post(sem);
}

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
int tourCourse(voiture *maVoiture, int numeroTour, int tourMax, sem_t *sem, int tempsS1, int tempsS2, int tempsS3, int tempsStand, int chanceStand, int chanceCrash){

    int total=0;                    //temps total
    int s = 0;                      //temps pour un secteur
    int i = 1;

    while (i<=3){

        switch (i) {
          case 1:
            s = secteur(tempsS1-30, tempsS1+70, chanceCrash);
            break;
          case 2:
            s = secteur(tempsS2-30,tempsS2+70, chanceCrash);
            break;
          case 3:
            s = secteur(tempsS3-30,tempsS3+70, chanceCrash);
            break;
        }

        sleep((s*10)/500);         // endormir le processus pendant s*10 milliseconde

        if (i == 1) {
          sem_wait(sem);
          refreshSecteurs(maVoiture);
          sem_post(sem);
        }

        if(s == 0){                 //test si il y a un crash
          sem_wait(sem);
          maVoiture->status = 0;
          maVoiture->crash = TRUE;
          maVoiture->meilleurTemps = INT_MAX;
          maVoiture->tempsTotal = INT_MAX;
          maVoiture->changeOrdre = TRUE;
          refreshSecteurs(maVoiture);
          sem_post(sem);
          return 0;
        }
        if((i%2)==0){                   //si il passe dans le secteur 2
          maVoiture->tempSecteur2 = s;
        }
        else if((i%3)==0){              //si il passe dans le secteur 3
          if(stand(chanceStand) || ((int)(tourMax/numeroTour) == 3 && maVoiture->passageAuStand < 1) || ((int)(tourMax/numeroTour) == 2 && maVoiture->passageAuStand < 2)){
            s += tempsStand;

            sem_wait(sem);
            maVoiture->status=1;
            maVoiture->passageAuStand +=1;
            sem_post(sem);

            sleep(tempsStand/100);                  // endormir le processus pendant s*10 milliseconde
          }
          sem_wait(sem);
          maVoiture->status=2;
          maVoiture->tempSecteur3 = s;
          sem_post(sem);
        }
        else{
          sem_wait(sem);
          maVoiture->tempSecteur1 = s;   //si il passe dans le secteur 1
          sem_post(sem);
        }
        sem_wait(sem);
				maVoiture->tempsTotal += s;
				maVoiture->changeOrdre = TRUE;
        sem_post(sem);
        total += s;                       //ajout au temps total de la voiture dans le circuit
        i++;
    }

    return total;
}

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
void Course(int tours, voiture *maVoiture, sem_t *sem, data *programmeData){
    int tempSecteur1 = (int)(programmeData->s1/programmeData->vitesseVoiture);
    int tempSecteur2 = (int)(programmeData->s2/programmeData->vitesseVoiture);
    int tempSecteur3 = (int)(programmeData->s3/programmeData->vitesseVoiture);

    int temps1 = 0;
    int temps2 = -1;
    int j = 1;

    do{
        //effectue un tour puis incremente le temps total que la voiture aura passe en course
        temps1 = tourCourse(maVoiture, maVoiture->tours, tours, sem, tempSecteur1, tempSecteur2, tempSecteur3, programmeData->dureeStand, programmeData->chanceStand, programmeData->chanceCrash);
        temps2 += temps1;


        if(temps1==0){
          sem_wait(sem);
          maVoiture->ready = -1;
          sem_post(sem);
          return;
        }

        sem_wait(sem);
        maVoiture->tours += 1;
        //verifie si la voiture a fait un meilleur temps que ce qu'elle avait precedemment fait
				if (maVoiture->meilleurTemps > temps1 || maVoiture->meilleurTemps == 0) {
					maVoiture->meilleurTemps = temps1;        //sauvegarde la valeur en memoire partagee
					if (!maVoiture->changeOrdre) {
						maVoiture->changeOrdre = TRUE;          //indique que le temps de la voiture a changeOrdre
					}
				}
        sem_post(sem);
    }while(maVoiture->tours<=tours && temps1 !=0);
    sem_wait(sem);
		maVoiture->ready = -1;
    sem_post(sem);
}

/** remets les secteurs de la voiture a zero.  Cela permet de simuler la fin d'un tour
*   sur le circuit
*
* @param voiture* mavoiture pointeur vers l'emplacement memoire de la voiture
*                           simulee par le processus
*
*/
void refreshSecteurs(voiture *maVoiture){
  maVoiture->tempSecteur1 = 0;
  maVoiture->tempSecteur2 = 0;
  maVoiture->tempSecteur3 = 0;
}
