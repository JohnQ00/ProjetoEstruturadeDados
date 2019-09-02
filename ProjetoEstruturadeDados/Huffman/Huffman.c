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
tnode *create_tnode(byte c, tnode *left, tnode *right);
void counting_frequence(FILE *input, lint *frequence);
void make_frequence(queue *tree_queue, lint *frequence);
void compress(FILE *input, char *archive, queue *tree_queue);
void decompress(FILE *input);
tnode *decompress_tree(FILE *input, tnode *huff_tree, int tree_size, int counter);
void construct_file(FILE *input, char *archive, tnode *huff_tree, hash * new_hash);
byte set_bit(byte aux, int i);
byte deset_bit(byte c, int i);
int is_bit_set(byte aux, int i);
int is_dualbit_set(dual_byte aux, int i);
void put_tree_in_output(tnode *huff_tree, FILE *output);
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
        decompress(input);
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
	int bin_size;
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

void decompress(FILE *input)
{
    tnode *huff_tree = NULL;
    byte c;
    byte one, two;

    fseek(input,-1, SEEK_END);
    byte bit = fgetc(input);
    long long int input_bytes = ftell(input);
    rewind(input);

    fread(&one, 1, 1, input);
    fread(&two, 1, 1, input);

    lint trash_size = 0;
    lint tree_size = 0;

    int counter = 0;
    int i = 0;
    int aux = 0;

    int file_start;

    for (i = 0; i < 16; i++)
    {
    	if (i < 8)
    	{
    		if (is_bit_set(two, i))
    		{
    			tree_size += pow(2,i);
    		}
    	}
    	else
    	{
    		if (i < 13)
    		{
    			if (is_bit_set(one, aux))
    			{
    				tree_size += pow(2,i);
    			}
    			aux++;
    		}
    		else
    		{
    			if (is_bit_set(one, aux))
    			{
    				trash_size += pow(2,counter);
    			}
    			aux++;
    			counter++;
    		}
    	}
    }

     printf("lixo : %d e arvore: %d", trash_size, tree_size);
     counter = 0;
     huff_tree = decompress_tree(input, huff_tree, tree_size, 0);

     printf("\n");
     print_pre_order(huff_tree);

     FILE *output = fopen("decompressed.mp4", "w+b");

     int j;
     tnode *root = huff_tree;

     while(1)
     {
     	fscanf(input, "%c", &c);

     	for (j = 7; j >= 0 ; j--)
     	{
     		if (is_bit_set(c,j))
     		{
     			huff_tree = huff_tree->right;
     		}
     		else
			{
				huff_tree = huff_tree->left;
			}
			if (huff_tree->left == NULL && huff_tree->right == NULL)
			{
				fputc(huff_tree->c, output);
				huff_tree = root;
			}
     	}
     	if (ftell(input) == (input_bytes - 1))
     	{
     		break;
     	}
     }

     for (j = 7; j >= trash_size; j--)
     {
     	if (is_bit_set(bit, j))
     	{
     		huff_tree = huff_tree->right;
     	}

     	else
     	{
     		huff_tree = huff_tree->left;
     	}

     	if (huff_tree->left == NULL && huff_tree->right == NULL)
     	{
     		fputc(huff_tree->c, output);
     		huff_tree = root;
     	}
     }

    printf("\n");
    puts("////////////////////");
    puts("Arquivo Descompactado");
    puts("////////////////////");
    printf("\n");
}

tnode *decompress_tree(FILE *input, tnode *huff_tree, int tree_size, int counter)
{
    byte c;

    if(counter == tree_size) return NULL;
    fscanf(input, "%c", &c);

    if(c == '\\')
    {
    	fscanf(input, "%c", &c);
    	return create_tnode(c, NULL, NULL);
    }
    else if (c != '*')
    {
    	return create_tnode(c, NULL, NULL);
    }
    else
    {
    	huff_tree = create_tnode(c, NULL, NULL);

    	huff_tree->left = decompress_tree(input, huff_tree->left, tree_size, counter + 1);
    	huff_tree->right = decompress_tree(input, huff_tree->right, tree_size, counter + 1);
    }

    return huff_tree;

}

