#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "constantes.h"
#include "voiture.h"
#include "secteurs.h"
#include "affichage.h"
/**
*
*simule un tour dans un circuit
*
*
*/
int tour(voiture *maVoiture){
    int total=0;//temps total
    int tour = 0;
    int crash = FALSE;//boolean pour le crash de la voiture
    int s = 0;//temps pour un secteur
    int i = 1;

    while (i<=3 && crash != TRUE){

        s = secteur(100,250);
        sleep((s*10)/1000);// endormir le processus pendant s*10 milliseconde

        if(s == 0){//test si il y a un crash
            crash = TRUE;
            //printf("crash sur le secteur %d\n",i);
            maVoiture->status = 0;
            return 0;
        }
        if((i%2)==0){//si il passe dans le secteur 2
           maVoiture->tempSecteur2 = s;
        }
        else if((i%3)==0){//si il passe dans le secteur 3
           // k.tempSecteur3=s;
           if(stand()){
                total += 15;
                maVoiture->status=1;
                //printf("passage au Stand: +15 seconde\n");
                sleep(1);// endormir le processus pendant s*10 milliseconde
                maVoiture->status=2;
           }
           maVoiture->tempSecteur3 = s;
        }
        else{//si il passe dans le secteur 1
           maVoiture->tempSecteur1 = s;
        }

        total += s;//ajout au temps total de la voiture dans le circuit
        i++;
        //recuperer le temps total ici
    }

    return total;
}

void essaiLibreQuali(int chrono, voiture *maVoiture){
    int temps1 = 0;
    int temps2 = 0;
    int j = 1;
    do{
        //TODO: recuperation des donnes de la voiture
        temps1 = tour(maVoiture);
        temps2 += temps1;

        if (maVoiture->meilleurTemps > temps1 || maVoiture->meilleurTemps == 0) {
          maVoiture->meilleurTemps = temps1;
          maVoiture->changeOrdre = TRUE;
        }

        if(temps1==0){
            printf("retour au stand: crash\n");
            //TODO: recuperation des donnes de la voiture
        }
        else if(temps2>=chrono){
            //TODO: recuperation des donnes de la voiture
            j++;
        }
        else {

            j++;
        }
    }while(temps2<chrono && temps1 !=0);
    maVoiture->ready = -1;
}

int Course(int tours, voiture *maVoiture){

    int temps1 = 0;
    int temps2 = -1;
    int j = 1;
    int i = 0;

    do{
        temps1 = tour(maVoiture);
        temps2 += temps1;
        if(temps1==0){
            printf("retour au stand: crash\n");
            //TODO: recuperation des donnes de la voiture
        }
        else{
            //TODO: recuperation des donnes de la voiture
            j++;
        }
    }while(i<=tours && temps1 !=0);

}
