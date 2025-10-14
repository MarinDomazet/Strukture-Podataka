/*2. Definirati strukturu osoba (ime, prezime, godina ro�enja) i napisati program koji:
A. dinami�ki dodaje novi element na po�etak liste,
B. ispisuje listu,
C. dinami�ki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. bri�e odre�eni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/
#define _CRT_SECURE_NO_WARNINGS
#define FILE_NOT_OPENED (-1)
#define MAX_LINE (1024)
#define WORD 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicija �vora jednostruko povezane liste.
// Svaki �vor sadr�i ime, prezime, godinu ro�enja i pokaziva� na sljede�i element.
typedef struct Node {
    char name[WORD];                // Polje za ime osobe
    char lastName[WORD];            // Polje za prezime osobe
    int year;                       // Godina ro�enja
    struct Node* next;              // Pokaziva� na sljede�i element u listi
} Node;

Node* Add_First(Node* head, char name[WORD], char lastName[WORD], int year);
void Add_Last(Node* head, char name[WORD], char lastName[WORD], int year);
void Find_LastName(Node* head, char lastName[WORD]);
void Print_List(Node* head);
void Delete(Node** head, char lastName[WORD]);

int main() {

    Node* head = NULL;  // Inicijalno lista nema elemenata

    // Dodavanje prvih �lanova na po�etak liste
    head = Add_First(head, "Marin", "Domazet", 2005);
    head = Add_First(head, "Petar", "Kresimir", 1991);

    // Ispis trenutnog stanja liste
    Print_List(head);

    // Dodavanje novog �lana na kraj liste
    Add_Last(head, "Marko", "Ivandic", 2017);

    // Tra�enje �lana po prezimenu
    Find_LastName(head, "Kresimir");

    // Brisanje odre�enog �lana iz liste
    Delete(&head, "Kresimir");

    // Ispis liste nakon brisanja
    Print_List(head);

    return 0;
}


// Dodaje novi �vor na po�etak liste
Node* Add_First(Node* head, char name[WORD], char lastName[WORD], int year) {
    Node* newNode = malloc(sizeof(Node));     // Alokacija memorije za novi �vor

    // Kopiranje vrijednosti u novi �vor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;

    // Novi �vor pokazuje na dosada�nji prvi element
    newNode->next = head;

    // Novi �vor postaje novi po�etak liste
    return newNode;
}

// Dodaje novi �vor na kraj liste
void Add_Last(Node* head, char name[WORD], char lastName[WORD], int year) {
    Node* temp = head;                        // Pomo�ni pokaziva� za prolazak kroz listu

    Node* newNode = malloc(sizeof(Node));     // Alokacija memorije za novi �vor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;
    newNode->next = NULL;                     // Zadnji element uvijek pokazuje na NULL

    // Ako lista postoji, prolazi do kraja
    while (temp) {
        if (temp->next == NULL) {             // Kad na�e zadnji element
            temp->next = newNode;             // Povezuje ga s novim
            return;
        }
        temp = temp->next;                    // Ina�e ide na sljede�i element
    }
}

// Ispisuje sve �lanove liste
void Print_List(Node* head) {
    Node* temp = head;
    if (temp == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    while (temp) {
        printf("Ime: %s\nPrezime: %s\nGodina ro�enja: %d\n\n",
            temp->name, temp->lastName, temp->year);
        temp = temp->next;
    }
}

// Tra�i osobu po prezimenu u listi
void Find_LastName(Node* head, char lastName[WORD]) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0) {
            printf("Element s prezimenom \"%s\" je prona�en.\n\n", lastName);
            return;
        }
        temp = temp->next;
    }
    printf("Element s prezimenom \"%s\" nije prona�en.\n\n", lastName);
}

// Bri�e element iz liste po prezimenu
void Delete(Node** head, char lastName[WORD]) {
    Node* temp = *head;        // Trenutni element
    Node* previous = NULL;        // Pokaziva� na prethodni element

    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0) {  // Ako se prezime podudara
            if (previous == NULL) {
                *head = temp->next;                   // Ako se bri�e prvi element
            }
            else {
                previous->next = temp->next;          // Prespaja se preko elementa koji se bri�e
            }
            free(temp);                               // Osloba�a memoriju
            printf("Element s prezimenom \"%s\" je obrisan.\n\n", lastName);
            return;
        }
        previous = temp;
        temp = temp->next;
    }
    printf("Element s prezimenom \"%s\" nije prona�en.\n\n", lastName);
}