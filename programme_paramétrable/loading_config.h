#ifndef LOADING_CONFIG_H_INCLUDED
#define LOADING_CONFIG_H_INCLUDED

typedef struct data_load {
  int *numeroDesVoitures;
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

void configuration(data *programmeData, char *file);
int *readNumerosVoitures(char *buffer, int tailleListe);
void readOtherInformation(data *programmeData, char* ligne, int commande);
void defaultConfiguration(data *programmeData);

#endif // LOADING_CONFIG_H_INCLUDED
