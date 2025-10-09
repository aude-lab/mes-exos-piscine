#include <stddef.h>
#include <stdio.h>
int count_words(const char *file_in)
{
    if (!file_in)
        return -1;
    FILE *file = fopen(file_in, "r");
    if (file == NULL)
        return -1;
    int count_carac = 0;
    int in_word = 0;
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\f')
        {
            if (in_word)
            {
                word_count++;
                in_word = 0;
            }
        }
        else
        {
            in_word = 1;
        }
    }
    if (in_word)
    {
        word_count++;
    }

    fclose(file);
    return word_count;
}
/*int main(void)
{
    int c = count_words("test.txt");
    printf("%d\n", c);
    return 0;
}*/
