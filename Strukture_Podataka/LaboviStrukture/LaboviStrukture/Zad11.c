#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define HASH_SIZE 11

typedef struct _city {
    char name[MAX_NAME];
    int population;
    struct _city *left;
    struct _city *right;
} City;

typedef struct _country {
    char name[MAX_NAME];
    City* cityRoot;
    struct _country* next;
} Country;

typedef struct {
    Country* buckets[HASH_SIZE];
} HashTable;

City* createCity(char* name, int pop);
City* insertCity(City* root, char* name, int pop);
void printCities(City* root, int minPop);
void freeCities(City* root);

int calculateHash(char* name);
City* readCitiesFromFile(char* filename);
void insertCountrySorted(HashTable* ht, char* countryName, char* fileName);
void freeHashTable(HashTable* ht);

void search(HashTable* ht);

int main() {
    HashTable ht;
    int i = 0;
    FILE* fp = NULL;
    char countryName[MAX_NAME], fileName[MAX_NAME];

    // Svaki bucket na pocetku mora biti prazan (NULL), inace bi imali "smece" pokazivace
    for (i = 0; i < HASH_SIZE; i++) {
        ht.buckets[i] = NULL;
    }

    // Otvara datoteku koja sadrzi parove: ime_drzave ime_datoteke_s_gradovima
    fp = fopen("drzave.txt", "r");
    if (!fp) {
        // Ako se ne moze otvoriti, nema smisla nastaviti
        printf("Greska: Nije moguce otvoriti datoteku drzave.txt\n");
        return 1;
    }

    // Cita red po red (dvije rijeci): drzava i pripadna datoteka gradova
    // fscanf vraca 2 samo ako su obje vrijednosti uspjesno procitane
    while (fscanf(fp, "%s %s", countryName, fileName) == 2) {
        // Za svaku drzavu:
        // - izracuna bucket (hash)
        // - ucita gradove u BST
        // - ubaci drzavu u sortirani lanac unutar bucketa
        insertCountrySorted(&ht, countryName, fileName);
    }
    fclose(fp);

    // Prolazi kroz sve buckete i ispisuje sve drzave u njima
    printf("--- ISPIS SVIH DRZAVA I GRADOVA ---\n");
    for (i = 0; i < HASH_SIZE; i++) {
        Country* curr = ht.buckets[i];

        // U svakom bucketu moze biti vise drzava (kolizije) povezane lancem
        while (curr) {
            printf("\nDRZAVA: %s (Index: %d)", curr->name, i);

            // Ispis gradova iz BST-a; minPop=0 znaci ispis svih
            printCities(curr->cityRoot, 0);

            curr = curr->next;
        }
    }

    // Korisnicka pretraga: drzava + minimalna populacija
    search(&ht);

    // Oslobadanje svega alociranog (drzave + stabla gradova)
    freeHashTable(&ht);

    return 0;
}

City* createCity(char* name, int pop) {
    // Alocira memoriju za novi cvor u stablu gradova
    City* newCity = (City*)malloc(sizeof(City));
    if (!newCity)
        return NULL; // Ako malloc ne uspije, vraca NULL

    // Popunjava polja cvora
    strcpy(newCity->name, name);
    newCity->population = pop;

    // Novi cvor nema djecu dok se ne spoji u stablo
    newCity->left = NULL;
    newCity->right = NULL;

    return newCity;
}

City* insertCity(City* root, char* name, int pop) {
    // Ako smo dosli do praznog mjesta u stablu, tu se stvara novi cvor
    if (root == NULL) {
        return createCity(name, pop);
    }

    // Primarno se usporeduje populacija:
    // manja populacija ide lijevo, veca ide desno
    if (pop < root->population) {
        root->left = insertCity(root->left, name, pop);
    } else if (pop > root->population) {
        root->right = insertCity(root->right, name, pop);
    } else {
        // Ako je populacija jednaka, koristi se ime kao "tie-break"
        // abecedno manji ide lijevo, veci/isti ide desno
        if (strcmp(name, root->name) < 0) {
            root->left = insertCity(root->left, name, pop);
        } else {
            root->right = insertCity(root->right, name, pop);
        }
    }

    // Vraca se korijen (ne mijenja se pointer root-a, samo se grane dopunjuju)
    return root;
}

void printCities(City* root, int minPop) {
    // Inorder obilazak: lijevo -> cvor -> desno
    // Zbog nacina umetanja, to daje ispis po rastucoj populaciji (i po imenu kad je populacija ista)
    if (root == NULL)
        return;

    printCities(root->left, minPop);

    // Filter: ispisuje se samo ako je populacija >= minPop
    if (root->population >= minPop) {
        printf("\n  - %-15s | Populacija: %d", root->name, root->population);
    }

    printCities(root->right, minPop);
}

