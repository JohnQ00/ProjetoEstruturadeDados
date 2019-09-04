
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

int is_bit_set(byte aux, int i)
{
    byte mask = 1 << i;
    return mask & aux;
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

     FILE *output = fopen("decompressed.jpg", "w+b");

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



