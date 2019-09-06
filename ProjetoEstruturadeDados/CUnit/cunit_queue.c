#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

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
		tree_queue->head = tree_node;////
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

tnode *dequeue(queue *tree_queue)
{
	if (tree_queue->head == NULL)
	{
		//printf("Underflow\n");
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

queue *create_queue()
{
	queue *temp = (queue*)malloc(sizeof(queue));
	temp->head = NULL;
	return temp;
}

void test_enqueue()
{
  queue *h_queue = create_queue();
  enqueue(h_queue,'t',2, NULL, NULL);
  CU_ASSERT(h_queue->head != NULL);
}

void test_create_queue()
{
  queue *h_queue = create_queue();
  CU_ASSERT(h_queue->head == NULL);
  CU_ASSERT(h_queue != NULL);
}

void test_dequeue()
{
  queue *h_queue = (queue*)malloc(sizeof(queue));
  enqueue(h_queue, 't', 10, NULL, NULL);
  dequeue(h_queue);
  CU_ASSERT(h_queue != NULL);
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
  if (NULL == CU_add_test(pSuite, "enqueue - test 1", test_enqueue))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (NULL == CU_add_test(pSuite, "create_queue - test 2", test_create_queue))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (NULL == CU_add_test(pSuite, "create_dequeue - test 3", test_dequeue))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }


  CU_basic_run_tests();
  return CU_get_error();
}
