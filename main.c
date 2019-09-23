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

 int randomF();

int numeroVoiture[20] = {7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};

int main()
{
    printf("Hello world!\n");
    randomF();

    return 0;
}

int randomF(){
    int i = 0;
    for(int j = 0; j<10;j++){

        i = rand()%5;
        printf("%d \n",i);

    }




}
