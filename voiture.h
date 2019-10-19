#ifndef VOITURE_H_INCLUDED
#define VOITURE_H_INCLUDED

typedef struct voiture {
int id;

int tempSecteur1;
int tempSecteur2;
int tempSecteur3;

int meilleurTemps;

int tours;

/*  status == -1 => le processus est terminé
    status == 0 => la voiture est crashée et est donc retirée de la course
    status == 1 => la voiture est au stand
    status == 2 => la voiture est dans la course !
*/
int status;

/*
  permet de synchroniser les voitures entre elles (grâce au processus main)
  ready == 0 la voiture est en course
  ready == 1 la voiture est prête à partir
  ready == -1 la voiture est en attente
*/
int ready;

/* permet de savoir si le temps totale de la voiture a été mis à jour => car nécessitera une réorganisation du classement
*   changerOrdre == 0 (FALSE) le temps n'a pas été mis à jour
*   changerOrdre == 1 (TRUE) le temps a été mis à jour
*/

int changeOrdre;

}voiture;

typedef struct tuple{

int voitureNumber;
double voitureTime;
}tuple;


#endif // VOITURE_H_INCLUDED
