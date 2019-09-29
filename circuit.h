#ifndef CIRCUIT_H_INCLUDED
#define CIRCUIT_H_INCLUDED



#endif // CIRCUIT_H_INCLUDED
int tour(){

    srand(time(NULL));
    int i;
    int total=0;
    for(i=0; i<3;i++){
        total += secteur(500,300);
    }
    int min = total%3600/60;
    int sec = total%60;
    printf("temps total est de %dmin %ds\n",min, sec);
}
