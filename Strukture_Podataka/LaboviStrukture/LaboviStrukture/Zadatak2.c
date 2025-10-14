/*2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node {
	char ime[20], prezime[20];
	int God_Rod;
	struct Node* next;
}Node;

Node* Dodaj_PrviClan(Node* head, char ime[20], char prezime[20], int God_Rod);
void Dodaj_ZadnjiClan(Node* head, char ime[20], char prezime[20], int God_Rod);
void Trazi_Prezime(Node* head, char prezime[20]);
void Ispis(Node* head);
void Brisanje(Node** Head, char prezime[20]);

int main() {

	Node* head = NULL;

	head = Dodaj_PrviClan(head, "Marin","Domazet",2005);
	head = Dodaj_PrviClan(head, "Petar", "Kresimir", 1991);
	Ispis(head);
	Dodaj_ZadnjiClan(head, "Marko", "Ivandic", 2017);
	Trazi_Prezime(head, "Kresimir");
	Brisanje(&head, "Kresimir");
	Ispis(head);

	return 0;
}

Node* Dodaj_PrviClan(Node* head, char ime[20], char prezime[20], int God_Rod) {
	Node* FirstElement;
	FirstElement = malloc(sizeof(Node));
	strcpy(FirstElement->ime, ime);
	strcpy(FirstElement->prezime, prezime);
	FirstElement->God_Rod = God_Rod;

	FirstElement->next = head;
	head = FirstElement;

	return FirstElement;
}

void Dodaj_ZadnjiClan(Node* head, char ime[20], char prezime[20], int God_Rod) {
	Node* temp = head;

	Node* LastElement;
	LastElement = (Node*)malloc(sizeof(Node));
	strcpy(LastElement->ime, ime);
	strcpy(LastElement->prezime, prezime);
	LastElement->God_Rod = God_Rod;
	LastElement->next = NULL;

	while (temp) {
		if (temp->next == NULL) {
			temp->next = LastElement;
			return;
		}
		temp = temp->next;
	}
}

void Ispis(Node* head) {
	Node* temp1 = head;
	while(temp1){
		if (temp1 == NULL) {
			printf("Nema clanova niza");
			return;
		}
		printf("%s\n%s\n%d\n\n", temp1->ime, temp1->prezime, temp1->God_Rod);
		temp1 = temp1->next;
	}
}

void Trazi_Prezime(Node* head, char prezime[20]) {
	Node* temp2 = head;
	while (temp2) {
		if (strcmp(temp2->prezime, prezime) == 0) {
			printf("Element s trazenim prezimenom \"%s\" je pronaden.\n\n",prezime);
			return;
		}
		temp2 = temp2->next;
	}
}

void Brisanje(Node** head, char prezime[20]) {
	Node* temp3 = *head;
	Node* prethodni = NULL;
	while (temp3) {
		if (!strcmp(temp3->prezime, prezime)) {
			if (prethodni == NULL) {
				*head = temp3->next;
			}
			else {
				prethodni->next = temp3->next;
			}
			free(temp3);
			break;
		}
		prethodni = temp3;
		temp3 = temp3->next;
	}
}