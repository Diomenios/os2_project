#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <math.h>
#include <semaphore.h>
#include "constantes.h"
#include "voiture.h"
#include "affichage.h"
#include "saveLoad.h"

/*************************  fonctions du programme  ***************************/

void sm_initialisation(int nbr);//creation des semaphore
void sm_wait(int nbr);//lock les semphores
void sm_post(int nbr);//unlock les semaphores
void sm_destroy(int nbr);//fermeture des semaphores
int readMemory(int nombreEnfants, voiture *shm, int typeDeCourse);//lecture de la memoire
int readQualifMemory(int nombreEnfants, voiture *shm, int *typeDeCourse, tuple **classementDuo, int size);//lecture en memoire de la qualif
int readCourseMemory(int nombreEnfants, voiture *shm);//lecture en memoire de la course
void initFork(int incr,char *semid, char *mode,const int numeroVoiture[]);//initialise les forks
void save(int compteur);//sauvegarde
int modeCourse(char *argument_entrer);//lancer la course finale
void redemarrerVoiture(tuple **classement, int nombreVoiture, int typeDeCourse, int offset);//relancer une voiture
int mycmp(const void *s1, const void *s2);
int mytuplecmp(const void *s1, const void *s2);
int mycoursecmp(const void *s1, const void *s2);
tuple * initTuple(voiture *local, voiture *memory);

/*********************  variables globales du programme  **********************/

voiture copieMemoire[NOMBRE_DE_VOITURE];
voiture *classement[NOMBRE_DE_VOITURE];
static sem_t tabSem[NOMBRE_DE_VOITURE];
int periode[] = {P1, P2, P3};


sem_t * sem;//initialisation de la variable du semaphore
//initialisation du semaphore


/**
*  Le premier parametre permet de savoir le mode qu'on veut lancer :
*  P1 == essai 1, P2 == essai 2, P3 == essai 3, Q == qualification, Course == course finale
*  Le deuxieme parametre permet de savoir le nombre de kilometre que fait la partie S1
*  Le troisieme parametre permet de savoir le nombre de kilometre que fait la partie S2
*  Le quatrieme parametre permet de savoir le nombre de kilometre que fait la partie S3
*
*/
int main(int argc, char *argv[]){

	//initialisation du tableau de semaphores
	sm_initialisation(NOMBRE_DE_VOITURE);
  printf("%s\n", "init ok");

	//permet de determiner le type de course choisi par l'utilisateur
  int typeDeCourse = modeCourse(argv[1]);
  if (typeDeCourse == -1) {
    perror("le paramètre rentré en premier au programme n'est pas conforme !");
    exit(EXIT_FAILURE);
  }

  //initialisation de la mémoire partagée + rattachement
  int semid = shmget(IPC_PRIVATE, sizeof(voiture)*NOMBRE_DE_VOITURE, IPC_CREAT | 0666);
  voiture *shm = (voiture *) shmat(semid, NULL, 0);

	//copie la memoire partagee dans une copie locale
  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);

	//initialise un tableau de pointeur permettant le tri des voitures plus tard
  for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
    classement[i] = &copieMemoire[i];
  }

  //convertis le semid en char* pour le passer aux fils
  //10 parce qu'un int n'est jamais plus grand qu'un nombre à 10 chiffres
  char pass[10];
  sprintf(pass, "%d", semid);

  /****************************************************************************
  *                     gestion de la course en cours                         *
  ****************************************************************************/

  									/********  course d'essais  *********/
  if (typeDeCourse == 1 || typeDeCourse == 2 || typeDeCourse == 3) {
		gagnant pourSecteur[];
    initFork(NOMBRE_DE_VOITURE, pass, argv[1], VOITURE_NUMBER); // lance les voitures
    while (readMemory(NOMBRE_DE_VOITURE, shm, typeDeCourse)) {
      sleep(1);
    }
    save(typeDeCourse);
  }
  								/********  courses de qualification  *********/
  else if (typeDeCourse == 4){
    tuple *classementDuo[NOMBRE_DE_VOITURE];
    for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
      classementDuo[i] = initTuple(classement[i], &shm[i]);
      classementDuo[i]->local->id = i;
    }
    int size = sizeof(classementDuo);

    initFork(NOMBRE_DE_VOITURE, pass, argv[1], VOITURE_NUMBER); // lance les voitures
    while(readQualifMemory(NOMBRE_DE_VOITURE, shm, &typeDeCourse, classementDuo, size)){
      sleep(1);
    }

    for (int i = 0; i < NOMBRE_DE_VOITURE; i++) {
      free(classementDuo[i]);
    }
	}
  								/********  course principale  *********/
  else {

		//lecture du fichier de sauvegarde cree lors des qualifications
    int *listeVoiture = loading("F1_quali_save.txt");
    if (*listeVoiture == -1) {					//verifie que le fichier existe
      printf("%s\n", "le fichier de sauvegarde n'existe pas !  Veuillez d'abord lancer
											les courses de qualification");
      shmdt(shm);
      sm_destroy(NOMBRE_DE_VOITURE);
      exit(EXIT_FAILURE);
    }
    initFork(NOMBRE_DE_VOITURE, pass, argv[1], listeVoiture); // lance les voitures

    while (readCourseMemory(NOMBRE_DE_VOITURE, shm)) {
      sleep(1);
    }
    free(listeVoiture);
		save(typeDeCourse);
  }

  shmdt(shm);
  sm_destroy(NOMBRE_DE_VOITURE);
  exit(EXIT_SUCCESS);
}

