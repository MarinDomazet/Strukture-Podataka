#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128

typedef struct City {
    char name[MAX];
    int population;
    struct City *next;
} City;

typedef struct CountryList {
    char name[MAX];
    City *cities;
    struct CountryList *next;
} CountryList;

typedef struct CountryTree {
    char name[MAX];
    City *cities;
    struct CountryTree *left;
    struct CountryTree *right;
} CountryTree;

City* insertCityList(City *head, char *name, int pop) {
    // Alocira novi cvor (novi grad)
    City *n = malloc(sizeof(City));

    // Kopira podatke u novi cvor
    strcpy(n->name, name);
    n->population = pop;

    // Inicijalno novi cvor ne pokazuje nikamo (nije u listi)
    n->next = NULL;

    // Ako je lista prazna -> novi postaje glava
    // Ako novi ima vecu populaciju od glave -> ide prije glave (sort silazno po populaciji)
    // Ako je populacija jednaka, ali je ime "manje" (abecedno prije) -> ide prije (tie-break po imenu)
    if (!head || pop > head->population ||
        (pop == head->population && strcmp(name, head->name) < 0)) {
        n->next = head;   // novi pokazuje na staru glavu
        return n;         // novi postaje glava
    }

    // Krece se od glave i trazi mjesto gdje treba umetnuti novi cvor
    City *cur = head;

    // Pomice se naprijed dok postoji sljedeci i dok sljedeci treba ostati ispred novog:
    // - sljedeci ima vecu populaciju (mora biti prije novog)
    // - ili ima istu populaciju i "dolazi" abecedno prije novog (zato je > 0 u strcmp u ovoj usporedbi)
    while (cur->next &&
          (cur->next->population > pop ||
          (cur->next->population == pop &&
           strcmp(name, cur->next->name) > 0)))
        cur = cur->next;

    // Umetanje: novi se "ugura" iza cur-a
    n->next = cur->next;  // novi pokazuje na element koji je do sada bio iza cur-a
    cur->next = n;        // cur sada pokazuje na novi
    return head;          // glava se ne mijenja
}

CountryList* insertCountryList(CountryList *head, char *name, City *cities) {
    // Alocira novi cvor za drzavu
    CountryList *n = malloc(sizeof(CountryList));

    // Spremi ime drzave i pokazivac na listu gradova
    strcpy(n->name, name);
    n->cities = cities;

    // Novi cvor jos nije povezan u listu
    n->next = NULL;

    // Ako je lista prazna ili nova drzava ide abecedno prije trenutne glave,
    // ubaci na pocetak
    if (!head || strcmp(name, head->name) < 0) {
        n->next = head;  // novi pokazuje na staru glavu
        return n;        // novi postaje glava
    }

    // Trazi poziciju za umetanje tako da lista ostane sortirana uzlazno po imenu
    CountryList *cur = head;

    // Ide naprijed dok postoji sljedeci i dok je sljedeci abecedno "manji" od novog,
    // tj. dok novi jos ne treba doci prije sljedeceg
    while (cur->next && strcmp(name, cur->next->name) > 0)
        cur = cur->next;

    // Umetanje novog iza cur-a
    n->next = cur->next;
    cur->next = n;

    return head;
}

CountryTree* insertCountryTree(CountryTree *root, char *name, City *cities) {
    // Ako je stablo prazno na ovoj poziciji, ovdje se stvara novi cvor
    if (!root) {
        CountryTree *n = malloc(sizeof(CountryTree));
        strcpy(n->name, name);
        n->cities = cities;

        // Novi cvor nema djecu
        n->left = n->right = NULL;
        return n;
    }

    // Usporedbom imena odlucuje se ide li u lijevo ili desno podstablo
    if (strcmp(name, root->name) < 0)
        root->left = insertCountryTree(root->left, name, cities);
    else
        root->right = insertCountryTree(root->right, name, cities);

    // Vraca se (nepromijenjeni) korijen trenutnog podstabla
    return root;
}

