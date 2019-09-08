#include <stdio.h>
#include <stdlib.h>

#define CHAR_SIZE 256

typedef struct trie trie;
trie *new_trie();
void insert(trie *head, char *str);
int search(trie *head, char *str);
int haveChildren(trie *head);
trie *delete(trie *head, char *str, int depth);

void main()
{
	trie *main_trie = new_trie();

    char stri[300];
    scanf("%s", stri);

    insert(main_trie, stri);
    printf("Encontrado ? %d\n", search(main_trie, stri));

	insert(main_trie, "structures");
	printf("Encontrado ? %d\n", search(main_trie, "stru"));

	insert(main_trie, "structs");
    printf("Encontrado ? %d\n", search(main_trie, "structs"));

    main_trie = delete(main_trie, "structs", 0);

    printf("Depois de deletado %d\n", search(main_trie, "structs"));
}

struct trie
{
	int isWord;
	trie *word[CHAR_SIZE];
};

trie *new_trie()
{
	trie *temp = (trie*)malloc(sizeof(trie));
	temp->isWord = 0;

	int i;
	for (i = 0; i < CHAR_SIZE; ++i)
		temp->word[i] = NULL;
	return temp;
}

void insert(trie *head, char *str)
{
	trie *temp = head; //começa da raíz
	while(*str)
	{
		if (temp->word[*str] == NULL) {temp->word[*str] = new_trie();}
		temp = temp->word[*str];
		str++;
	}
	temp->isWord = 1;
}

int search (trie *head,char *str)
{
	trie *walk = head;
    int k;
	for (k = 0; str[k]; k++)
	{
		if (walk -> word[str[k]] == NULL)
			return 0;
		walk = walk -> word[str[k]];
	}
	if (walk -> isWord == 1)
		return 1;
}

int haveChildren(trie *head)
{
	for (int i = 0; i < CHAR_SIZE; i++)
		if (head->word[i])
			return 1;

	return 0;
}

trie *delete(trie *head, char *str, int depth)
{

	if (head == NULL)
		return NULL;
	if (depth == strlen(str) )
	{

		if (head->isWord) head->isWord = 0;

		if (!haveChildren(head)) {
			free(head);
			head = NULL;
		}
		return head;
	}

	int index = str[depth];
	head->word[index] = delete(head->word[index], str, depth + 1);

	if (!haveChildren(head) && head->isWord == 0)
	{
		free(head);
		head = NULL;
	}

	return head;
}