tnode *create_tnode(byte c, tnode *left, tnode *right)
{
	tnode *temp = (tnode*)malloc(sizeof(tnode));
	temp->c = c;
	temp->freq = 0;
	temp->next = NULL;
	temp->left = left;
	temp->right = right;
	return temp;
}

void construct_file(FILE *input, char *archive, tnode *huff_tree, hash * new_hash)
{
	//puts("construct");
	//int archive_name = strlen(archive);

	//strcat(archive_name, ".huff");

	dual_byte tree_size = 0;
	count_tree_size(huff_tree, &tree_size);

	FILE *output = fopen("compressed.huff", "w+b"); // escreve o arquivo em modo binário

	fseek(output, 2 + (tree_size), SEEK_SET); // o inteiro no segundo parametro é o n de bytes, já quando coloca uma variável com tamanho é o numero de bits

	byte c = 0;
	byte aux = 0;
	int index = 7; // serve pra montar cada byte no arquivo de saída, 0 - 7
	int i;

	// while(fscanf(input, "%c", &c) != EOF)
	// {
	// 	printf("%d", new_hash->table[c]->bin_size);
	// }
	while(fscanf(input, "%c", &c) != EOF)
	{
		for (i = 0; i < new_hash->table[c]->bin_size; ++i)
		{
		    //printf("%d\n", new_hash->table[c]->bin_counter[i]);
			if (new_hash->table[c]->bin_counter[i] == 49)
			{
				aux = set_bit(aux,index);
			}
			index--;

			if (index == -1)
			{
				fwrite(&aux, sizeof(byte),1, output);

				aux = 0;

				index = 7;
			}
		}
	}


    dual_byte trash = index + 1;
    fwrite(&aux, sizeof(byte), 1, output);

	rewind(output);

/////////////////////////////////////////////////////////////////////////

	dual_byte aux2 = trash;
	aux2 = aux2 << 13;
	//printf("\n%d\n", aux2);

	printf("tree size: %d\n", tree_size);
	aux2 = aux2 + tree_size;
	printf("\nlixo: %d\n", aux2);
	//printf("\n%d\n", aux2);

	byte byte_one = 0;
	byte byte_two = 0;

	for(i = 15; i >= 8; i--)
    {
        if (is_dualbit_set(aux2, i))
        {
        	byte_one = set_bit(byte_one,i - 8);
        }
    }

    for(i = 7; i >= 0 ; i--)
    {
        if (is_bit_set(aux2, i))
        {
        	byte_two = set_bit(byte_two,i);
        }
    }

    printf("%d\n", byte_one);
    printf("%d\n", byte_two);
    fputc(byte_one, output);
    fputc(byte_two, output);
    put_tree_in_output(huff_tree,output);

    printf("\n");
    puts("////////////////////");
    puts("Arquivo Compactado");
    puts("////////////////////");
    printf("\n");
/////////////////////////////////////////////////////////////////////////
}

byte set_bit(byte aux, int i)
{
	byte mask = 1 << i;
	return mask | aux;
}

byte deset_bit(byte c, int i)
{
	int j;
	byte mask = 1 << i;
	byte r;
	for (j= 0; j < 7 ; ++j)
	{
		if (is_bit_set(c,j) && !is_bit_set(mask, j))
		{
			r = set_bit(r,j);
		}
	}
	return r;
}

int is_bit_set(byte aux, int i)
{
	byte mask = 1 << i;
	return mask & aux;
}

int is_dualbit_set(dual_byte aux, int i)
{
	dual_byte mask = 1 << i;
	return mask & aux;
}

void put_tree_in_output(tnode *huff_tree, FILE *output)
{
	if (huff_tree != NULL)
	{
		if ((huff_tree->c == '\\' || huff_tree->c == '*') && huff_tree->left == NULL && huff_tree->right == NULL )
		{
			fputc('\\', output);
		}

		fputc(huff_tree->c, output);
		put_tree_in_output(huff_tree->left, output);
		put_tree_in_output(huff_tree->right, output);
	}
}

void count_tree_size(tnode *huff_tree, dual_byte *t)
{

	if (huff_tree != NULL)
	{
		if ((huff_tree->c == '\\' || huff_tree->c == '*') && huff_tree->left == NULL && huff_tree->right == NULL )
		{
			*t += 1;
		}
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
	temp->bin_size = strlen(bin_counter);
	printf("%d \n", temp->bin_size);
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
