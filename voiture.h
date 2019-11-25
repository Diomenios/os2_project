#ifndef VOITURE_H_INCLUDED
#define VOITURE_H_INCLUDED

typedef struct voiture {
int id;

int tempSecteur1;
int tempSecteur2;
int tempSecteur3;

int meilleurTemps;
int tempsTotal;

int tours;

/*  status == -1 => le processus est termine
    status == 0 => la voiture est crashze et est donc retiree de la course
    status == 1 => la voiture est au stand
    status == 2 => la voiture est dans la course !
*/
int status;

/*
  permet de synchroniser les voitures entre elles (grace au processus main)
  ready == 0 la voiture est en course
  ready == 1 la voiture est prete a partir
  ready == -1 la voiture est en attente
  readi == 2 la voiture ne participe pas a la course
*/
int ready;

/* permet de savoir si le temps totale de la voiture a ete mis à jour => car necessitera une reorganisation du classement
*   changerOrdre == 0 (FALSE) le temps n'a pas ete mis a jour
*   changerOrdre == 1 (TRUE) le temps a ete mis à jour
*/


int changeOrdre;
}voiture;

typedef struct tuple{

voiture *memory;
voiture *local;
}tuple;


#endif // VOITURE_H_INCLUDED
