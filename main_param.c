#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <math.h>
#include <fcntl.h>           /* Pour les constantes O_* */
#include <sys/stat.h>        /* Pour les constantes « mode » */
#include <semaphore.h>
#include <limits.h>
#include "constantes.h"
#include "voiture.h"
#include "affichage.h"
#include "saveLoad.h"
#include "helper.h"
#include "loading_config.h"

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define BASE_NOM_SEMAPHORE "semi"
#define CLOCK 1

/*************************  fonctions du programme  ***************************/

char **semaphores_name_init(int nbr, char *name, int taille);
void tableau_string_free(char **tableauNoms, int sizeOfTableau);
void sm_initialisation(int nbr, sem_t *tableau_semaphores[], char **noms);//creation des semaphore
void sm_wait(int nbr, sem_t *tableau_semaphores[]);//lock les semphores
void sm_post(int nbr, sem_t *tableau_semaphores[]);//unlock les semaphores
void sm_unlink(int nbr, char *tableau_semaphores[]);//fermeture des semaphores
int readMemory(int nombreEnfants, voiture *shm, int typeDeCourse, sem_t **tableau_semaphores, voiture *copieMemoire, voiture **classement);//lecture de la memoire
int readQualifMemory(int nombreEnfants, int qualifOffset, voiture *shm, int *typeDeCourse, tuple **classementDuo, int size, sem_t **tableau_semaphores, voiture *copieMemoire, voiture **classement);//lecture en memoire de la qualif
int readCourseMemory(int nombreEnfants, voiture *shm, int *meilleurId, int *meilleurIdTemps, sem_t **tableau_semaphores, voiture *copieMemoire, voiture **classement);//lecture en memoire de la course
void initFork(int nombreEnfants,char *semid, char *mode, char *tableauNoms[], int dataID, int *numeroDesVoitures);//initialise les forks
void save(int compteur, data *programmeData, voiture *copieMemoire, voiture **classement);//sauvegarde
int modeCourse(char *argument_entrer);//lancer la course finale
void redemarrerVoiture(tuple **classement, int nombreVoiture, int typeDeCourse, int offset);//relancer une voiture
int mycmp(const void *s1, const void *s2);
int mytuplecmp(const void *s1, const void *s2);
int mycoursecmp(const void *s1, const void *s2);
tuple * initTuple(voiture *local, voiture *memory);
void initGagnant(gagnant *secteur);
int totalCrashDetection(int nombreEnfants, voiture *copieMemoire);
int overCrashDetection(int nombreEnfants, int overCrash, voiture *copieMemoire);

/*********************  variable globale du programme  **********************/
gagnant meilleursSecteur[3];

