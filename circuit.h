#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED

#define true 1
#define false 0



#endif // CIRCUIT_H_INCLUDED
/**
*
*simule un tour dans un circuit
*
*
*/
int tour(){

    int total=0;//temps total
    int tour = 0;
    int crash = false;//boolean pour le crash de la voiture
    int s = 0;//temps pour un secteur
    int i = 1;

    while (i<=3 && crash != true){

        s = secteur(100,250);
        Sleep(s*10);// endormir le processus pendant s*10 milliseconde

        if(s == 0){//test si il y a un crash
            crash = true;
            //printf("crash sur le secteur %d\n",i);
            //TODO: recuperation des donnes de la voiture
            return 0;
        }
        if((i%2)==0){//si il passe dans le secteur 2
           //TODO: recuperation des donnes de la voiture
        }
        else if((i%3)==0){//si il passe dans le secteur 3
           // k.tempSecteur3=s;
           if(stand()){
                total += 15;
                //printf("passage au Stand: +15 seconde\n");
                Sleep(200);// endormir le processus pendant s*10 milliseconde
           }
           //TODO: recuperation des donnes de la voiture
        }
        else{//si il passe dans le secteur 1
           //TODO: recuperation des donnes de la voiture
        }

        total += s;//ajout au temps total de la voiture dans le circuit
        i++;
        //recuperer le temps total ici
    }

    return total;
}
void essaiLibreQuali(int chrono){
    int temps1 = 0;
    int temps2 = 0;
    int j = 1;
    do{
        //TODO: recuperation des donnes de la voiture
        temps1 = tour();
        temps2 += temps1;

        if(temps1==0){
            printf("retour au stand: crash\n");
            //TODO: recuperation des donnes de la voiture
        }
        else if(temps2>=chrono){
            //TODO: recuperation des donnes de la voiture
            j++;
        }
        else {

            affichage(j);
            j++;
        }
    }while(temps2<chrono && temps1 !=0);
}

int Course(int tours){

    int temps1 = 0;
    int temps2 = -1;
    int j = 1;
    int i = 0;

    do{
        temps1 = tour();
        temps2 += temps1;
        if(temps1==0){
            printf("retour au stand: crash\n");
            //TODO: recuperation des donnes de la voiture
        }
        else{
            //TODO: recuperation des donnes de la voiture
            j++;
        }
    }while(i<=tours && temps1 !=0);

}
