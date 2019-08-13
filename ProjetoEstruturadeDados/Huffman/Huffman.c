#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


typedef struct queue queue;
typedef struct list tnode;
typedef long int lint;
typedef unsigned char byte;

void set_frequence_to_zero(lint *frequence);
queue *create_queue();
void counting_frequence(FILE *input, lint *frequence);
void make_list(queue *tree_queue, lint *frequence);
void enqueue(queue *tree_queue, byte c, lint freq, tnode *left, tnode *right);
void make_huff_tree(queue *tree_queue);
void merge(queue *tree_queue, tnode *a, tnode *b);
tnode *dequeue(queue *tree_queue);
void print(tnode *p_queue);
void print_pre_order(tnode *huff_tree);

void main()
{
	char archive[256];
	queue *new_queue = create_queue();
	lint frequence[256]; // Feito para receber a frequência dos caracteres do arquivo
	set_frequence_to_zero(frequence);

	printf("Insira o nome do arquivo: \n");
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
	make_list(new_queue, frequence);
}

struct queue
{
	tnode *head;
};

struct list
{
	byte c;
	lint freq;
	tnode *next;
	tnode *left;
	tnode *right;

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


void make_list(queue *tree_queue, lint *frequence)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		if (frequence[i] != 0)
		{
			enqueue(tree_queue, i, frequence[i], NULL, NULL); // Tecnicamente,transforma um array numa lista com prioridade/fila
		}
	}
	make_huff_tree(tree_queue);
}

void enqueue(queue *tree_queue, byte c, lint f, tnode *left, tnode *right)
{
	tnode *tree_node = (tnode*)malloc(sizeof(tnode));
	tree_node->c = c;
	tree_node->freq = f;
	tree_node->left = left;
	tree_node->right = right; // aqui só seta os valores


	if (tree_queue->head == NULL || f < tree_queue->head->freq) //se for o primeiro, cria do menor para o maior, colocando a head no menor
	{
		tree_node->next = tree_queue->head;
		tree_queue->head = tree_node;
	}
	else
	{
		tnode *tree_node2 = tree_queue->head;
		while(tree_node2->next != NULL && tree_node2->next->freq < f)
		{
			tree_node2 = tree_node2->next;
		}
		tree_node->next = tree_node2->next;
		tree_node2->next = tree_node;
	}
}

void make_huff_tree(queue *tree_queue)
{
	tnode *a = tree_queue->head;
	tnode *b = a->next;
	while(b != NULL)
	{
		enqueue(tree_queue, '*', a->freq + b->freq, a, b); // propriedade comum do huffman para montar sua árvore, seleciona os dois menores e soma criando
        dequeue(tree_queue);								// um novo nó com a soma e um * que aponta para os dois que foram usados para criar esse nó
        dequeue(tree_queue);
		a = tree_queue->head;
		b = a->next;
	}

	tnode *hff = tree_queue->head;
	print(hff);

	tnode *huff_tree = tree_queue->head;
	printf("Tree -> ");
	print_pre_order(huff_tree);
	printf("\n");

}

tnode *dequeue(queue *tree_queue)
{
	if (tree_queue->head == NULL)
	{
		printf("Underflow\n");
		return NULL;
	}
	else
	{
		tnode *temp = tree_queue->head;
		tree_queue->head = tree_queue->head->next;
		temp->next = NULL;
		return temp;
	}
}

void print(tnode *b)
{
	if(b != NULL)
	{
		printf("%d %c \n", b->freq, b->c);
		print(b->left);
		print(b->right);
	}
}

void print_pre_order(tnode *huff_tree)
{
	if (huff_tree != NULL)
	{
		printf("%c", huff_tree->c);
		print_pre_order(huff_tree->left);
		print_pre_order(huff_tree->right);
	}
}
