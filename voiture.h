#ifndef VOITURE_H_INCLUDED
#define VOITURE_H_INCLUDED

typedef struct voiture {
int id;

double tempSecteur1;
double tempSecteur2;
double tempSecteur3;

double temp1Total;

double temp2Total;

double temp3Total;


/*  status == -1 => le processus est terminé
    status == 0 => la voiture est crashée et est donc retirée de la course
    status == 1 => la voiture est au stand
    status == 2 => la voiture est dans la course !
*/
int status;

int classement;

/*
  permet de synchroniser les voitures entre elles (grâce à la main)
  ready = 0 la voiture est en course
  ready = 1 la voiture est prête à partir
  ready = -1 la voiture
*/
int ready;

}voiture;


#endif // VOITURE_H_INCLUDED