/**********************************  fonctions Semaphore  ******************************************/

void sm_initialisation(int nbr){
	for(int i = 0; i < nbr ;i++){
		if(sem_init(&tabSem[i],0,1)!=0){//si erreur d'initialisation de semaphore
			exit(EXIT_FAILURE);
		}
	}
}
void sm_wait(int nbr){
	for(int i = 0; i < nbr ;i++){
		if(sem_wait(&tabSem[i])!=0){//si erreur d'initialisation de semaphore
			exit(EXIT_FAILURE);
		}
	}
}
void sm_post(int nbr){
	for(int i = 0; i < nbr ;i++){
		if(sem_post(&tabSem[i])!=0){//si erreur d'initialisation de semaphore
			exit(EXIT_FAILURE);
		}
	}
}
void sm_destroy(int nbr){
	for(int i = 0; i < nbr ;i++){
		if(sem_destroy(&tabSem[i])!=0){//si erreur d'initialisation de semaphore
			exit(EXIT_FAILURE);
		}
	}
}
/**********************************  fonctions auxiliaires  ******************************************/

/** copie en memoire local le contenu de la memoire partagee(section critique).
*		trie le classement de voiture si c'est necessaire (temps des tours ont change).
*		affiche le resultat du tri dans le terminal.  sauvegarde la session d'essais
*		si les voitures ont termine leur course
*
* @param int nombreEnfants le nombre de voiture present dans la course
* @param voiture* shm			 le pointeur vers l'emplacement en memoire partagee des voiture
*	@param int typeDeCourse	 nombre determinant le type de course dans laquelle on est.
*													 valeurs possibles : 1, 2 ou 3
*
*	@return int							 retourne les valeurs TRUE ou FALSE.  si FALSE est renvoye,
*													 la course est finie
*
*/
//TODO mettre le sémaphore
int readMemory(int nombreEnfants, voiture *shm, int typeDeCourse){
  typeDeCourse = 1;
  sm_wait(NOMBRE_DE_VOITURE);
  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);
  sm_post(NOMBRE_DE_VOITURE);
  int sorting = FALSE;
  int saveStatus = TRUE;

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
  }

  if (sorting) {
    qsort(classement, sizeof(classement)/sizeof(*classement), sizeof(*classement), mycmp);
  }
  afficherTableauScore(classement, typeDeCourse);

  if (saveStatus) {
    system("clear");
    return FALSE;
  }
  return TRUE;
}

/*******************  QUALIF  *****************************/

/** copie en memoire local le contenu de la memoire partagee(section critique).
*		trie le classement de voiture si c'est necessaire (temps des tours ont change).
*		affiche le resultat du tri dans le terminal.  sauvegarde la session d'essais
*		si les voitures ont termine leur course
*
* @param int nombreEnfants 			le nombre de voiture present dans la course
* @param voiture* shm			 			le pointeur vers l'emplacement en memoire partagee des voiture
* @param int* typeDeCourse 			pointeur permettant de savoir si on est en qualif 1, 2 ou 3
*																valeur de depart : 4, et sera incremente au cours du programme
*	@param tuple** classementDuo	pointeur vers un tableau de tuples contenant le classement
*																de la course en cours
* @param int size								la taille de classementDuo, evite les repetitions de sizeof()
*																lors de l'execution de la methode
*
*	@return int										retourne les valeurs TRUE ou FALSE.  si FALSE est renvoye,
*													 			la course est finie
*
*	N.B. le code est divise en 2 partie Q1 et {Q2, Q3}, car le fonctionnement est un
*			 peu different pour la partie 2 car on ne trie plus un tableau de voiture
*			 dans cette partie, mais un tableau de tuple (qui lie ensemble les pointeurs
*			 vers la memoire partagee et les pointeurs vers la memoire locale)
*
*/
//TODO mettre le sémaphore
int readQualifMemory(int nombreEnfants, voiture *shm, int *typeDeCourse, tuple **classementDuo, int size){
  int voiture_en_course;
  int sorting = FALSE;
  int saveStatus = TRUE;

  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
  }

