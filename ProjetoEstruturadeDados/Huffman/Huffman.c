#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


typedef struct queue queue;
typedef struct list tnode;
typedef struct hash hash;
typedef struct element element;
typedef long int lint;
typedef unsigned char byte;
typedef unsigned short dual_byte;

void set_frequence_to_zero(lint *frequence);
queue *create_queue();
hash *create_dictionary();
void counting_frequence(FILE *input, lint *frequence);
void make_frequence(queue *tree_queue, lint *frequence);
void compress(FILE *input, char *archive, queue *tree_queue);
void construct_file(FILE *input, char *archive, tnode *huff_tree, hash * new_hash);
void count_tree_size(tnode *huff_tree, dual_byte *t);
void enqueue(queue *tree_queue, byte c, lint freq, tnode *left, tnode *right);
void make_huff_tree(queue *tree_queue);
void make_hash(hash *ht, tnode *huff, byte *bin_counter, lint *i);
void string_to_hash(hash *ht, byte index, byte *bin_counter);
byte *add_left(byte *bin_counter, lint *i);
byte *add_right(byte *bin_counter, lint *i);
tnode *dequeue(queue *tree_queue);
void print(tnode *p_queue);
void print_pre_order(tnode *huff_tree);
void print_dictionary(hash *ht);

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
     }
     else
        puts("é pra descompressão");

	////////////////////////////////////////////////////
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

struct hash
{
	element *table[256];
};

struct element
{
	byte bin_counter[100];
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

hash *create_dictionary()
{
	hash *temp = (hash*)malloc(sizeof(hash));
	int i;
	for (i = 0; i < 256; ++i)
	{
		temp->table[i] = NULL;
	}
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

void compress(FILE *input, char *archive, queue *tree_queue)
{
	make_huff_tree(tree_queue);

	hash *new_hash = create_dictionary();

	tnode *hff = tree_queue->head;
	print(hff);

	tnode *huff_tree = tree_queue->head;
	printf("Tree -> ");
	print_pre_order(huff_tree);
	printf("\n");

	byte bin_counter[20] = {0};
	lint i = 0;
	make_hash(new_hash, tree_queue->head, bin_counter, &i);
	print_dictionary(new_hash);

	construct_file(input, &archive, huff_tree, new_hash);
}

void construct_file(FILE *input, char *archive, tnode *huff_tree, hash * new_hash)
{
	puts("construct");
	int archive_name = strlen(archive);

	strcat(archive_name, ".huff");

	int tree_size = 0;
	count_tree_size(huff_tree, &tree_size);

	printf("tree size: %d\n", tree_size);

	FILE *output = fopen(compressed.huff, "w+b"); // escreve o arquivo em modo binário

	fseek(output, );
}

void count_tree_size(tnode *huff_tree, dual_byte *t)
{
	puts("count");
	if (huff_tree != NULL)
	{
		// if ((huff_tree->c == '\\' || huff_tree->c == '*') && huff_tree->left == NULL && huff_tree->right == NULL )
		// {
		// 	*t += 1;
		// }
		*t += 1;

		count_tree_size(huff_tree->left, t);
		count_tree_size(huff_tree->right, t);
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

void make_hash(hash *ht, tnode *huff, byte *bin_counter, lint *i)
{
	if (huff != NULL)
	{
		if(huff->left == NULL && huff->right == NULL)
		{
			string_to_hash(ht, huff->c, bin_counter);

			bin_counter[*i] = NULL;
			*i -= 1;
			return;
		}

		bin_counter = add_left(bin_counter, i);
		make_hash(ht, huff->left, bin_counter, i);

		bin_counter = add_right(bin_counter, i);
		make_hash(ht, huff->right, bin_counter, i);

		bin_counter[*i] = NULL;
		*i -= 1;
	}
}

void string_to_hash(hash *ht, byte index, byte *bin_counter)
{
	element *temp = (element*)malloc(sizeof(element));
	strcpy(temp->bin_counter, bin_counter);
	ht->table[index] = temp;
}

byte *add_left(byte *bin_counter, lint *i)
{
	bin_counter[*i] = '0';
	*i += 1;
	return bin_counter;
}

byte *add_right(byte *bin_counter, lint *i)
{
	bin_counter[*i] = '1';
	*i += 1;
	return bin_counter;
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

void print_dictionary(hash *ht)
{
	int i;

	for (i = 0; i < 255; ++i)
	{
		if (ht->table[i] != NULL)
		{
			printf("%c ---> %s\n", i, ht->table[i]->bin_counter);
		}
	}
}
