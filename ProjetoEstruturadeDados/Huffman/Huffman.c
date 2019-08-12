#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>


typedef struct queue queue;
typedef struct list node;
typedef long int lint;
typedef unsigned char byte;

void set_frequence_to_zero(lint *frequence);
queue *create_queue();
void counting_frequence(FILE *input, lint *frequence);

void main()
{
	char archive[256];
	queue *new_queue = create_queue();
	lint frequence[256]; // Feito para receber a frequência dos caracteres do arquivo
	set_frequence_to_zero(frequence);

	printf("Insira o nome do arquivo a ser comprimido: \n");
	scanf("%[^\n]s", archive);
	FILE *input = fopen(archive, "rb"); //Lê o arquivo em binário, por isso o "rb"

	if (input == NULL)
	{
		puts("ERRO");
		exit(1);
	}

	////////////////////////////////////////////////////
	printf("Compactar ou Descompactar ? [c // d]\n");
	char choice[5];
	scanf("%[^\n]s", choice);
	////////////////////////////////////////////////////

	counting_frequence(input, frequence);

	int i;
	for (i = 0; i < 256; ++i)
	{
	    if(frequence[i] != 0)
            printf("%c %d\n", i, frequence[i]);
	}
}

struct queue
{
	node *head;
};

struct list
{
	byte c;
	lint freq;
	node *next;
	node *left;
	node *right;

};

void set_frequence_to_zero(lint *frequence)
{
	int i;
	for (i = 0; i < 256; ++i)
		frequence[i] = 0;
}

queue *create_queue()
{
	queue *temp = (queue*)malloc(sizeof(queue));
	temp->head = NULL;
	return temp;
}

void counting_frequence(FILE *input, lint *frequence)
{
	byte c;
	while(fscanf(input, "%c", &c) != EOF) //Essa função vai ler de caractere em caractere até o final
	{
		if (c != NULL){frequence[c] += 1;} //Se for diferente de NULL, ele vai adicionar a quantidade nesse array
		else{frequence[0] += 1;} //Se for NULL salva aqui
	}
	rewind(input); //Retorna o ponteiro para o começo do arquivo, pois o fscanf move o ponteiro até o final
}
