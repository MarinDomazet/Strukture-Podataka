#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* insert(Node* root, int value) {
    if (!root) {
        Node* n = malloc(sizeof(Node));
        n->value = value;
        n->left = NULL;
        n->right = NULL;
        return n;
    }
    if (value < root->value)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);
    return root;
}

int replace(Node* root) {
    if (!root)
        return 0;
    int leftSum = replace(root->left);
    int rightSum = replace(root->right);
    int oldValue = root->value;
    root->value = leftSum + rightSum;
    return root->value + oldValue;
}

void inorder(Node* root, FILE* f) {
    if (!root)
        return;
    inorder(root->left, f);
    fprintf(f, "%d ", root->value);
    inorder(root->right, f);
}

void freeTree(Node* root) {
    if (!root)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    srand((unsigned)time(NULL));
    Node* root = NULL;
    int n = 10;
    int values[10];

    for (int i = 0; i < n; i++) {
        values[i] = rand() % 81 + 10;
        root = insert(root, values[i]);
    }

    FILE* f = fopen("output.txt", "w");
    inorder(root, f);
    fprintf(f, "\n");

    replace(root);
    inorder(root, f);

    fclose(f);
    freeTree(root);
    return 0;
}