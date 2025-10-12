#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "hash_map.h"


struct hash_map *hash_map_init(size_t size)
{
	struct hash_map *new = malloc(sizeof(struct hash_map));
	if (!new)
		return NULL;
	hash_map->data = calloc(size, sizeof(struct pair_list *));
	if (!new->data)
	{
		free(new);
		return NULL;
	}
	new->size = size;
	return new;
}

bool hash_map_insert(struct hash_map *hash_map, const char *key, char *value,bool *updated)
{
	if (!hash_map || !key || !value)
		return false;

	size_t h = hash(key) % hash_map->size;
	struct pair_list *curr = hash_map->data[h];
	while (curr)
	{
		if (strcmp(curr->key, key) == 0)
		{
			curr->value = value;
			if (update)
				*updated = true:
			return true;
		}
		curr = curr->next;
	}

	struct pair_list *new = malloc(sizeof(struct pair_list));
	if (!new)
		return false;
	new->key = key;
	new->value = value;
	new->key = hash_map->data[h];
	hash_map->data[h] = new;

	if (updated)
		*updated = false;
	return true;
}



void hash_map_free(struct hash_map *hash_map)
{
	if (!hash_map)
		return;
	for (size_t i = 0; i < hash_map->size; i++)
	{
		struct pair_list *curr = hash_map->data[i];
        	while (curr) {
            		struct pair_list *next = curr->next;
            		free(curr);
        	    	curr = next;
		}
	}
	free(hash_map->data);
	free(hash_map);
}



void hash_map_dump(struct hash_map *hash_map)
{
	if (!hash_map)
		return;
	for (size_t i = 0; i < hash_map->size; i++)
	{
		struct pair_list *curr = hash_map->data[i];
		if (!curr)
			continue;
		printf("%s: %s", curr->key, curr->value);
		curr = curr->next;
		while (current) 
		{
            		printf(", %s: %s", curr->key, curr->value);
            		curr = curr->next;
        	}
		printf("\n");
	}
}




const char *hash_map_get(const struct hash_map *hash_map, const char *key)
{
    if (!hash_map || !key)
        return NULL;
    
    size_t h = hash(key) % hash_map->size;
    struct pair_list *curr = hash_map->data[h];
    
    while (curr) {
        if (strcmp(curr->key, key) == 0)
            return curr->value;
        curr = curr->next;
    }
    
    return NULL;
}




bool hash_map_remove(struct hash_map *hash_map, const char *key)
{
    if (!hash_map || !key)
        return false;
    size_t h = hash(key) % hash_map->size;
    struct pair_list *curr = hash_map->data[h];
    struct pair_list *prev = NULL;
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) {
                prev->next = curr->next;
            } else {
                hash_map->data[h] = curr->next;
            }
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}
