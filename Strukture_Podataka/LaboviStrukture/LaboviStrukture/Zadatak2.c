/*2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/
#define _CRT_SECURE_NO_WARNINGS
#define FILE_NOT_OPENED (-1)
#define MAX_LINE (1024)
#define WORD 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicija èvora jednostruko povezane liste.
// Svaki èvor sadrži ime, prezime, godinu roðenja i pokazivaè na sljedeæi element.
typedef struct Node {
    char name[WORD];                // Polje za ime osobe
    char lastName[WORD];            // Polje za prezime osobe
    int year;                       // Godina roðenja
    struct Node* next;              // Pokazivaè na sljedeæi element u listi
} Node;

Node* Add_First(Node* head, char name[WORD], char lastName[WORD], int year);
void Add_Last(Node* head, char name[WORD], char lastName[WORD], int year);
void Find_LastName(Node* head, char lastName[WORD]);
void Print_List(Node* head);
void Delete(Node** head, char lastName[WORD]);

int main() {

    Node* head = NULL;  // Inicijalno lista nema elemenata

    // Dodavanje prvih èlanova na poèetak liste
    head = Add_First(head, "Marin", "Domazet", 2005);
    head = Add_First(head, "Petar", "Kresimir", 1991);

    // Ispis trenutnog stanja liste
    Print_List(head);

    // Dodavanje novog èlana na kraj liste
    Add_Last(head, "Marko", "Ivandic", 2017);

    // Traženje èlana po prezimenu
    Find_LastName(head, "Kresimir");

    // Brisanje odreðenog èlana iz liste
    Delete(&head, "Kresimir");

    // Ispis liste nakon brisanja
    Print_List(head);

    return 0;
}


// Dodaje novi èvor na poèetak liste
Node* Add_First(Node* head, char name[WORD], char lastName[WORD], int year) {
    Node* newNode = malloc(sizeof(Node));     // Alokacija memorije za novi èvor

    // Kopiranje vrijednosti u novi èvor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;

    // Novi èvor pokazuje na dosadašnji prvi element
    newNode->next = head;

    // Novi èvor postaje novi poèetak liste
    return newNode;
}

// Dodaje novi èvor na kraj liste
void Add_Last(Node* head, char name[WORD], char lastName[WORD], int year) {
    Node* temp = head;                        // Pomoæni pokazivaè za prolazak kroz listu

    Node* newNode = malloc(sizeof(Node));     // Alokacija memorije za novi èvor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;
    newNode->next = NULL;                     // Zadnji element uvijek pokazuje na NULL

    // Ako lista postoji, prolazi do kraja
    while (temp) {
        if (temp->next == NULL) {             // Kad naðe zadnji element
            temp->next = newNode;             // Povezuje ga s novim
            return;
        }
        temp = temp->next;                    // Inaèe ide na sljedeæi element
    }
}

// Ispisuje sve èlanove liste
void Print_List(Node* head) {
    Node* temp = head;
    if (temp == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    while (temp) {
        printf("Ime: %s\nPrezime: %s\nGodina roðenja: %d\n\n",
            temp->name, temp->lastName, temp->year);
        temp = temp->next;
    }
}

// Traži osobu po prezimenu u listi
void Find_LastName(Node* head, char lastName[WORD]) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0) {
            printf("Element s prezimenom \"%s\" je pronaðen.\n\n", lastName);
            return;
        }
        temp = temp->next;
    }
    printf("Element s prezimenom \"%s\" nije pronaðen.\n\n", lastName);
}

// Briše element iz liste po prezimenu
void Delete(Node** head, char lastName[WORD]) {
    Node* temp = *head;        // Trenutni element
    Node* previous = NULL;        // Pokazivaè na prethodni element

    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0) {  // Ako se prezime podudara
            if (previous == NULL) {
                *head = temp->next;                   // Ako se briše prvi element
            }
            else {
                previous->next = temp->next;          // Prespaja se preko elementa koji se briše
            }
            free(temp);                               // Oslobaða memoriju
            printf("Element s prezimenom \"%s\" je obrisan.\n\n", lastName);
            return;
        }
        previous = temp;
        temp = temp->next;
    }
    printf("Element s prezimenom \"%s\" nije pronaðen.\n\n", lastName);
}