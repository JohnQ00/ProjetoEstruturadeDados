typedef struct queue queue;
typedef struct list tnode;
typedef struct hash hash;
typedef struct element element;
typedef long int lint;
typedef unsigned char byte;
typedef unsigned short dual_byte;

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