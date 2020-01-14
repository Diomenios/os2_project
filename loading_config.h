#ifndef LOADING_CONFIG_H_INCLUDED
#define LOADING_CONFIG_H_INCLUDED

/** structure permettant de garder les differentes information vitale a la 
*   parametrisation du programme
*
*/
typedef struct data_load {
  int nombreDeVoitures;
  int vitesseVoiture;
  int qualifOffset;
  int p1;
  int p2;
  int p3;
  int q1;
  int q2;
  int q3;
  int toursCourse;
  int s1;
  int s2;
  int s3;
  int dureeStand;
  int chanceStand;
  int chanceCrash;
}data;

/** lis le fichier de configuration ligne par ligne, repere les caracteres servant
*   de balises et lance les fonctions s'y rapportant
*
* @param data* programmeData  pointeur vers l'emplacement en memoire partagee ou doivent
*                             etre stockees les donnees
* @param char* file           string contenant le chemin vers le fichier de configuration a lire
*
* @return int*                chaine d'int contenant les numeros des voitures qui vont participer
*                             a la course
*
*/
int *configuration(data *programmeData, char *file);

/** va decomposer le string contenant les numeros en coupant au niveau des virgules,
*   et en "oubliant" les espaces.  La balise de fin est l'accolade fermante (ou quand
*   le nombre max de numero a ete lu =>  en cas d'erreur de l'utilisateur)
*
* @param char* buffer     String contenant les numeros a decrypter
* @param int tailleListe  nombre max de numero a retrouver
*
* @return int*            chaine d'int contenant les numeros des voitures qui vont participer
*                         a la course
*
*/
int *readNumerosVoitures(char *buffer, int tailleListe);

/** permet de lire les differentes donnees du fichier de config.  Ignore les espaces
*   et les %, et utilise le caractere "line feed" pour savoir quand arreter de lire
*   (ou la fin buffer en cas d'erreur)
*
* @param data* programmeData  pointeur vers l'emplacement en memoire partagee ou doivent
*                             etre stockees les donnees
* @param char* ligne          String contenant le nombre decrypter
* @param int commande         int permettant de savoir ou mettre le nombre decrypte
*
*/
void readOtherInformation(data *programmeData, char* ligne, int commande);

/** configure la parametrisation du programme par defaut en allant chercher les
*   differentes valeurs contenues dans le fichier constantes.h
*
* @param data* programmeData  pointeur vers l'emplacement en memoire partagee ou doivent
*                             etre stockees les donnees
*
* @return int*                chaine d'int contenant les numeros des voitures qui vont participer
*                             a la course
*
*/
int *defaultConfiguration(data *programmeData);

#endif // LOADING_CONFIG_H_INCLUDED