void freeCities(City* root) {
    // Postorder obilazak: prvo djeca, pa roditelj
    // Tako se sigurno oslobada cijelo stablo bez gubljenja pokazivaca
    if (root == NULL)
        return;

    freeCities(root->left);
    freeCities(root->right);
    free(root);
}

int calculateHash(char* name) {
    // Racuna indeks bucketa zbrajanjem ASCII vrijednosti prvih do 5 znakova
    // (kratka, jednostavna hash funkcija)
    int sum = 0;
    int i = 0;

    for (i = 0; i < 5 && name[i] != '\0'; i++) {
        sum += name[i];
    }

    // Modulo velicine tablice daje vrijednost 0..HASH_SIZE-1
    return sum % HASH_SIZE;
}

City* readCitiesFromFile(char* filename) {
    // Otvara datoteku s gradovima za jednu drzavu
    FILE* fp = fopen(filename, "r");
    City* root = NULL;
    char name[MAX_NAME];
    int pop = 0;

    if (!fp)
        return NULL; // Ako se ne moze otvoriti, vraca NULL (drzava ce imati prazno stablo)

    // Cita format "ImeGrada, broj"
    //  %[^,] -> ucita sve do zareza (ukljucuje razmake u imenu ako postoje prije zareza)
    //  , %d  -> nakon zareza i razmaka ucitaj broj
    while (fscanf(fp, " %[^,], %d", name, &pop) == 2) {
        // Svaki grad se ubaci u BST po pravilima iz insertCity
        root = insertCity(root, name, pop);
    }

    fclose(fp);
    return root;
}

void insertCountrySorted(HashTable* ht, char* countryName, char* fileName) {
    // Odredi bucket u koji ide drzava
    int index = calculateHash(countryName);

    // Alocira novi cvor drzave (element lanca u bucketu)
    Country* newCountry = (Country*)malloc(sizeof(Country));

    // Pointer na pointer: olaksava umetanje u sortiranu vezanu listu
    Country** current = &(ht->buckets[index]);

    if (!newCountry)
        return;

    // Popuni podatke drzave
    strcpy(newCountry->name, countryName);

    // Ucita gradove iz datoteke i dobije korijen BST-a gradova
    newCountry->cityRoot = readCitiesFromFile(fileName);

    // Novi cvor jos nije povezan
    newCountry->next = NULL;

    // "current" pokazuje na mjesto gdje ce se novi cvor zakaciti
    // Pomice se dok je trenutni element abecedno manji od novog
    // => lanac u bucketu ostaje sortiran po imenu drzave
    while (*current != NULL && strcmp((*current)->name, countryName) < 0) {
        current = &((*current)->next);
    }

    // Umetanje: newCountry ide prije *current
    newCountry->next = *current;
    *current = newCountry;
}

void search(HashTable* ht) {
    char target[MAX_NAME];
    int minPop = 0;
    int index = 0;
    Country* curr = NULL;

    // Unos trazene drzave i minimalne populacije
    printf("\n\n--- PRETRAGA ---");
    printf("\nUnesite ime drzave: ");
    scanf("%s", target);
    printf("Minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    // Izracun bucketa u kojem bi drzava trebala biti
    index = calculateHash(target);
    curr = ht->buckets[index];

    // Prolazi kroz lanac u tom bucketu i trazi tocno ime drzave
    while (curr != NULL) {
        if (strcmp(curr->name, target) == 0) {
            // Ako je pronadjena, ispisuje gradove s populacijom >= minPop
            printf("\nGradovi u %s s populacijom vecom od %d:", target, minPop);
            printCities(curr->cityRoot, minPop);
            return;
        }
        curr = curr->next;
    }

    // Ako se doslo do kraja lanca bez pogotka, drzava ne postoji
    printf("\nDrzava '%s' nije pronadjena.", target);
}

void freeHashTable(HashTable* ht) {
    int i = 0;

    // Prolazi kroz sve buckete
    for (i = 0; i < HASH_SIZE; i++) {
        Country* curr = ht->buckets[i];

        // Oslobada lanac drzava u bucketu
        while (curr != NULL) {
            Country* temp = curr;
            curr = curr->next;

            // Prvo oslobodi BST gradova te drzave
            freeCities(temp->cityRoot);

            // Zatim oslobodi samu drzavu
            free(temp);
        }

        // Nakon ciscenja, bucket vrati na NULL radi urednosti/sigurnosti
        ht->buckets[i] = NULL;
    }
}
