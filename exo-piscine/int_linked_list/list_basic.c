#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct list *list_prepend(struct list *list, int value)
{
    struct list *new = malloc(sizeof(struct list));
    if (!new)
        return NULL;
    new->data = value;
    new->next = list;
    return new;
}

size_t list_length(struct list *list)
{
    if (!list)
        return 0;
    size_t len = 1;
    struct list *verif = list->next;
    while (verif)
    {
	verif = verif->next;
        len++;
    }

    return len;
}

void list_print(struct list *list)
{
    if (!list)
        return;
    
    struct list *verif = list;
    while (verif)
    {
        printf("%d", verif->data);

	if (verif->next)
		printf(" ");
        verif = verif->next;
    }
    printf("\n");
}

void list_destroy(struct list *list)
{
	struct list *verif = list;
	while (verif)
	{
		struct list *next = verif->next;
		free(verif);
		verif = next;
	}
}
