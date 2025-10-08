/*1. Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/

#define _CRT_SECURE_NO_WARNINGS
/*#define FILE_NOT_OPENED(-1)
#define MAX_LINE(1024)*/

#include<stdio.h>
#include<stdlib.h>

typedef struct{
	char ime[20];
	char prezime[20];
	int bodovi; 
	double relB;

}Student;

int main() {

	FILE* file;

	file = fopen("Studenti.txt", "r");

	char c[1024];
	int br = 0;

	while(fgets(c, sizeof(c), file)) {
		br++;
	}

	rewind(file);

	Student* Studenti = (Student*)malloc(br * sizeof(Student));


	for (int i = 0; i < br; i++) {
		fscanf(file,"%s %s %d", Studenti[i].ime, Studenti[i].prezime, &Studenti[i].bodovi);
		Studenti[i].relB = ((double)Studenti[i].bodovi / 1000) * 100;
	}

	for (int i = 0; i < br; i++) {
		printf("%d %s %s %d %f\n", i + 1, Studenti[i].ime, Studenti[i].prezime, Studenti[i].bodovi, Studenti[i].relB);
	}

	fclose(file);

	printf("Brpj redaka: %d", br);

	return 0;
}