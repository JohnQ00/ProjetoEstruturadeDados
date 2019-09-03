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
		bst = create_new_node(RAND_MAX/2);
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

//Maps the tree to the desired node.
int map(b_tree *bst, int number, int comp){
	if(bst == NULL){
		printf("O elemento nao existe na arvore\n");
		return comp;
	}
	else if(number == bst->value){
		printf("O elemento existe na arvore\n");
		return comp+1;
	}
	else if(number < bst->value){
		comp = map(bst->left, number, ++comp);
	}
	else if(number > bst->value){
		comp = map(bst->right, number, ++comp);
	}
	
	return comp;
}

//Handles the tree search.
void tree_search(b_tree *bst, int number){
	int x = map(bst, number, 0);
	printf("comp: %d\n\n", x);
}

void print(b_tree *bst){
	printf("%d ", bst->value);
	if(bst->left != NULL)
		print(bst->left);
	if(bst->right != NULL)
		print(bst->right);
}

//Build the list randomly.
l_list* construct_list(l_list *list, int value){
	l_list *new_node = (l_list *) malloc(sizeof(l_list ));
	new_node->value = value;
	new_node->next = list;
	return new_node;
}

//Handles the list search.
void list_search(l_list *list, int item){
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
	if(i) printf("O elemento existe na lista\ncomp: %d\n", comp);
	else printf("O elemento nao existe na lista\ncomp: %d\n", comp);
}

int main(){
	b_tree *bst = NULL;
	l_list *list = NULL;
	int x;
	
	srand((unsigned) time(NULL));
	
	for(int i = 0; i<10000; i++){
		x = rand();
		bst = construct(bst, x);
		list = construct_list(list, x);
	}
	print(bst);
	printf("\n\nDigite um numero: ");
	scanf("%d", &x);
	tree_search(bst, x);
	list_search(list, x);

	return 0;
}
