
all : main voiture

main : main_param.c affichage.c saveLoad.c helper.c loading_config.c
			gcc -o main_param main_param.c affichage.c saveLoad.c helper.c loading_config.c -lm -lrt -lpthread

voiture : voiture.c secteurs.c circuit.c
			gcc -o voiture voiture.c circuit.c secteurs.c -lm -lrt -lpthread

clean:
				rm -rf main_param voiture *save.txt
