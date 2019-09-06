#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

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
	return temp;////
}

void test_create_dictionary()
{
  hash *new_hash = create_dictionary();
  CU_ASSERT(new_hash != NULL);
}

void test_put_in_hash()
{
  hash *h = create_dictionary();
  CU_ASSERT(h != NULL);

  if (h == NULL)
  {
  	return;
  }

  string_to_hash(h, 'x', "teste");
  CU_ASSERT(!strcmp(h->table['x'], "teste"));////
}

int main()
{
    // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
   // Sets the basic run mode, CU_BRM_VERBOSE will show maximum output of run details
   // Other choices are: CU_BRM_SILENT and CU_BRM_NORMAL
  CU_basic_set_mode(CU_BRM_VERBOSE);
   // Run the tests and show the run summary


  CU_pSuite pSuite = NULL;

// Add a suite to the registry
  pSuite = CU_add_suite("TESTES", 0, 0);
// Always check if add was successful
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
// Add the test to the suite
  if (NULL == CU_add_test(pSuite, "create_dictionary - test 1", test_create_dictionary))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (NULL == CU_add_test(pSuite, "put_in_hash - test 2", test_put_in_hash))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }


  CU_basic_run_tests();
  return CU_get_error();
}
