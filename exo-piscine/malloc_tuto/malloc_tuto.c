#include <stdio.h>
#include "malloc_tuto.h"
#include <stdlib.h>
#include <string.h>

int *create_array(size_t size)
{
	if (size == 0)
		return NULL;
	int *region = malloc(size * sizeof(int));
	if (!region)
		return NULL;
	return region;
}

void free_array(int *arr)
{
	if (arr != NULL°)
		free(arr);
}

void read_and_inc(int *v)
{
	printf("%d\n", *v);
	(*v)++;
}


char *my_strdup(const char *str)
{
	if (!str)
	       return NULL;

	size_t len_str = strlen(str);
	char *newstr = malloc(len_str + 1);
	if (!newstr)
		return NULL;
	memcpy(newstr, str, len_str + 1);
	return newstr;

}


char *my_strndup(const char *str, size_t n)
{
	if (!str)
		return NULL;
	size_t len_str = strlen(str);
	if (len_str + 1 ==  n)
		return my_strdup(str);

	 char *newstr = malloc(n + 1);
    	if (!newstr)
        	return NULL;
    	memcpy(newstr, str, n);
    	newstr[n] = '\0';
    	return newstr;
}

