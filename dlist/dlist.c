#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include "dlist.h"




static unsigned int min3(unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}





struct dlist *dlist_init(void)
{
	struct dlist *new_dlist = malloc(sizeof(struct dlist));
	new_dlist->size = 0;
	new_dlist->head = NULL;
	new_dlist->tail = NULL;
	return new_dlist;
}

int dlist_push_front(struct dlist *list, int element) 
{
	if (element < 0)
		return 0;
	struct dlist_item *new = malloc(sizeof(struct dlist_item));
	if (!new)
		return 0;

	new->data = element;
	new->prev = NULL;
	new->next = list->head;

	if (list->head)
		list->head->prev = new;
	else
		list->tail = new;
	list->head = new;
	list->size++;
	return 1;
}

void dlist_print(const struct dlist *list)
{
	if (list->size == 0)
		return;
	struct dlist_item *cur = list->head;
	while (cur != NULL)
	{
		printf("%d\n",cur->data);
		cur = cur->next;
	}
}


int dlist_push_back(struct dlist *list, int element)
{
	if (element < 0 || !list)
	       return 0;

	struct dlist_item *new = malloc(sizeof(struct dlist_item));
	new->data = element;
	new->next = NULL;
	new->prev = list->tail;

	if (list->tail)
		list->tail->next = new;
	else
		list->head = new;

	list->tail = new;
	list->size++;
	return 1;
}


size_t dlist_size(const struct dlist *list)
{
	if (!list)
	       return 0;
	return list->size;
}	

int dlist_get(const struct dlist *list, size_t index)
{
	if (!list || index > list->size)
		return -1;
	
	struct dlist_item *current = list->head;
	size_t i = 0;
	while ( current && i < index)
	{
		current = current->next;
		i++;
	}
	return current->data;
}

int dlist_insert_at(struct dlist *list, int element, size_t index)
{
	if ( element < 0 || index > list->size || !list)
               return 0;

	if (index == 0)
	       return dlist_push_front(list, element);
    
    	if (index == list->size)
        	return dlist_push_back(list, element);	
        
	struct dlist_item *new = malloc(sizeof(struct dlist_item));
        new->data = element;
        
	struct dlist_item *previ = list->head;
	for (size_t i = 0; i < index; i++)
	{
		previ = previ->next;
	}

	new->next = previ->next;
	new->prev = previ;

	if (previ->next)
		previ->next->prev = new;

	previ->next = new;
	list->size++;
	return 1;
}


int dlist_find(const struct dlist *list, int element)
{
	struct dlist_item *verif = list->head;
	for (size_t i = 0; i < list->size; i++)
	{
		if (verif->data == element)
			return i;
		verif = verif->next;
	}
	return -1;
}
		

int dlist_remove_at(struct dlist *list, size_t index)
{
	if (!list || index >= list->size)
        	return -1;

    	struct dlist_item *aremove;
    	int value;

	if (index == 0)
	{
		aremove = list->head;
		list->head = aremove->next;
        
        	if (list->head) 
            		list->head->prev = NULL;
        	else 
            		list->tail = NULL;
    	}
   
    	else if (index == list->size - 1)
    	{
        	aremove = list->tail;
        	list->tail = aremove->prev;
        	list->tail->next = NULL;
    	}
    
    	else
    	{
        
        	aremove = list->head;
        	for (size_t i = 0; i < index; i++)
        	{
            		aremove = aremove->next;
        	}

        	aremove->prev->next = aremove->next;
        	aremove->next->prev = aremove->prev;
    	}

    	value = aremove->data;
    	free(aremove);
    	list->size--;
    
    	return value;
}


void dlist_clear(struct dlist *list)
{
	if (list == NULL)
		return;
	while (list->head != NULL)
	{
		struct dlist_item *first = list->head;
		list->head = first->next;
		free(first);
	}
	list->tail = NULL;
	list->size = 0;
}


void dlist_map_square(struct dlist *list)
{
	struct dlist_item *curr = list->head;
	while (curr != NULL)
	{
		curr->data = curr->data * curr->data;
		curr = curr->next;
	}
}

void dlist_reverse(struct dlist *list)
{
	if (list->head == NULL)
		return;
	struct dlist_item *curr = list->head;
	struct dlist_item *tem = NULL;

	list->head = list->tail;
	list->tail = curr;
	while (curr != NULL)
	{
		tem = curr->prev;
		curr->prev = curr->next;
		curr->next = tem;
		curr = curr->prev;
	}
}



struct dlist *dlist_split_at(struct dlist *list, size_t index)
{
	if (!list || index > list->size)
		return NULL;

	if (index == 0)
	{
		struct dlist *new_list = dlist_init();
		if (new_list == NULL)
			return NULL;
		new_list->head = list->head;
		new_list->tail = list->tail;
		new_list->size = list->size;

		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		return new_list;
	}

	if (index == list->size)
		return dlist_init();

	struct dlist_item *current = list->head;
    	for (size_t i = 0; i < index; i++)
    	{
        	current = current->next;
    	}
	
	struct dlist *new_list = dlist_init();
	if (new_list == NULL)
		return NULL;
	new_list->head = current;
	new_list->tail = list->tail;
	new_list->size = list->size - index;
	list->tail = current->prev;
	list->tail->next = NULL;
    	list->size = index;
	current->prev = NULL;

	return new_list;
}


void dlist_concat(struct dlist *list1, struct dlist *list2)
{
	if (!list1)
		return;
	if (!list2 || !list2->head)
		return;

	if (!list1->head)
	{
		list1->head = list2->head;
		list1->tail = list2->tail;
		list1->size = list2->size;
	}
	else
	{
		list1->tail->next = list2->head;
        	list2->head->prev = list1->tail;
		list1->tail = list2->tail;
		list1->size += list2->size;
	}
	list2->head = NULL;
    	list2->tail = NULL;
    	list2->size = 0;
}



unsigned int dlist_levenshtein(const struct dlist *list1, const struct dlist *list2)
{
    if (!list1 || !list2)
        return 0;

    size_t len1 = list1->size;
    size_t len2 = list2->size;


    if (len1 == 0) return len2;
    if (len2 == 0) return len1;


    unsigned int **matrix = malloc((len1 + 1) * sizeof(unsigned int *));
    for (size_t i = 0; i <= len1; i++)
    {
        matrix[i] = malloc((len2 + 1) * sizeof(unsigned int));
    }

    for (size_t i = 0; i <= len1; i++)
        matrix[i][0] = i;
    for (size_t j = 0; j <= len2; j++)
        matrix[0][j] = j;

    struct dlist_item *item1 = list1->head;
    for (size_t i = 1; i <= len1; i++)
    {
        struct dlist_item *item2 = list2->head;
        for (size_t j = 1; j <= len2; j++)
        {
            int cost = (item1->data == item2->data) ? 0 : 1;

            matrix[i][j] = min3(
                matrix[i-1][j] + 1,   
                matrix[i][j-1] + 1,    
                matrix[i-1][j-1] + cost
            );

            item2 = item2->next;
        }
        item1 = item1->next;
    }

    unsigned int result = matrix[len1][len2];

    for (size_t i = 0; i <= len1; i++)
        free(matrix[i]);
    free(matrix);

    return result;
}