/***              Q1            ****/
  if (*typeDeCourse == 4) {
    if (sorting) {
      qsort(classementDuo, size/sizeof(*classementDuo), sizeof(*classementDuo), mytuplecmp);
    }
    afficherTableauScoreQualif(classementDuo, *typeDeCourse);

    if (saveStatus) {
      system("clear");
      redemarrerVoiture(classementDuo, nombreEnfants, *typeDeCourse, 3);
      *typeDeCourse +=1;
      return TRUE;
    }
    return TRUE;
  }
/***            Q2 et Q3        ****/
  else if (*typeDeCourse == 5) {
    voiture_en_course = 6;
  }
  else{
    voiture_en_course = 3;
  }

  tuple *voiture_qualif[voiture_en_course];

  memcpy(voiture_qualif, classementDuo, sizeof(tuple*)*voiture_en_course);
  if (sorting) {
    qsort(voiture_qualif, sizeof(voiture_qualif)/sizeof(*voiture_qualif), sizeof(*voiture_qualif), mytuplecmp);
  }
  qsort(voiture_qualif, sizeof(voiture_qualif)/sizeof(*voiture_qualif), sizeof(*voiture_qualif), mytuplecmp);
  memcpy(classementDuo, voiture_qualif, sizeof(tuple*)*voiture_en_course);

  afficherTableauScoreQualif(classementDuo, *typeDeCourse);

  if (saveStatus && *typeDeCourse == 6) {
    system("clear");
    printf("%s\n", "sauvegarde des qualifications");
    saveQuali(classementDuo);
    return FALSE;
  }
  if (saveStatus) {
    system("clear");
    redemarrerVoiture(classementDuo, nombreEnfants, *typeDeCourse, 3);
    *typeDeCourse +=1;
    return TRUE;
  }
  return TRUE;
}

/*******************************  COURSE  *************************************/

/** copie en memoire local le contenu de la memoire partagee(section critique).
*		trie le classement de voiture si c'est necessaire (temps des tours ont change).
*		affiche le resultat du tri dans le terminal.  sauvegarde la session de course
*		si les voitures ont termine de concourir
*
* @param int nombreEnfants le nombre de voiture present dans la course
* @param voiture* shm			 le pointeur vers l'emplacement en memoire partagee des voiture
*
*	@return int							 retourne les valeurs TRUE ou FALSE.  si FALSE est renvoye,
*													 la course est finie
*
*/
//TODO mettre le sémaphore
int readCourseMemory(int nombreEnfants, voiture *shm){
  sm_wait(NOMBRE_DE_VOITURE);
  memcpy(copieMemoire, shm, sizeof(voiture)*NOMBRE_DE_VOITURE);
  sm_post(NOMBRE_DE_VOITURE);
  int sorting = FALSE;
  int saveStatus = TRUE;

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
  }

  if (sorting) {
    qsort(classement, sizeof(classement)/sizeof(*classement), sizeof(*classement), mycoursecmp);
  }
  afficherTableauScoreCourse(classement, 7);

  if (saveStatus) {
    system("clear");
    return FALSE;
  }
  return TRUE;
}

/**fork le nomnbre de fois demande, et lance le programme voiture dans un processus
*	 fils.
*
*	@param int incr 								le nombre de voiture desiree
*	@param char* semid 							un nombre (sous forme de char) qui permet de retrouver
*																	la memoire partagee a partir de shmat()
*	@param char* mode								le type de course que la voiture doit effectuer
																	valeurs possibles : P1, P2, P3, Q, Course
* @param const int* numeroVoiture le tableau contenant les numeros (id) des voitures
*																	a lancer dans la course
*
*/
void initFork(int incr,char *semid, char *mode,const int numeroVoiture[]){

  for (int i = 0; i < incr; i++) {
		printf("%s\n", "fork");
    if (fork() == 0) {
      char pass[3];
      char id[3];

      sprintf(pass, "%d", i);
      sprintf(id, "%d", numeroVoiture[i]);

      char *voiture[]= {"./voiture",semid,pass,mode,id,NULL};
      execvp(voiture[0], voiture);
    }
  }
}

/**utilise les methodes de saveLoad.c pour sauvegarder les resultats des courses
*	 determine le type de course dans laquelle on se trouve
*
*	@param int compteur le numero permettant de savoir le type de course que l'on a effectuee
*
*/
void save(int compteur){
  switch (compteur){
    case 1:
      saveEssai(compteur, P1, classement);
      break;
    case 2:
      saveEssai(compteur, P2, classement);
      break;
    case 3:
      saveEssai(compteur, P3, classement);
      break;
    case 7:
      saveCourse(classement);
      break;
    default :
      printf("%s\n", "erreur dans la sauvegarde");
      exit(EXIT_FAILURE);
  }
}

