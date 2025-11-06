/*3. Prethodnom zadatku dodati funkcije:
A. dinami�ki dodaje novi element iza odre�enog elementa,
B. dinami�ki dodaje novi element ispred odre�enog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. �ita listu iz datoteke.
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
Node* Find_LastName(Node* head, char lastName[WORD]);
void Print_List(Node* head);
void Delete(Node** head, char lastName[WORD]);

void Add_Behind(Node* head, char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year);
void Add_Front(Node** head, char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year);
void Add_ToFile(Node* head);
Node* Read_FromFile(Node* head);
Node* Sort(Node* head);

int main() {

    Node* head = NULL;  // Inicijalno lista nema elemenata

    // Dodavanje prvih �lanova na po�etak liste
    head = Add_First(head, "Marin", "Domazet", 2005);
    head = Add_First(head, "Petar", "Kresimir", 1991);
    Add_Behind(head, "Kresimir", "Luka", "Baric", 2023);
    Add_Front(&head, "Kresimir", "Ivica", "Matun", 1962);

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

    Add_ToFile(head);

    head = Read_FromFile(head);
    Print_List(head);

    head = Sort(head);
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

    printf("\n\n\n");
}

// Tra�i osobu po prezimenu u listi
Node* Find_LastName(Node* head, char lastName[WORD]) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0) {
            printf("Element s prezimenom \"%s\" je prona�en.\n\n", lastName);
            return temp;
        }
        temp = temp->next;
    }
    printf("Element s prezimenom \"%s\" nije prona�en.\n\n", lastName);
    return NULL;
}

// Bri�e element iz liste po prezimenu
void Delete(Node** head, char lastName[WORD]) {
    Node* temp = *head;                               // Trenutni element
    Node* previous = NULL;                            // Pokaziva� na prethodni element

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

// Dodaje novi �vor iza odre�enog elementa u listi
void Add_Behind(Node* head, char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year) {

    Node* element = Find_LastName(head, lastNameSearch);  // Pronalazi element iza kojeg se dodaje novi

    Node* newNode = (Node*)malloc(sizeof(Node));         // Alokacija memorije za novi �vor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;

    if (element == NULL) {
        return printf("Prezime nije prona�eno, te nije uspjelo dodavanje clana");  // Ako nije prona�en, prekida se
    }

    newNode->next = element->next;   // Novi �vor pokazuje na sljede�i element iza prona�enog
    element->next = newNode;         // Prona�eni element pokazuje na novi �vor
}

// Dodaje novi �vor ispred odre�enog elementa u listi
void Add_Front(Node** head, char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year) {

    Node* temp = *head;                                 // Pokaziva� na po�etak liste
    Node* element = Find_LastName(temp, lastNameSearch); // Pronalazi element ispred kojeg se dodaje novi

    Node* newNode = (Node*)malloc(sizeof(Node));        // Alokacija memorije za novi �vor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;

    if (element == NULL) {
        return;                                         // Ako nije prona�en, prekida se
    }

    while (temp) {
        if (temp == element) {
            newNode->next = *head;                      // Novi �vor pokazuje na trenutni po�etak
            *head = newNode;                            // Novi �vor postaje po�etak liste
            return;
        }
        temp = temp->next;                              // Pomak na sljede�i element
    }
}

// Upisuje sve elemente liste u tekstualnu datoteku
void Add_ToFile(Node* head) {
    FILE* fp = fopen("Lista.txt", "w");                 // Otvara datoteku za pisanje

    while (head) {
        fprintf(fp, "%s %s %d\n", head->name, head->lastName, head->year);  // Upisuje podatke svakog �vora
        head = head->next;
    }

    fclose(fp);                                         // Zatvara datoteku
}

// �ita listu iz tekstualne datoteke i vra�a pokaziva� na po�etak nove liste
Node* Read_FromFile(Node* head) {
    FILE* fp = fopen("Lista.txt", "r");                 // Otvara datoteku za �itanje

    Node* temp = NULL;                                  // Pokaziva� na po�etak nove liste
    Node* previous = NULL;                              // Pokaziva� na prethodni �vor

    while (1) {
        Node* newNode = (Node*)malloc(sizeof(Node));    // Alokacija memorije za novi �vor
        if (fscanf(fp, "%s %s %d\n", newNode->name, newNode->lastName, &newNode->year) != 3) {
            free(newNode);                              // Ako nije uspje�no u�itano, prekida se
            break;
        }

        newNode->next = NULL;                           // Novi �vor inicijalno ne pokazuje ni na �to

        if (temp == NULL) {
            temp = newNode;                             // Prvi element postaje po�etak liste
        }
        else {
            previous->next = newNode;                   // Povezuje prethodni s novim
        }
        previous = newNode;                             // A�urira prethodni �vor
    }
    fclose(fp);                                         // Zatvara datoteku

    return temp;                                        // Vra�a po�etak nove liste
}

// Sortira listu po prezimenima osoba uzlazno
Node* Sort(Node* head) {
    Node* sorted = NULL;                                // Po�etak sortirane liste
    Node* temp = NULL;
    Node* previous = NULL;

    while (head) {
        temp = head;                                    // Uzimanje trenutnog �vora
        head = head->next;                              // Pomak na sljede�i
        temp->next = NULL;                              // Odvajanje �vora

        // Ako je lista prazna ili je prezime manje od prvog u sortiranoj listi
        if (sorted == NULL || strcmp(temp->lastName, sorted->lastName) < 0) {
            temp->next = sorted;                        // Novi �vor postaje po�etak
            sorted = temp;
        }
        else {
            Node* loop = sorted;                        // Petlja za pronalazak pozicije
            previous = loop;

            while (loop) {
                loop = loop->next;
                if (loop == NULL) {
                    previous->next = temp;              // Dodaje na kraj ako je najve�e prezime
                    temp->next = NULL;
                }
                if (strcmp(temp->lastName, loop->lastName) < 0) {
                    previous->next = temp;              // Ubacuje izme�u dva �vora
                    temp->next = loop;
                    break;
                }
                previous = loop;                        // Pomak na sljede�i
            }
        }
    }

    return sorted;                                      // Vra�a sortiranu listu
}