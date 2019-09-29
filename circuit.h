#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED

#define true 1
#define false 0



#endif // CIRCUIT_H_INCLUDED
int tour(){


    int i=0;
    int total=0;
    int crash = false;
    while (i<3 || crash == true){
        if(secteur(300,500)==0){
            crash = true;
            printf("crash\n");
            break;
        }
        total += secteur(500,300);
        i++;
    }
    int min = total%3600/60;
    int sec = total%60;
    printf("temps total est de %dmin %ds\n",min, sec);
}
