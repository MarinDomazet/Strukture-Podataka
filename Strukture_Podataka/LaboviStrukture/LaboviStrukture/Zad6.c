/*Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji

predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je

raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu

naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor

vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je

omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni

artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen. */

#define _CRT_SECURE_NO_WARNINGS
#define WORD 128
#define LINE 1024

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

typedef struct ArticleNode {
	char name[WORD];
	int quantity;
	double price;
	struct ArticleNode* next;
}ArticleNode;

typedef struct ReceiptNode {
	char date[11];
	struct ReceiptNode* next;
	struct ArticleNode* articleHead;
}ReceiptNode;

FILE* openFile(char name[WORD]);
void closeFile(FILE* fp);
ReceiptNode* readListFile(ReceiptNode* head, FILE *fpLR);
ReceiptNode* sortDate(ReceiptNode* head, ReceiptNode* newNode);
void freeMemReceipt(ReceiptNode* head);
void freeMemArticle(ArticleNode* head);
ArticleNode* sortName(ArticleNode* head, ArticleNode* newNode);
int search(ReceiptNode* head, char date1[WORD], char date2[WORD], char name[WORD], double article[2]);

int main() {

	ReceiptNode* head = NULL;

	FILE* fpLR = openFile("ListaRacuna.txt");

	head = readListFile(head, fpLR);
	
	double article[2] = { 0 };
	char name[WORD], date1[WORD], date2[WORD];

	printf("Unesite artikl koji želite tražiti: ");
	scanf("%s", name);
	printf("\n--NAPOMENA: obavezno unosite datume u formatu YYYY-MM-DD--\n");
	printf("Unesite datum OD kojeg želite pretraživati: ");
	scanf("%s", date1);
	printf("Unesite datum DO kojeg želite pretraživati: ");
	scanf("%s", date2);

	if (!search(head, date1, date2, name, article)) {
		printf("Nije pronaðen artikl koji zadovoljava uvjete");
	}

	closeFile(fpLR);
	freeMemReceipt(head);

	return 0;
}

int search(ReceiptNode* head, char date1[WORD], char date2[WORD], char name[WORD], double article[2]) {
	article[0] = 0;
	article[1] = 0;
	ReceiptNode* tmp = head;

	while (tmp) {
		if ((strcmp(tmp->date, date1) >= 0 ) && (strcmp(tmp->date, date2) <= 0)) {
			ArticleNode* temp = tmp->articleHead;

			while (temp) {
				if (!strcmp(name, temp->name)) {
					article[0] += temp->price * temp->quantity;
					article[1] += temp->quantity;
				}
				temp = temp->next;
			}
		}
		tmp = tmp->next;
	}

	if (article[0] != 0 && article[1] != 0) {
		printf("cijena: %.2lf\n", article[0]);
		printf("kolicina: %.lf\n", article[1]);
		return 1;
	}
	else {
		return 0;
	}
}

void freeMemReceipt(ReceiptNode* head) {
	while (head) {
		ReceiptNode* tmp = head;
		freeMemArticle(head->articleHead);
		head = head->next;
		free(tmp);
	}
}

void freeMemArticle(ArticleNode* head) {
	while (head) {
		ArticleNode* tmp = head;
		head = head->next;
		free(tmp);
	}
}

ArticleNode* sortName(ArticleNode* head, ArticleNode* newNode) {
	if (head == NULL) {
		newNode->next = head;
		head = newNode;
		return head;
	}
	else {
		ArticleNode* tmp = head;
		ArticleNode* prev = NULL;

		while (tmp) {
			if (strcmp(newNode->name, tmp->name) < 0) {
				if (prev == NULL) {
					newNode->next = head;
					head = newNode;
					return head;
				}
				else {
					newNode->next = prev->next;
					prev->next = newNode;
					return head;
				}
			}
			if (strcmp(newNode->name, tmp->name) > 0) {
				prev = tmp;
				tmp = tmp->next;
				if (tmp == NULL) {
					newNode->next = tmp;
					prev->next = newNode;
					return head;
				}
			}
		}
	}
}

ReceiptNode* sortDate(ReceiptNode* head, ReceiptNode* newNode) {
	if (head==NULL) {
		newNode->next = head;
		head = newNode;
		return head;
	}
	else {
		ReceiptNode* tmp = head;
		ReceiptNode* prev = NULL;
		
		while (tmp) {
			if (strcmp(newNode->date, tmp->date) <= 0) {
				if (prev == NULL) {
					newNode->next = head;
					head = newNode;
					return head;
				}
				else {
					newNode->next = prev->next;
					prev->next = newNode;
					return head;
				}
			}
			if (strcmp(newNode->date, tmp->date) > 0) {
				prev = tmp;
				tmp = tmp->next;
				if (tmp==NULL) {
					newNode->next = tmp;
					prev->next = newNode;
					return head;
				}
			}
		}
	}
}

ReceiptNode* readListFile(ReceiptNode* head, FILE* fpLR) {
	char line[LINE];

	while (fgets(line, LINE, fpLR)!=NULL) {
		ReceiptNode* newNode = malloc(sizeof(ReceiptNode));
		if (newNode == NULL) {
			return NULL;
		}

		int len = strlen(line);
		if (len > 0 && line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}
		FILE* fp = openFile(line);
		if (!fp) continue;
		
		char date[WORD];
		fgets(date, WORD, fp);
		len = strlen(date);
		if (len > 0 && date[len - 1] == '\n') {
			date[len - 1] = '\0';
		}

		strcpy(newNode->date, date);
		newNode->articleHead = NULL;

		head = sortDate(head, newNode);

		char article[WORD], buffer[LINE];
		int quantity;
		double price;

		while (fgets(buffer, LINE, fp) != NULL) {
			ArticleNode* newArticle = malloc(sizeof(ArticleNode));
			if (newArticle == NULL) {
				return NULL;
			}

			len = strlen(buffer);
			if (len > 0 && buffer[len - 1] == '\n') {
				buffer[len - 1] = '\0';
			}

			sscanf(buffer, "%[^,],%d,%lf", article, &quantity, &price);
			strcpy(newArticle->name, article); 
			newArticle->quantity = quantity;
			newArticle->price = price;

			newNode->articleHead = sortName(newNode->articleHead, newArticle);
		}

		closeFile(fp);
	}

	return head;
}

FILE* openFile(char name[WORD]) {
	FILE* fp = fopen(name, "r");
	if (!fp) {
		printf("Nije uspjelo otvaranje datoteke");
		return NULL;
	}
	else {
		return fp;
	}
}

void closeFile(FILE* fp) {
	fclose(fp);
}