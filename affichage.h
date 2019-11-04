#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED

void afficherTableauScore(voiture *classement[], int manche);
char* creationCelluleNombre(int tailleCellule, int input, int sizeInput);
char* decodageStatus(int status);
char* creationManche(int manche, int len);

#endif // AFFICHAGE_H_INCLUDED
