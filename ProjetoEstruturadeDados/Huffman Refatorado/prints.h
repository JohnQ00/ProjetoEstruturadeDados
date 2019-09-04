
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