/**
*  Le premier parametre permet de savoir le mode qu'on veut lancer :
*  P1 == essai 1, P2 == essai 2, P3 == essai 3, Q == qualification, Course == course finale
*  Le deuxieme parametre permet de savoir le nombre de kilometre que fait la partie S1
*  Le troisieme parametre permet de savoir le nombre de kilometre que fait la partie S2
*  Le quatrieme parametre permet de savoir le nombre de kilometre que fait la partie S3
*
*/
int main(int argc, char *argv[]){

	int dataID;
	int *numeroDesVoitures;
	data *programmeData;

	//initialisation du programme
	if (argc<2) {
		printf("%s\n", "veuillez au minimum déterminer le type de course que vous voulez lancer(p1, p2, p3, q, c)");
		printf("%s\n", "Où : p1 = première période d'essai, p2 = deuxième période d'essai, p3 = troisième période d'essai");
		printf("%s\n", "     q = les qualifications, c = la course (fichier des résultats des qualif requis)");
		exit(EXIT_SUCCESS);
	}

	dataID = shmget(IPC_PRIVATE, sizeof(data), IPC_CREAT | 0666);
	programmeData = (data *) shmat(dataID, NULL, 0);

	if (argc < 3) {
		printf("%s\n", "chargement des configurations par défaut");
		numeroDesVoitures = defaultConfiguration(programmeData);
		sleep(0.5);
	}
	else{
		printf("%s%s\n", "chargement du fichier de configuration : ", argv[2]);
		sleep(0.5);
		numeroDesVoitures = configuration(programmeData, argv[2]);
		if (programmeData == NULL) {
			exit(EXIT_SUCCESS);
		}
	}

	/****** initialisation des variables *********/
	sem_t *tabSem[programmeData->nombreDeVoitures];
	int typeDeCourse = modeCourse(argv[1]);
	int semid;
	voiture *shm;
	char **tableauNoms;
	sem_t *tableau_semaphores[programmeData->nombreDeVoitures];
	voiture copieMemoire[programmeData->nombreDeVoitures];
	voiture *classement[programmeData->nombreDeVoitures];

	//permet de determiner le type de course choisi par l'utilisateur
  if (typeDeCourse == -1) {
    perror("le paramètre rentré en premier au programme n'est pas conforme !");
    exit(EXIT_SUCCESS);
  }

  //initialisation de la mémoire partagée + rattachement
  semid = shmget(IPC_PRIVATE, sizeof(voiture)*programmeData->nombreDeVoitures, IPC_CREAT | 0666);

  shm = (voiture *) shmat(semid, NULL, 0);

	tableauNoms = semaphores_name_init(programmeData->nombreDeVoitures, BASE_NOM_SEMAPHORE, strlen(BASE_NOM_SEMAPHORE)+(int)log10(programmeData->nombreDeVoitures)+1);


	sm_initialisation(programmeData->nombreDeVoitures, tableau_semaphores, tableauNoms);
	initGagnant(meilleursSecteur);
	//copie la memoire partagee dans une copie locale
  memcpy(copieMemoire, shm, sizeof(voiture)*programmeData->nombreDeVoitures);

	//initialise un tableau de pointeur permettant le tri des voitures plus tard
  for (int i = 0; i < programmeData->nombreDeVoitures; i++) {
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
		for (int i = 0; i < programmeData->nombreDeVoitures; i++) {
			shm[i].id = numeroDesVoitures[i];
		}
    initFork(programmeData->nombreDeVoitures, pass, argv[1], tableauNoms, dataID, numeroDesVoitures); // lance les voitures
    while (readMemory(programmeData->nombreDeVoitures, shm, typeDeCourse, tableau_semaphores, copieMemoire, classement)) {
      sleep(CLOCK);
    }
    save(typeDeCourse, programmeData, copieMemoire, classement);
  }
  								/********  courses de qualification  *********/
  else if (typeDeCourse == 4){
		for (int i = 0; i < programmeData->nombreDeVoitures; i++) {
			shm[i].id = numeroDesVoitures[i];
		}
    tuple *classementDuo[programmeData->nombreDeVoitures];
    for (int i = 0; i < programmeData->nombreDeVoitures; i++) {
      classementDuo[i] = initTuple(classement[i], &shm[i]);
      classementDuo[i]->local->id = i;
    }
    int size = sizeof(classementDuo);

    initFork(programmeData->nombreDeVoitures, pass, argv[1], tableauNoms, dataID, numeroDesVoitures); // lance les voitures

    while(readQualifMemory(programmeData->nombreDeVoitures, programmeData->qualifOffset, shm, &typeDeCourse, classementDuo, size, tableau_semaphores, copieMemoire, classement)){
      sleep(CLOCK);
    }

    for (int i = 0; i < programmeData->nombreDeVoitures; i++) {
      free(classementDuo[i]);
    }
	}
  								/********  course principale  *********/
  else {

		//lecture du fichier de sauvegarde cree lors des qualifications
    int *listeVoiture = loading("F1_quali_save.txt", programmeData->nombreDeVoitures);
    if (*listeVoiture == -1) {					//verifie que le fichier existe
      printf("%s\n", "le fichier de sauvegarde n'existe pas !  Veuillez d'abord lancer les courses de qualification");
      shmdt(shm);
      sm_unlink(programmeData->nombreDeVoitures, tableauNoms);
			tableau_string_free(tableauNoms, programmeData->nombreDeVoitures);
			free(numeroDesVoitures);
			shmdt(programmeData);
      exit(EXIT_FAILURE);
    }
		for (int i = 0; i < programmeData->nombreDeVoitures; i++) {
			shm[i].id = listeVoiture[i];
		}
    initFork(programmeData->nombreDeVoitures, pass, argv[1], tableauNoms, dataID, listeVoiture); // lance les voitures

		int meilleurId = -1;
		int meilleurIdTemps = INT_MAX;
    while (readCourseMemory(programmeData->nombreDeVoitures, shm, &meilleurId, &meilleurIdTemps, tableau_semaphores, copieMemoire, classement)) {
      sleep(CLOCK);
    }
    free(listeVoiture);
		saveCourse(classement, meilleursSecteur, meilleurIdTemps, meilleurId, programmeData->nombreDeVoitures);
  }

  shmdt(shm);
  sm_unlink(programmeData->nombreDeVoitures, tableauNoms);
	tableau_string_free(tableauNoms, programmeData->nombreDeVoitures);
	free(numeroDesVoitures);
	shmdt(programmeData);
  exit(EXIT_SUCCESS);
}

