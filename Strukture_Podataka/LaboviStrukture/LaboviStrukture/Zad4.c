#define _CRT_SECURE_NO_WARNINGS
#define WORD 128
#define LINE 1024

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

typedef struct Node {
	int number;
	int exponent;
	struct Node* next;
}Node;

FILE* fp;

int openFile(char name[WORD]);
void closeFile();
Node* readFile();
Node* sumPoly(Node* poly1, Node* poly2);
Node* product(Node* poly1, Node* poly2);
void freeMemory(Node* head);

int main(){
	if (!openFile("polinomi.txt")) {
		printf("datoteka se nije uspjela otvoriti");
	}
	else {

		Node* poly1 = NULL, * poly2 = NULL, *sum=NULL, *productPoly=NULL;

		poly1 = readFile();
		poly2 = readFile();

		sum = sumPoly(poly1, poly2);

		while (sum) {
			printf("%d*x^%d\n", sum->number, sum->exponent);
			sum = sum->next;
		}

		productPoly = product(poly1, poly2);

		while (productPoly) {
			printf("%d*x^%d\n", productPoly->number, productPoly->exponent);
			productPoly = productPoly->next;
		}

		freeMemory(poly1);
		freeMemory(poly2);
		freeMemory(sum);
		freeMemory(product);

		closeFile();

		return 0;
	}
}
void freeMemory(Node* head) {
	while (head) {
		Node* prev = head;
		head = head->next;
		free(head);
	}
}
Node* product(Node* poly1, Node* poly2) {
	Node* product = NULL;

	while (poly1) {
		Node* temp = poly2;
		Node* tmp = NULL;
		
		while (temp) {
			Node* newNode = malloc(sizeof(Node));
			newNode->exponent = poly1->exponent+temp->exponent;
			newNode->number = poly1->number*temp->number;

			newNode->next = tmp;
			tmp = newNode;

			temp = temp->next;
		}

		product = sumPoly(tmp, product);

		poly1 = poly1->next;
	}

	return product;
}
Node* sumPoly(Node* poly1, Node* poly2) {
	if (!poly2) {
		return poly1;
	}
	if (!poly1) {
		return poly2;
	}
	if (!poly2 && !poly1) {
		printf("Obe liste su prazne");
		return NULL;
	}

	Node* sum = NULL;
	Node* temp = poly2;
	while (temp) {
		Node* newNode = malloc(sizeof(Node));
		newNode->number = temp->number;
		newNode->exponent = temp->exponent;

		newNode->next = sum;
		sum = newNode;

		temp = temp->next;
	}

	while (poly1) {
		int i = 0;
		temp = sum;
		while (temp) {
			if (poly1->exponent == temp->exponent) {
				temp->number += poly1->number;
				i = 1;
				break;
			}
			temp = temp->next;
		}

		if (!i) {
			Node* newNode = malloc(sizeof(Node));
			newNode->exponent = poly1->exponent;
			newNode->number = poly1->number;

			newNode->next = sum;
			sum = newNode;
		}
		poly1 = poly1->next;
	}

	return sum;
}
Node* readFile() {
	char line[LINE];
	int num, exp, n, i;
	Node* head = NULL;

	fgets(line, LINE, fp);
	if (line == NULL) {
		return NULL;
	}
	char* p = line;

	while (sscanf(p,"%d %d %n", &num, &exp, &n) == 2) {
		i = 0;
			if (head != NULL) {
				Node* tmp = head;
				while (tmp) {
					if (exp == tmp->exponent) {
						tmp->number += num;
						i = 1;
						break;
					}
					tmp = tmp->next;
				}
			}
			if (!i) {
				Node* newNode = malloc(sizeof(Node));
				if (!newNode) {
					perror("Neuspjesna alokacija");
					return head;
				}
				newNode->number = num;
				newNode->exponent = exp;

				newNode->next = head;
				head = newNode;
			}
			p += n;
	}

	return head;
}
int openFile(char name[WORD]) {
	fp = fopen(name, "r");
	if (fp) {
		return 1;
	}
	else {
		return -1;
	}
}
void closeFile() {
	fclose(fp);
}