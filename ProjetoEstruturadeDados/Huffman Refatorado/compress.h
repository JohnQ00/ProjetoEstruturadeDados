
byte set_bit(byte aux, int i)
{
	byte mask = 1 << i;
	return mask | aux;
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