/**********************************  fonctions Semaphore  ******************************************/

char **semaphores_name_init(int nbr, char *name, int taille){
	if (taille < strlen(name)+(int)log10(nbr)+1) {
		printf("%s\n", "erreur,la taille passée n'est pas correcte veuillez recommencer !");
		return NULL;
	}
	char **retour = malloc(nbr*sizeof(char*));

	for(int i = 0; i < nbr ;i++){

		retour[i] = malloc((taille+1)* sizeof(char));
		char buffer[(int)log10(nbr)+1];

		strcpy(retour[i], name);
		sprintf(buffer, "%d", i);
		strcat(retour[i], buffer);
	}

	return retour;
}

void tableau_string_free(char **tableauNoms, int sizeOfTableau){
	for (size_t i = 0; i < sizeOfTableau; i++) {
		free(tableauNoms[i]);
	}
	free(tableauNoms);
}

void sm_initialisation(int nbr, sem_t *tableau_semaphores[], char **noms){
	for(int i = 0; i < nbr ;i++){
		tableau_semaphores[i] = sem_open(noms[i], O_CREAT | O_EXCL, SEM_PERMS, 1);

		if (tableau_semaphores[i] == SEM_FAILED) {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
	}
}

void sm_wait(int nbr, sem_t *tableau_semaphores[]){
	for(int i = 0; i < nbr ;i++){
		if(sem_wait(tableau_semaphores[i])!=0){//si erreur d'initialisation de semaphore
			perror("sem_wait error");
			exit(EXIT_FAILURE);
		}
	}
}
void sm_post(int nbr, sem_t *tableau_semaphores[]){
	for(int i = 0; i < nbr ;i++){
		if(sem_post(tableau_semaphores[i])!=0){//si erreur d'initialisation de semaphore
			perror("sem_post error");
			exit(EXIT_FAILURE);
		}
	}
}
void sm_unlink(int nbr, char *tableau_semaphores[]){
	for(int i = 0; i < nbr ;i++){
		if(sem_unlink(tableau_semaphores[i])!=0){//si erreur d'initialisation de semaphore
			perror("sem_close error");
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
* @param int nombreEnfants 						le nombre de voiture present dans la course
* @param voiture* shm			 						le pointeur vers l'emplacement en memoire partagee des voiture
*	@param int typeDeCourse	 						nombre determinant le type de course dans laquelle on est.
*													 						valeurs possibles : 1, 2 ou 3
*	@param sem_t** tableau_semaphores		le tableau contenant les semaphores
* @param voiture* copieMemoire				pointeur vers la copie en memoire locale de la memoire partagee
* @param voiture** classement					tableau de pointeurs contenant les copies des adresses memoires
*																			de copieMemoire
*
*	@return int							 retourne les valeurs TRUE ou FALSE.  si FALSE est renvoye,
*													 la course est finie
*
*/
int readMemory(int nombreEnfants, voiture *shm, int typeDeCourse, sem_t **tableau_semaphores, voiture *copieMemoire, voiture **classement){

	//zone critique
	sm_wait(nombreEnfants, tableau_semaphores);
	memcpy(copieMemoire, shm, sizeof(voiture)*nombreEnfants);
	sm_post(nombreEnfants, tableau_semaphores);

  int sorting = FALSE;
  int saveStatus = TRUE;

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
		if (copieMemoire[i].tempSecteur1 > 0 && copieMemoire[i].tempSecteur1 < meilleursSecteur[0].voitureTemps) {
			meilleursSecteur[0].voitureTemps = copieMemoire[i].tempSecteur1;
			meilleursSecteur[0].voitureId = copieMemoire[i].id;
		}
		if (copieMemoire[i].tempSecteur2 > 0 && copieMemoire[i].tempSecteur2 < meilleursSecteur[1].voitureTemps) {
			meilleursSecteur[1].voitureTemps = copieMemoire[i].tempSecteur2;
			meilleursSecteur[1].voitureId = copieMemoire[i].id;
		}
		if (copieMemoire[i].tempSecteur3 > 0 && copieMemoire[i].tempSecteur3 < meilleursSecteur[2].voitureTemps) {
			meilleursSecteur[2].voitureTemps = copieMemoire[i].tempSecteur3;
			meilleursSecteur[2].voitureId = copieMemoire[i].id;
		}
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
  }

  if (sorting) {
		voiture *sort[nombreEnfants];
		memcpy(sort, classement, sizeof(voiture *) * nombreEnfants);
    qsort(sort, sizeof(sort)/sizeof(*sort), sizeof(*sort), mycmp);
		memcpy(classement, sort, sizeof(voiture *) * nombreEnfants);
  }
  afficherTableauScore(classement, typeDeCourse, meilleursSecteur, nombreEnfants);

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
* @param int nombreEnfants 					le nombre de voiture present dans la course
* @param int qualifOffset 					le nombre de voiture éliminée à chaque manche
* @param voiture* shm			 					le pointeur vers l'emplacement en memoire partagee des voiture
* @param int* typeDeCourse 					pointeur permettant de savoir si on est en qualif 1, 2 ou 3
*																		valeur de depart : 4, et sera incremente au cours du programme
*	@param tuple** classementDuo			pointeur vers un tableau de tuples contenant le classement
*																		de la course en cours
* @param int size										la taille de classementDuo, evite les repetitions de sizeof()
*																		lors de l'execution de la methode
*	@param sem_t** tableau_semaphores	le tableau contenant les semaphores
* @param voiture* copieMemoire			pointeur vers la copie en memoire locale de la memoire partagee
* @param voiture** classement				tableau de pointeurs contenant les copies des adresses memoires
*																		de copieMemoire
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
int readQualifMemory(int nombreEnfants, int qualifOffset, voiture *shm, int *typeDeCourse, tuple **classementDuo, int size, sem_t **tableau_semaphores, voiture *copieMemoire, voiture** classement){
  int voiture_en_course;
  int sorting = FALSE;
  int saveStatus = TRUE;

	//zone critique
	sm_wait(nombreEnfants, tableau_semaphores);
	memcpy(copieMemoire, shm, sizeof(voiture)*nombreEnfants);
	sm_post(nombreEnfants, tableau_semaphores);

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
		if (copieMemoire[i].tempSecteur1 > 0 && copieMemoire[i].tempSecteur1 < meilleursSecteur[0].voitureTemps) {
			meilleursSecteur[0].voitureTemps = copieMemoire[i].tempSecteur1;
			meilleursSecteur[0].voitureId = copieMemoire[i].id;
		}
		if (copieMemoire[i].tempSecteur2 > 0 && copieMemoire[i].tempSecteur2 < meilleursSecteur[1].voitureTemps) {
			meilleursSecteur[1].voitureTemps = copieMemoire[i].tempSecteur2;
			meilleursSecteur[1].voitureId = copieMemoire[i].id;
		}
		if (copieMemoire[i].tempSecteur3 > 0 && copieMemoire[i].tempSecteur3 < meilleursSecteur[2].voitureTemps) {
			meilleursSecteur[2].voitureTemps = copieMemoire[i].tempSecteur3;
			meilleursSecteur[2].voitureId = copieMemoire[i].id;
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
    afficherTableauScoreQualif(classementDuo, *typeDeCourse, meilleursSecteur, nombreEnfants);
    if (saveStatus) {
      system("clear");
			if (overCrashDetection(nombreEnfants, qualifOffset, copieMemoire)) {
				printf("%s\n", "Trop de voitures se sont crashée, les qualifications ne sont plus possibles !  aucunes données n'a été sauvegardée");
				return FALSE;
			}
			else{
				redemarrerVoiture(classementDuo, nombreEnfants, *typeDeCourse, qualifOffset);
	      *typeDeCourse +=1;
	      return TRUE;
			}
    }
    return TRUE;
  }
/***            Q2 et Q3        ****/
  else if (*typeDeCourse == 5) {
    voiture_en_course = nombreEnfants - qualifOffset;
  }
  else{
    voiture_en_course = nombreEnfants - 2*qualifOffset;
  }

  tuple *voiture_qualif[voiture_en_course];

  memcpy(voiture_qualif, classementDuo, sizeof(tuple*)*voiture_en_course);
  if (sorting) {
    qsort(voiture_qualif, sizeof(voiture_qualif)/sizeof(*voiture_qualif), sizeof(*voiture_qualif), mytuplecmp);
  }
  qsort(voiture_qualif, sizeof(voiture_qualif)/sizeof(*voiture_qualif), sizeof(*voiture_qualif), mytuplecmp);
  memcpy(classementDuo, voiture_qualif, sizeof(tuple*)*voiture_en_course);

  afficherTableauScoreQualif(classementDuo, *typeDeCourse, meilleursSecteur, nombreEnfants);

  if (saveStatus && *typeDeCourse == 6) {
    system("clear");
		if (overCrashDetection(voiture_en_course, qualifOffset, copieMemoire)) {
			printf("%s\n", "Trop de voitures se sont crashée, les qualifications ne sont plus possibles !  aucunes données n'a été sauvegardée");
		}
		else{
				saveQuali(classementDuo, meilleursSecteur, nombreEnfants);
		}
    return FALSE;
  }
  if (saveStatus) {
    system("clear");
		if (overCrashDetection(voiture_en_course, qualifOffset, copieMemoire)) {
			printf("%s\n", "Trop de voitures se sont crashée, les qualifications ne sont plus possibles !  aucunes données n'a été sauvegardée");
			return FALSE;
		}
		else{
			redemarrerVoiture(classementDuo, nombreEnfants, *typeDeCourse, qualifOffset);
	    *typeDeCourse +=1;
	    return TRUE;
		}
  }
  return TRUE;
}

/*******************************  COURSE  *************************************/

/** copie en memoire local le contenu de la memoire partagee(section critique).
*		trie le classement de voiture si c'est necessaire (temps des tours ont change).
*		affiche le resultat du tri dans le terminal.  sauvegarde la session de course
*		si les voitures ont termine de concourir
*
* @param int nombreEnfants 						le nombre de voiture present dans la course
* @param voiture* shm			 						le pointeur vers l'emplacement en memoire partagee des voiture
*	@param int*	meilleurId	 						pointeur vers la memoire contenant l'id de la voiture avec
*													 						le meilleur temps
*	@param int* meilleurIdTemps					pointeur vers la memoire contenant meilleur temps pour
*																			un tour
*	@param sem_t** tableau_semaphores		le tableau contenant les semaphores
* @param voiture* copieMemoire				pointeur vers la copie en memoire locale de la memoire partagee
* @param voiture** classement					tableau de pointeurs contenant les copies des adresses memoires
*																			de copieMemoire
*
*	@return int							 retourne les valeurs TRUE ou FALSE.  si FALSE est renvoye,
*													 la course est finie
*
*/
int readCourseMemory(int nombreEnfants, voiture *shm, int *meilleurId, int *meilleurIdTemps, sem_t **tableau_semaphores, voiture *copieMemoire, voiture **classement){

	//zone critique
	sm_wait(nombreEnfants, tableau_semaphores);
	memcpy(copieMemoire, shm, sizeof(voiture)*nombreEnfants);
	sm_post(nombreEnfants, tableau_semaphores);

  int sorting = FALSE;
  int saveStatus = TRUE;

  for (int i = 0; i < nombreEnfants; i++) {
    if(copieMemoire[i].changeOrdre){
      copieMemoire[i].changeOrdre = FALSE;
      sorting = TRUE;
    }
		if (copieMemoire[i].tempSecteur1 > 0 && copieMemoire[i].tempSecteur1 < meilleursSecteur[0].voitureTemps) {
			meilleursSecteur[0].voitureTemps = copieMemoire[i].tempSecteur1;
			meilleursSecteur[0].voitureId = copieMemoire[i].id;
		}
		if (copieMemoire[i].tempSecteur2 > 0 && copieMemoire[i].tempSecteur2 < meilleursSecteur[1].voitureTemps) {
			meilleursSecteur[1].voitureTemps = copieMemoire[i].tempSecteur2;
			meilleursSecteur[1].voitureId = copieMemoire[i].id;
		}
		if (copieMemoire[i].tempSecteur3 > 0 && copieMemoire[i].tempSecteur3 < meilleursSecteur[2].voitureTemps) {
			meilleursSecteur[2].voitureTemps = copieMemoire[i].tempSecteur3;
			meilleursSecteur[2].voitureId = copieMemoire[i].id;
		}
		if (copieMemoire[i].meilleurTemps > 0 && copieMemoire[i].meilleurTemps < *meilleurIdTemps) {
			*meilleurId = copieMemoire[i].id;
			*meilleurIdTemps = copieMemoire[i].meilleurTemps;
		}
    if (saveStatus == TRUE && copieMemoire[i].ready != -1) {
      saveStatus = FALSE;
    }
  }

  if (sorting) {
		voiture *sort[nombreEnfants];
		memcpy(sort, classement, sizeof(voiture *) * nombreEnfants);
    qsort(sort, sizeof(sort)/sizeof(*sort), sizeof(*sort), mycoursecmp);
		memcpy(classement, sort, sizeof(voiture *) * nombreEnfants);
  }
  afficherTableauScoreCourse(classement, 7, meilleursSecteur, *meilleurId, *meilleurIdTemps, nombreEnfants);

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
*																	valeurs possibles : P1, P2, P3, Q, Course
*	@param char* tableauNoms[]			tableau contenant les noms des semaphores
* @param	int dataID							nombre permettant de retrouver l'emplacement en memoire
*																	partagee des donnees de configuration
* @param int *numeroDesVoitures		les ids des differentes voitures de la course
*
*/
void initFork(int nombreEnfants,char *semid, char *mode, char *tableauNoms[], int dataID, int *numeroDesVoitures){

  for (int i = 0; i < nombreEnfants; i++) {
    if (fork() == 0) {
      char nombreFiliale[3];
      char idData[10];
			char idVoiture[4];

      sprintf(nombreFiliale, "%d", i);
      sprintf(idData, "%d", dataID);
			sprintf(idVoiture, "%d", numeroDesVoitures[i]);

      char *voiture[]= {"./voiture",semid,nombreFiliale,mode,idData,tableauNoms[i],idVoiture, NULL};
      execvp(voiture[0], voiture);
    }
  }
}

/**utilise les methodes de saveLoad.c pour sauvegarder les resultats des courses
*	 determine le type de course dans laquelle on se trouve
*
*	@param int compteur 					le numero permettant de savoir le type de course que l'on a effectuee
* @param data *programmeData		pointeur vers l'emplacement mémoire des données de configuration
* @param voiture* copieMemoire	pointeur vers la copie en memoire locale de la memoire partagee
* @param voiture** classement		tableau de pointeurs contenant les copies des adresses memoires
*																de copieMemoire
*
*/
void save(int compteur, data *programmeData, voiture *copieMemoire, voiture **classement){
	if (totalCrashDetection(programmeData->nombreDeVoitures, copieMemoire)) {
		printf("%s\n", "toutes les voitures se sont crashée, la séance n'est donc pas valide !  aucunes données n'a été sauvegardée");
	}
	else{
		switch (compteur){
	    case 1:
	      saveEssai(compteur, programmeData->p1, classement, meilleursSecteur, programmeData->nombreDeVoitures);
	      break;
	    case 2:
	      saveEssai(compteur, programmeData->p2, classement, meilleursSecteur, programmeData->nombreDeVoitures);
	      break;
	    case 3:
	      saveEssai(compteur, programmeData->p3, classement, meilleursSecteur, programmeData->nombreDeVoitures);
	      break;
	    default :
	      printf("%s\n", "erreur dans la sauvegarde");
	      exit(EXIT_FAILURE);
	  }
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
  if (strcmp(argument_entrer, "p1") == 0){
    return 1;
  }
  if (strcmp(argument_entrer, "p2") == 0){
    return 2;
  }
  if (strcmp(argument_entrer, "p3") == 0){
    return 3;
  }
  if (strcmp(argument_entrer, "q") == 0){
    return 4;
  }
  if (strcmp(argument_entrer, "c") == 0){
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
      if (i>=nombreVoiture-offset) {
        classement[i]->memory->status = 0;
				classement[i]->memory->ready = TRUE;
      }
			else{
				classement[i]->memory->status = 2;
				classement[i]->memory->ready = TRUE;
			}
    }
  }
  else{
    for (int i = 0; i < nombreVoiture-offset; i++) {
      if (i>=nombreVoiture-2*offset) {
        classement[i]->memory->status = 0;
				classement[i]->memory->ready = TRUE;
      }
			else{
				classement[i]->memory->status = 2;
				classement[i]->memory->ready = TRUE;
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

		if (l->tours > r->tours) return -1;
		if (l->tours < r->tours) return 1;

		if (l->tempSecteur3 != 0 && r->tempSecteur3 == 0) return -1;
		if (l->tempSecteur3 == 0 && r->tempSecteur3 != 0) return 1;

		if (l->tempSecteur2 != 0 && r->tempSecteur2 == 0) return -1;
		if (l->tempSecteur2 == 0 && r->tempSecteur2 != 0) return 1;

		if (l->tempSecteur1 != 0 && r->tempSecteur1 == 0) return -1;
		if (l->tempSecteur1 == 0 && r->tempSecteur1 != 0) return 1;


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

/** permet d'initialiser un tableau de taille 3 contenant des structures
*		de donnee "gagnant"
*
*	@param gagnant* secteur	le tableau de structure "gagnant" devant etre initialise
*
*/
void initGagnant(gagnant *secteur) {
	for (int i = 0; i < 3; i++) {
		secteur[i].voitureId = -1;
		secteur[i].voitureTemps = INT_MAX;
	}
}

/** permet de savoir si toutes les voitures sont crashee ou non
*
*	@param int nombreEnfants			le nombre de voitures dans la course
* @param voiture* copieMemoire	pointeur vers la copie en memoire locale de la memoire partagee
*
*	@return int 									booleen donnant le resultat : FALSE si jamais au moins une
*																voiture n'est pas crashee, TRUE dans les autres cas
*
*/
int totalCrashDetection(int nombreEnfants, voiture *copieMemoire){

	for (int i = 0; i < nombreEnfants; i++) {
		if(!copieMemoire[i].crash){
      return FALSE;
    }
	}

	return TRUE;
}

/** permet de savoir si le nombre de voiture crashee sont superieur ou non
*		a un nombre defini de crash
*
*	@param int nombreEnfants			le nombre de voitures dans la course
*	@param int overCrash					le nombre de crashs maximum tolere
* @param voiture* copieMemoire	pointeur vers la copie en memoire locale de la memoire partagee
*
*	@return int 									booleen donnant le resultat : FALSE si jamais il y a eu
*																un nombre de crash egal ou inferieur a overCrash, TRUE
*																dans les autres cas
*
*/
int overCrashDetection(int nombreEnfants, int overCrash, voiture *copieMemoire){
	int crashNumber = 0;

	for (int i = 0; i < nombreEnfants; i++) {
		if(copieMemoire[i].crash){
      crashNumber ++;
    }
	}
	// test de crash
	if (crashNumber <= overCrash) {
		return FALSE;
	}
	return TRUE;
}
