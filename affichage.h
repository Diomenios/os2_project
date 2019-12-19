#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED

/** Fonction principale : permet de creer puis d'afficher le tableau dans le
*         terminal a l'aide d'une serie de concatenantions
*
* @param  voiture* classement[] un pointeur vers un tableau de voiture triees
* @param  int manche            un numero indiquant la partie de la serie de course
*                               dans laquelle on est
*/
void afficherTableauScore(voiture *classement[], int manche, gagnant *secteurs);

/* Fonction d'affichage pour les qualifications : permet de creer puis d'afficher
*         le tableau dans le terminal a l'aide d'une serie de concatenantions
*
*@param tuple* classement[] un pointeur vers un tableau de voiture triees en memoire partagee et locale
*@param int manche          un numero indiquant la partie de la serie de course dans laquelle
*                           on est
* N.B: tuple utilise pour gestion des pointeurs entre les pointeurs de la memoire locale et partagee
*/
void afficherTableauScoreQualif(tuple *classement[], int manche, gagnant *secteurs);

/* Fonction d'affichage pour la course : permet de creer puis d'afficher
*         le tableau dans le terminal a l'aide d'une serie de concatenantions
*
* @param  voiture* classement[] un pointeur vers un tableau de voiture triees
* @param  int manche            un numero indiquant la partie de la serie de course
*                               dans laquelle on est
*/
void afficherTableauScoreCourse(voiture *classement[], int manche, gagnant *secteurs);

/** Permet de generer et de remplir un case pour le tableau.
*  ATTENTION : necessite de faire un free apres utilisation car on utilise un malloc
*
* @param  int tailleCellule la taille de la cellule qu'on desire creer
* @param  int input         nombre que l'on desire introduire dans la cellule
*                           si le nombre == 0, on le remplacera par le string NA
* @param  int sizeInput     le nombre de chiffres constituant le parametre input
* @return                   renvoie un poiteur vers la zone memoire contenant notre cellule
*/
char* creationCelluleNombre(int tailleCellule, int input, int sizeInput);

/** Permet de choisir entre les differents etats possibles pour la voiture a savoir
*  OUT, P ou GO
*
* @param  int status  le status de la voiture sous format numerique
* @param  int crash   booleen indiquant si une voiture est crashee
* @return             retourne une chaine de caracteres de bonne taille pour la cellule
*/
char* decodageStatus(int status, int crash);

/** Permet de creer le titre du tableau.  Selectionne le titre correcte pour la
*  course en cours
*
* @param  int manche  un nombre permettant de determiner la course dans laquelle on est.
*                     Doit etre compris entre 1 et 7 inclus
* @return             renvoie un poiteur vers la zone memoire contenant notre ligne
*/
char* creationManche(int manche);

#endif // AFFICHAGE_H_INCLUDED
