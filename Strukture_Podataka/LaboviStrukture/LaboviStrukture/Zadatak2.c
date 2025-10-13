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

int main() {

	Node* head = NULL;
	
	Node* FirstElement;
	FirstElement = malloc(sizeof(Node));
	strcpy(FirstElement->ime, "Marin");
	strcpy(FirstElement->prezime, "Domazet");
	FirstElement->God_Rod = 2005;
	FirstElement->next = head;
	head = FirstElement;
	
	Node* temp = head;

	Node* LastElement;
	LastElement = (Node*)malloc(sizeof(Node));
	strcpy(LastElement->ime, "Ivan");
	strcpy(LastElement->prezime, "Bonacic");
	LastElement->God_Rod = 2000;
	LastElement->next = NULL;
	while (temp) {
		if (temp->next == NULL) {
			temp->next = LastElement;
			break;
		}
		temp = temp->next;
	}
	
	Node *temp1 = head;
	do {
		if (temp1 == NULL) {
			printf("Nema clanova niza");
			break;
		}
		printf("%s\n%s\n%d\n\n", temp1->ime, temp1->prezime, temp1->God_Rod);
		temp1 = temp1->next;
	} while (temp1);

	Node *temp2 = head;
	while (temp2) {
		if (strcmp(temp2->prezime,"Bonacic") == 0) {
			printf("Element s trazenim prezimenom \"Bonacic\" je pronaden.\n");
			break;
		}
		temp2 = temp2->next;
	}

	Node* temp3 = head;
	Node* prethodni = NULL;
	while (temp3) {
		if (!strcmp(temp3->prezime, "Bonacic")) {
			if (prethodni == NULL) {
				head = temp3->next;
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

	temp1 = head;
	do {
		if (temp1 == NULL) {
			printf("Nema clanova niza");
			break;
		}
		printf("%s\n%s\n%d\n\n", temp1->ime, temp1->prezime, temp1->God_Rod);
		temp1 = temp1->next;
	} while (temp1);

	return 0;
}