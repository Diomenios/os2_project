

main : main.c affichage.c saveLoad.c
			gcc -o main main.c affichage.c saveLoad.c -lm -lrt -lpthread

voiture : voiture.c secteurs.c circuit.c
			gcc -o voiture voiture.c circuit.c secteurs.c -lrt -lpthread

clean:
				rm -rf main voiture *save.txt
