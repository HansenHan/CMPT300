#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode(int item)
{
	 struct nodeStruct* node = malloc(sizeof(struct nodeStruct));
	 node->item = item;
	 node->next = NULL;
	 return node;
}


/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node)
{
	if (node == NULL)
	{
		return;
	}
	struct nodeStruct *curr = *headRef;
	while(curr != NULL) // not at tail
	{
		*headRef = node;
	}
	node->next = *headRef;
	*headRef = node;
}


/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node)
{
	struct nodeStruct *curr = *headRef;

	if(curr == NULL)
	{
		*headRef = node;
	}
	if(node == NULL)
	{
		return;
	}

	while (curr->next != NULL)
	{
		curr = curr->next;
	}

	curr->next = node;
}


/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head)
{
	int count = 0;
	struct nodeStruct *curr = head;

	while (curr != NULL)
	{
		curr = curr->next;
		count++;
	}
	return count;
}


/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, int item)
{
	struct nodeStruct *curr = head;

	if(head != NULL)
	{

		while(curr->item != item && curr->next != NULL)
		{
			curr = curr->next;
		}
		if(curr->item == item) // item found
		{
			return curr;
		}
		else
			return NULL;
	}
	else   // head = NULL
		return NULL;

}


 // * Delete node from the list and free memory allocated to it.
 // * This function assumes that node has been properly set to a valid node 
 // * in the list. For example, the client code may have found it by calling 
 // * List_findNode(). If the list contains only one node, the head of the list 
 // * should be set to NULL.
 
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node)
{
	struct nodeStruct *curr = *headRef;
	struct nodeStruct *prev = NULL;

	if(curr->item == node->item)  //only one item in list
	{
		if(curr->next == NULL)
		{
			free(curr);
			*headRef = NULL;
		}
		else   // delete head and move head to next node
		{
			*headRef = curr->next;
			free(curr);
		}
		
	}


	while(curr->item != node->item && curr->next != NULL) 
	{
		prev = curr;
		curr = curr->next;
	}
	if(curr->item == node->item)  // find node in middle 
	{
		prev->next = curr->next;
		free(curr);
	}
	else if (curr->next == NULL)  // did not find node
	{
		return;
	}

}


/*
 * Sort the list in ascending order based on the item field.
 * Any sorting algorithm is fine.
 */
void List_sort (struct nodeStruct **headRef)
{
	struct nodeStruct *i,*j;
	for(i = *headRef;i != NULL && i->next != NULL; i = i->next)
	{
		struct nodeStruct *min = i;
		for(j = i->next; j != NULL; j = j->next)
		{
			if(j->item < i->item)
			{
				min = j;
			}
		}
		if(min != i)
		{
			int curr = min->item;
			min->item = i->item;
			i->item = curr;
		}
	}
}