#define _CRT_SECURE_NO_WARNINGS
#define WORD 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct folder {
    char name[WORD];
    struct folder* back;
    struct folder* next;
    struct folder* childFolder;
} folder;

/* Prototipovi */
folder* md(folder* current, char name[WORD]);
folder* cd(folder* current, char name[WORD]);
folder* cdBack(folder* current);
void dir(folder* current);
void freeTree(folder* root);

int main() {
    /* ROOT direktorij */
    folder* root = malloc(sizeof(folder));
    strcpy(root->name, "root");
    root->back = NULL;
    root->next = NULL;
    root->childFolder = NULL;

    folder* current = root;

    int choice;
    char name[WORD];

    while (1) {
        printf("\n----------------------------------\n");
        printf("Trenutni direktorij: %s\n", current->name);
        printf("1 - md\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - exit\n");
        printf("----------------------------------\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Ime direktorija: ");
            scanf("%s", name);
            md(current, name);
            break;

        case 2:
            printf("Ime direktorija: ");
            scanf("%s", name);
            current = cd(current, name);
            break;

        case 3:
            current = cdBack(current);
            break;

        case 4:
            dir(current);
            break;

        case 5:
            freeTree(root);
            return 0;

        default:
            printf("Nepoznata naredba!\n");
        }
    }
}


folder* md(folder* current, char name[WORD]) {
    folder* newNode = malloc(sizeof(folder));
    strcpy(newNode->name, name);

    newNode->childFolder = NULL;
    newNode->back = current;

    newNode->next = current->childFolder;
    current->childFolder = newNode;

    return current;
}

folder* cd(folder* current, char name[WORD]) {
    folder* temp = current->childFolder;

    while (temp) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }

    printf("Direktorij ne postoji!\n");
    return current;
}

folder* cdBack(folder* current) {
    if (current->back != NULL) {
        return current->back;
    }
    return current;
}

void dir(folder* current) {
    folder* temp = current->childFolder;

    if (!temp) {
        printf("(prazan direktorij)\n");
        return;
    }

    while (temp) {
        printf("%s\t", temp->name);
        temp = temp->next;
    }
    printf("\n");
}

void freeTree(folder* root) {
    if (!root) return;

    freeTree(root->childFolder);
    freeTree(root->next);
    free(root);
}