City* readCities(char *filename) {
    // Otvara datoteku s gradovima
    FILE *f = fopen(filename, "r");
    if (!f) return NULL; // Ako ne uspije otvoriti, vraca NULL

    City *head = NULL;  // Prazna lista gradova na pocetku
    char name[MAX];
    int pop;

    // Cita linije oblika: "ImeGrada,broj"
    //  %[^,] -> ucitaj sve znakove do zareza u name
    //  %d    -> ucitaj broj nakon zareza
    while (fscanf(f, " %[^,],%d", name, &pop) == 2)
        head = insertCityList(head, name, pop); // svaki procitani grad ubaci sortirano u listu

    fclose(f);
    return head; // vraca glavu liste gradova
}

void printCities(City *c) {
    // Prolazi kroz listu gradova i ispisuje svaki
    while (c) {
        printf("    %s %d\n", c->name, c->population);
        c = c->next;
    }
}

void printCountryList(CountryList *c) {
    // Prolazi kroz listu drzava
    while (c) {
        // Ispis imena drzave
        printf("%s\n", c->name);

        // Ispis svih gradova te drzave (lista je vec sortirana)
        printCities(c->cities);

        // Sljedeca drzava
        c = c->next;
    }
}

void printCountryTree(CountryTree *r) {
    // Inorder obilazak:
    // lijevo podstablo -> trenutni -> desno podstablo
    // To daje abecedni ispis drzava (ako je BST po imenu)
    if (!r) return;

    printCountryTree(r->left);

    printf("%s\n", r->name);
    printCities(r->cities);

    printCountryTree(r->right);
}

City* findCitiesList(CountryList *c, char *name) {
    // Linearna pretraga kroz listu drzava
    while (c) {
        // Ako je ime jednako trazenom, vrati listu gradova
        if (!strcmp(c->name, name))
            return c->cities;

        c = c->next;
    }
    // Nije pronadjeno
    return NULL;
}

City* findCitiesTree(CountryTree *r, char *name) {
    // Ako je doslo do praznog cvora -> nema te drzave
    if (!r) return NULL;

    // Usporedba odlucuje smjer pretrage
    int cmp = strcmp(name, r->name);

    // Ako je jednako -> pronadjeno, vrati gradove
    if (cmp == 0) return r->cities;

    // Ako je trazeno ime manje -> ide lijevo
    if (cmp < 0) return findCitiesTree(r->left, name);

    // Inace ide desno
    return findCitiesTree(r->right, name);
}

void searchCities(City *c, int minPop) {
    // Prolazi kroz listu gradova i ispisuje samo one koji prolaze filter
    while (c) {
        // Strogo vece od minPop (nije >=)
        if (c->population > minPop)
            printf("    %s %d\n", c->name, c->population);

        c = c->next;
    }
}

int main() {
    // Otvara glavnu datoteku u kojoj su parovi: drzava datoteka_gradova
    FILE *f = fopen("drzave.txt", "r");
    if (!f)
        return 1;

    CountryList *list = NULL; // glava liste drzava
    CountryTree *tree = NULL; // korijen BST-a drzava
    char country[MAX], file[MAX];

    // Cita svaki par (drzava, datoteka) iz drzave.txt
    while (fscanf(f, "%s %s", country, file) == 2) {
        // Ucitaj gradove iz pripadne datoteke u sortiranu listu
        City *cities = readCities(file);

        // Ubaci drzavu u vezanu listu (abecedno)
        list = insertCountryList(list, country, cities);

        // Ubaci drzavu u BST (po imenu)
        tree = insertCountryTree(tree, country, cities);
    }

    fclose(f);

    // Ispis struktura (lista i stablo) radi provjere
    printf("VEZANA LISTA:\n");
    printCountryList(list);

    printf("\nSTABLO:\n");
    printCountryTree(tree);

    // Korisnicki unos za pretragu
    char searchCountry[MAX];
    int minPop;

    printf("\nUnesi drzavu: ");
    scanf("%s", searchCountry);

    printf("Unesi minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    // Pretraga se radi preko BST-a (brze nego linearno kroz listu u prosjeku)
    City *c = findCitiesTree(tree, searchCountry);

    if (c)
        // Ispisi gradove te drzave koji imaju populaciju > minPop
        searchCities(c, minPop);
    else
        printf("Drzava ne postoji\n");

    return 0;
}
