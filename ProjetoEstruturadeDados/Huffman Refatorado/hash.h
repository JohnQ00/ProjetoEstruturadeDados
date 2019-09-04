
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
