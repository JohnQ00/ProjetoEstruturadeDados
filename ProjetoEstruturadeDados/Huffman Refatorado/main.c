#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "global.h"
#include "hash.h"
#include "decompress.h"
#include "compress.h"
#include "prints.h"


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

void make_frequence(queue *tree_queue, lint *frequence)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		if (frequence[i] != 0)
		{
			enqueue(tree_queue, i, frequence[i], NULL, NULL); // Tecnicamente,transforma um array numa lista com prioridade/fila
		}
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
}

queue *create_queue()
{
	queue *temp = (queue*)malloc(sizeof(queue));
	temp->head = NULL;
	return temp;
}


void compress(FILE *input, char *archive, queue *tree_queue)
{
	make_huff_tree(tree_queue);

	hash *new_hash = create_dictionary();

	//tnode *hff = tree_queue->head;
	//print(hff);

	tnode *huff_tree = tree_queue->head;
	printf("Tree -> ");
	print_pre_order(huff_tree);
	printf("\n");

	byte bin_counter[20] = {0};
	lint i = 0;
	int j = 0;
	make_hash(new_hash, tree_queue->head, bin_counter, &i);
	//for(i = 0; i < 256; i++)
        //{for(j = 0;j < 20;j++){if(new_hash->table[i]->bin_counter != 0){printf("bin: %d\n", new_hash->table[i]->bin_counter[j]);}}}
	print_dictionary(new_hash);

	construct_file(input, &archive, huff_tree, new_hash);
}

void set_frequence_to_zero(lint *frequence)
{
	int i;
	for (i = 0; i < 256; ++i)
		frequence[i] = 0;
}

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
	 printf("Compactar ou Descompactar ?\n");
	 printf("1) Compactar\n2) Descompactar\n");
	 int choice;
	 scanf("%d", &choice);

     if(choice == 1)
     {
        counting_frequence(input, frequence);
        make_frequence(new_queue, frequence);
        compress(input, &archive, new_queue);
        fclose(input);
     }
     else
        decompress(input);
        fclose(input);
	////////////////////////////////////////////////////
}
