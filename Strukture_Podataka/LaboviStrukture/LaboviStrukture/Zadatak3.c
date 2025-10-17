/*3. Prethodnom zadatku dodati funkcije:
A. dinamièki dodaje novi element iza odreðenog elementa,
B. dinamièki dodaje novi element ispred odreðenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. èita listu iz datoteke.
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
Node* Find_LastName(Node* head, char lastName[WORD]);
void Print_List(Node* head);
void Delete(Node** head, char lastName[WORD]);

void Add_Behind(Node* head,char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year);
void Add_Front(Node** head, char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year);
void Add_ToFile(Node* head);
Node* Read_FromFile(Node* head);
Node* Sort(Node* head);

int main() {

    Node* head = NULL;  // Inicijalno lista nema elemenata

    // Dodavanje prvih èlanova na poèetak liste
    head = Add_First(head, "Marin", "Domazet", 2005);
    head = Add_First(head, "Petar", "Kresimir", 1991);
    Add_Behind(head, "Kresimir", "Luka", "Baric", 2023);
    Add_Front(&head, "Kresimir", "Ivica", "Matun", 1962);

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

    Add_ToFile(head);

    head = Read_FromFile(head);
    Print_List(head);

    head = Sort(head);
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

    printf("\n\n\n");
}

// Traži osobu po prezimenu u listi
Node* Find_LastName(Node* head, char lastName[WORD]) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0) {
            printf("Element s prezimenom \"%s\" je pronaðen.\n\n", lastName);
            return temp;
        }
        temp = temp->next;
    }
    printf("Element s prezimenom \"%s\" nije pronaðen.\n\n", lastName);
    return NULL;
}

// Briše element iz liste po prezimenu
void Delete(Node** head, char lastName[WORD]) {
    Node* temp = *head;                               // Trenutni element
    Node* previous = NULL;                            // Pokazivaè na prethodni element

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

// Dodaje novi èvor iza odreðenog elementa u listi
void Add_Behind(Node* head, char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year) {

    Node* element = Find_LastName(head, lastNameSearch);  // Pronalazi element iza kojeg se dodaje novi

    Node* newNode = (Node*)malloc(sizeof(Node));         // Alokacija memorije za novi èvor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;

    if (element == NULL) {
        return printf("Prezime nije pronaðeno, te nije uspjelo dodavanje clana");  // Ako nije pronaðen, prekida se
    }

    newNode->next = element->next;   // Novi èvor pokazuje na sljedeæi element iza pronaðenog
    element->next = newNode;         // Pronaðeni element pokazuje na novi èvor
}

// Dodaje novi èvor ispred odreðenog elementa u listi
void Add_Front(Node** head, char lastNameSearch[WORD], char name[WORD], char lastName[WORD], int year) {

    Node* temp = *head;                                 // Pokazivaè na poèetak liste
    Node* element = Find_LastName(temp, lastNameSearch); // Pronalazi element ispred kojeg se dodaje novi

    Node* newNode = (Node*)malloc(sizeof(Node));        // Alokacija memorije za novi èvor
    strcpy(newNode->name, name);
    strcpy(newNode->lastName, lastName);
    newNode->year = year;

    if (element == NULL) {
        return;                                         // Ako nije pronaðen, prekida se
    }

    while (temp) {
        if (temp == element) {
            newNode->next = *head;                      // Novi èvor pokazuje na trenutni poèetak
            *head = newNode;                            // Novi èvor postaje poèetak liste
            return;
        }
        temp = temp->next;                              // Pomak na sljedeæi element
    }
}

// Upisuje sve elemente liste u tekstualnu datoteku
void Add_ToFile(Node* head) {
    FILE* fp = fopen("Lista.txt", "w");                 // Otvara datoteku za pisanje

    while (head) {
        fprintf(fp, "%s %s %d\n", head->name, head->lastName, head->year);  // Upisuje podatke svakog èvora
        head = head->next;
    }

    fclose(fp);                                         // Zatvara datoteku
}

// Èita listu iz tekstualne datoteke i vraæa pokazivaè na poèetak nove liste
Node* Read_FromFile(Node* head) {
    FILE* fp = fopen("Lista.txt", "r");                 // Otvara datoteku za èitanje

    Node* temp = NULL;                                  // Pokazivaè na poèetak nove liste
    Node* previous = NULL;                              // Pokazivaè na prethodni èvor

    while (1) {
        Node* newNode = (Node*)malloc(sizeof(Node));    // Alokacija memorije za novi èvor
        if (fscanf(fp, "%s %s %d\n", newNode->name, newNode->lastName, &newNode->year) != 3) {
            free(newNode);                              // Ako nije uspješno uèitano, prekida se
            break;
        }

        newNode->next = NULL;                           // Novi èvor inicijalno ne pokazuje ni na što

        if (temp == NULL) {
            temp = newNode;                             // Prvi element postaje poèetak liste
        }
        else {
            previous->next = newNode;                   // Povezuje prethodni s novim
        }
        previous = newNode;                             // Ažurira prethodni èvor
    }
    fclose(fp);                                         // Zatvara datoteku

    return temp;                                        // Vraæa poèetak nove liste
}

// Sortira listu po prezimenima osoba uzlazno
Node* Sort(Node* head) {
    Node* sorted = NULL;                                // Poèetak sortirane liste
    Node* temp = NULL;
    Node* previous = NULL;

    while (head) {
        temp = head;                                    // Uzimanje trenutnog èvora
        head = head->next;                              // Pomak na sljedeæi
        temp->next = NULL;                              // Odvajanje èvora

        // Ako je lista prazna ili je prezime manje od prvog u sortiranoj listi
        if (sorted == NULL || strcmp(temp->lastName, sorted->lastName) < 0) {
            temp->next = sorted;                        // Novi èvor postaje poèetak
            sorted = temp;
        }
        else {
            Node* loop = sorted;                        // Petlja za pronalazak pozicije
            previous = loop;

            while (loop) {
                loop = loop->next;
                if (loop == NULL) {
                    previous->next = temp;              // Dodaje na kraj ako je najveæe prezime
                    temp->next = NULL;
                }
                if (strcmp(temp->lastName, loop->lastName) < 0) {
                    previous->next = temp;              // Ubacuje izmeðu dva èvora
                    temp->next = loop;
                    break;
                }
                previous = loop;                        // Pomak na sljedeæi
            }
        }
    }

    return sorted;                                      // Vraæa sortiranu listu
}
