

main : main.c affichage.c saveLoad.c helper.c
			gcc -o main main.c affichage.c saveLoad.c helper.c -lm -lrt -lpthread

voiture : voiture.c secteurs.c circuit.c
			gcc -o voiture voiture.c circuit.c secteurs.c -lm -lrt -lpthread

clean:
				rm -rf main voiture *save.txt
