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

        s = secteur(90,250);
        Sleep(s*10);// endormir le processus pendant s*10 milliseconde
        if(s == 0){//test si il y a un crash
            crash = true;
            printf("crash sur le secteur %d\n",i);
            return 0;
        }
        if((i%2)==0){//si il passe dans le secteur 2
           // k.tempSecteur2=s;
           printf("temps du secteur %d: %d seconde\n",i, s);
           //recuperer le temps ici
        }
        else if((i%3)==0){//si il passe dans le secteur 3
           // k.tempSecteur3=s;
           if(stand()){
                total += 15;
                printf("passage au Stand: +20 seconde\n");
           }
           printf("temps du secteur %d: %d seconde\n",i, s);
           //recuperer le temps ici
        }
        else{//si il passe dans le secteur 1
           // k.tempSecteur1=s;
           printf("temps du secteur %d: %d seconde\n",i, s);
           //recuperer le temps ici
        }


        total += s;//ajout au temps total de la voiture dans le circuit
        i++;
        //recuperer le temps total ici

    }
    return total;
}
void essaiLibre(int chrono){
    int temps1 = 0;
    int temps2 = -1;
    int j = 1;
    do{
        temps1 = tour();
        temps2 += temps1;
        if(temps1==0){
            printf("retour au stand: crash\n");
        }
        else{
            printf("temps du tour %d: %d seconde\n",j, temps2);
            j++;
        }
    }while(temps2<chrono && temps1 !=0);
}
int qualification(int chrono){
    int temps1 = 0;
    int temps2 = -1;
    int j = 1;
    do{
        temps1 = tour();
        temps2 += temps1;
        if(temps1==0){
            printf("retour au stand: crash\n");
        }
        else{
            printf("temps du tour %d: %d seconde\n",j, temps2);
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
        }
        else{
            printf("temps du tour %d: %d seconde\n",j, temps2);
            j++;
        }
    }while(i<tours && temps1 !=0);
}
