#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct b_tree b_tree;
typedef struct l_list l_list;

struct b_tree{
	int value;
	b_tree *left;
	b_tree *right;
};

struct l_list{
	int value;
	l_list *next;
};

//Creates the new tree nodes.
b_tree* create_new_node(int value){
	b_tree *new_node = (b_tree *) malloc(sizeof(b_tree ));
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->value = value;
	return new_node;
}

//Build the tree randomly.
b_tree* construct(b_tree *bst, int number){
	if(bst == NULL){
		bst = create_new_node(number); // tava RANDMAX/2
	}
	else if(number < bst->value){
		if(bst->left != NULL)
			construct(bst->left, number);
		else
			bst->left = create_new_node(number);
	}
	else{
		if(bst->right != NULL)
			construct(bst->right, number);
		else
			bst->right = create_new_node(number);
	}
	return bst;
}

//Maps the tree to the desired node;
void map2(b_tree *bst, int number, int *counter){
	if(bst == NULL){
		printf("O elemento n existe na arvore!!!");
	}
	else{

		if(number == bst->value){
			printf("Existe o elemento\n");
			*counter += 1;
		}
		else if(number < bst->value){
			*counter += 1;
			map2(bst->left, number, counter);
		}
		else if(number > bst->value){
			*counter +=1;
			map2(bst->right, number, counter);
		}
	}
}

//Handles the tree search.
void tree_search(b_tree *bst, int number)
{
	int counter = 0;
	map2(bst, number, &counter);

	FILE *out2 = fopen("abb.csv", "a+");
	printf("number = %d\ncounter = %d\n\n", number, counter);
	fprintf(out2, "%d,%d\n", number, counter);
	fclose(out2);
	//printf("comp: %d\n\n", x);
}

void print(b_tree *bst)
{
	if(bst != NULL){
		printf("%d ", bst->value);
		print(bst->left);
		print(bst->right);
	}
}

//Build the list randomly.
l_list* construct_list(l_list *list, int value)
{
	l_list *new_node = (l_list *) malloc(sizeof(l_list ));
	new_node->value = value;
	new_node->next = list;
	return new_node;
}

//Handles the list search.
void list_search(l_list *list, int item)
{
	l_list *aux = list;
	int comp=0, i=0;
	while(aux != NULL){
		comp++;
		if(aux->value == item){
			i=1;
			break;
		}
		aux=aux->next;
	}

	printf("item: %d\n", item);
	printf("comp: %d\n\n", comp);

	FILE *out = fopen("list.csv", "a+");
	fprintf(out, "%d,%d\n", item, comp);
	fclose(out);
	
	// if(i) printf("O elemento existe na lista\ncomp: %d\n", comp);
	// else printf("O elemento nao existe na lista\ncomp: %d\n", comp);
}

void printlist(l_list *list){

	while(list != NULL){
		printf("%d\n", list->value);
		list = list->next;
	}
}
int main()
{
	//LEMBRAR QUE CADA VEZ Q RODA O CODIGO, ELE ESCREVE NO FINAL DO .CSV SE JÁ EXISTIR UM abb.csv ou list.csv ; 
	b_tree *bst = NULL;
	l_list *list = NULL;
	int x,i;
	
	srand((unsigned) time(NULL));

	printf("\n\nDigite um numero: ");
	scanf("%d", &x);
	
	for(i = 0; i < 50000; i++)
	{
		x = 0 + ( rand() % 10000 );
		bst = construct(bst, x);
		list = construct_list(list, x);
	}
	for(i =0; i< 50000;i++){
		x = 0 + ( rand() % 10000 );
		tree_search(bst, x);
		list_search(list, x);

	}
	

	//print(bst);

	return 0;
}
