#ifndef VOITURE_H_INCLUDED
#define VOITURE_H_INCLUDED
/*
*Structure d'une voiture avec ses caracteristiques
*
*/
typedef struct voiture {
	int id;							//numero de la voiture

	int tempSecteur1;
	int tempSecteur2;
	int tempSecteur3;

	int meilleurTemps;	//meilleur temps pour un tour complet
	int tempsTotal;			//temps de tous les tours effectue cumule

	int tours;

/*
    status == 0 => le processus est termine
    status == 1 => la voiture est au stand
    status == 2 => la voiture est dans la course !
*/
	int status;

/*
  permet de synchroniser les voitures entre elles (grace au processus main)
  ready == 0 la voiture est en course
  ready == 1 la voiture est prete a partir
  ready == -1 la voiture est en attente
  ready == 2 la voiture ne participe pas a la course
*/
	int ready;

/* permet de savoir si le temps totale de la voiture a ete mis à jour => car necessitera une reorganisation du classement
*   changerOrdre == 0 (FALSE) le temps n'a pas ete mis a jour
*   changerOrdre == 1 (TRUE) le temps a ete mis à jour
*/
	int changeOrdre;
	/* permet de savoir si la voiture est crashee
	*   crash == 0 (FALSE) la voiture est toujours en course
	*   crash == 1 (TRUE) la voiture est OUT
	*/
	int crash;
	int passageAuStand;
}voiture;


/*
*
*tuple = type de donnees compose servant à stocker une collection d'elements
*
* Permet de lier l'emplacement memoire local d'une voiture avec l'emplacement en
* memoire partagee de sa copie
*/
typedef struct tuple{

voiture *memory;
voiture *local;
}tuple;

/*
* permet de stocker le numéro d'une voiture ainsi que son temps.  Ici c'est pour garder
* la voiture qui a fait le meilleur temps dans un secteur, ainsi que le temps qu'elle a fait
*
*/
typedef struct gagnant{
  int voitureId;
  int voitureTemps;
}gagnant;
/*
*
*
*
*/
#endif // VOITURE_H_INCLUDED
