#include <stdio.h>
#include <stdlib.h>

//#include <time.h>
struct voiture {
int id;

double temp1Secteur1;
double temp1Secteur2;
double temp1Secteur3;

double temp1Total;

double temp2Secteur1;
double temp2Secteur2;
double temp2Secteur3;

double temp2Total;

double temp3Secteur1;
double temp3Secteur2;
double temp3Secteur3;

double temp3Total;

int classement;

int status;

};


int numeroVoiture[20] = {7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};

int main()
{
    printf("Hello world!\n");
    tour();
    return 0;
}


// generer un PRNG entre Min et Max
int my_rand(int min, int max){
  int c = rand()%(max-min+1)+min;//creation du nombre aleatoire
  return c;
}
//temps d'un secteur
int secteur(){
    int temp = 0;
    temp = my_rand(50, 600);//Generation du nombre aleatoire
    printf("tour en %ds\n",temp);
    return temp;
}
int tour(){

    srand(time(NULL));
    int i;
    int total=0;
    for(i=0; i<3;i++){
        total +=secteur();
    }
    int min = total%3600/60;
    int sec = total%60;
    printf("temps total est de %dmin %ds\n",min, sec);


}
