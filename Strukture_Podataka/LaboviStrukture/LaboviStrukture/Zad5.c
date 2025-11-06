#define _CRT_SECURE_NO_WARNINGS
#define LINE 1024
#define WORD 128

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct Node {
	double number;
	struct Node* next;
}Node;

void push(Node** head, double number);
double pop(Node** head);

int main() {
	Node* head = NULL;
	
	FILE* fp = fopen("postfix.txt", "r");
	if (fp == NULL) {
		printf("Nemoguce otvoriti datoteku");
		return -1;
	}
	else {
		char c;
		while ((c = fgetc(fp))!=EOF) {
			if (isspace(c)) {
				continue;
			}

			if (isdigit(c)) {
				double value = c - '0';
				push(&head, value);
			}
			else {
				
				double a=0, b=0,d=0;
				switch (c) {
					case '+':
						a = pop(&head);
						b = pop(&head);
						d = a + b;
						push(&head,d);
						break;
					case '-':
						a = pop(&head);
						b = pop(&head);
						d = b - a;
						push(&head, d);
						break;
					case '*':
						a = pop(&head);
						b = pop(&head);
						d = a * b;
						push(&head, d);
						break;
					case '/':
						a = pop(&head);
						b = pop(&head);
						d = b/a;
						push(&head, d);
						break;
				}
				push(&head, d);
			}
		}
	}

	if (head != NULL) {
		printf("Rezultat je: %.1f", head->number);
	}

	fclose(fp);

	return 0;

}
void push(Node** head, double number) {
	Node* newNode = malloc(sizeof(Node));
	newNode->next = *head;
	*head = newNode;
	newNode->number = number;
}
double pop(Node** head) {
	if ((*head) == NULL) {
		printf("Stog je prazan");
		return 0;
	}
	Node* tmp = *head;
	*head = (*head)->next;

	return tmp->number;
}