/**convertis un string en un nombre permettant de determminer le type de course effectuee.
*	 si l'entree est invalide, retourne -1
*
*	@param char* argument_entrer le string que la methode doit convertir
*							 valeurs autorisees : P1, P2, P3, Q, Course
*
* @return int	 retourne 1, 2, 3, 4 ou 7 si jamais la convertion a reussi, sinon
							 retourne -1 en cas d'echec de convertion
*
*/
int modeCourse(char *argument_entrer){
  if (strcmp(argument_entrer, "P1") == 0){
    return 1;
  }
  if (strcmp(argument_entrer, "P2") == 0){
    return 2;
  }
  if (strcmp(argument_entrer, "P3") == 0){
    return 3;
  }
  if (strcmp(argument_entrer, "Q") == 0){
    return 4;
  }
  if (strcmp(argument_entrer, "Course") == 0){
    return 7;
  }
  else{
    return -1;
  }
}

/**lors des qualifications, relance les voitures lors de la phase de qualification 1
*	 ou 2.  ne relance qu'une partie des voitures pour simuler la disqualification des voitures
*	 les plus lentes
*
*	@param tuple** classement	pointeur vers un tableau de tuples contenant le classement triee des voitures
*	@param int nombreVoiture	le nombre de voitures concourant dans la course
* @param int typeDeCourse		nombre permettant de savoir si on est en qualif 1 ou 2 (valeurs : 4 ou 5)
* @param int offset					nombre de voiture etant eliminee a chaque fin de periode de qualif
*
*/
void redemarrerVoiture(tuple **classement, int nombreVoiture, int typeDeCourse, int offset){
  if (typeDeCourse == 4) {
    for (int i = 0; i < nombreVoiture; i++) {
      classement[i]->memory->ready = TRUE;
      if (i>=nombreVoiture-offset) {
        classement[i]->memory->status = -1;
      }
    }
  }
  else{
    for (int i = 0; i < nombreVoiture-offset; i++) {
      classement[i]->memory->ready = TRUE;
      if (i>=nombreVoiture-2*offset) {
        classement[i]->memory->status = -1;
      }
    }
  }
}

/**comparateur utilise pour le tri de voiture**
*	 compare sur le meilleurTemps
*
*	@param const void* s1 l'element de gauche
* @param const void* s2 l'element de droite
*
* @return int						retourne -1(plus petit), 0(egal) ou 1(plus grand)
*
*/
int mycmp(const void *s1, const void *s2) {
    const voiture *l = *(const voiture **)s1;
    const voiture *r = *(const voiture **)s2;

    int mtl = l->meilleurTemps;
    int mtr = r->meilleurTemps;

    if (mtl < mtr) return -1;
    if (mtl > mtr) return 1;
    return 0;
}

/**comparateur utilise pour le tri de tuple**
*	 compare sur le meilleurTemps
*
*	@param const void* s1 l'element de gauche
* @param const void* s2 l'element de droite
*
* @return int						retourne -1(plus petit), 0(egal) ou 1(plus grand)
*
*/
int mytuplecmp(const void *s1, const void *s2){
  const tuple *l = *(const tuple **)s1;
  const tuple *r = *(const tuple **)s2;

  int mtl = l->local->meilleurTemps;
  int mtr = r->local->meilleurTemps;

  if (mtl < mtr) return -1;
  if (mtl > mtr) return 1;
  return 0;
}

/**comparateur utilise pour le tri de voiture**
*	 compare sur le meilleurTemps
*
*	@param const void* s1 l'element de gauche
* @param const void* s2 l'element de droite
*
* @return int						retourne -1(plus petit), 0(egal) ou 1(plus grand)
*
*/
int mycoursecmp(const void *s1, const void *s2) {
    const voiture *l = *(const voiture **)s1;
    const voiture *r = *(const voiture **)s2;

    int mtl = l->tempsTotal;
    int mtr = r->tempsTotal;

    if (mtl < mtr) return -1;
    if (mtl > mtr) return 1;
    return 0;
}

/** initialise un tuple
*		ATTENTION : il faut penser a faire un free sur le resultat de la methode
*								une fois celui-ci plus necessaire
*
*	@param voiture* local
*	@param voiture*	memory
*
* @return tuple* le tuple initialise
*
*/
tuple * initTuple(voiture *local, voiture *memory){

  tuple *new = (tuple*) malloc(sizeof(tuple));

  new->local = local;
  new->memory = memory;

  return new;
}
