#include <stdio.h>
#include <string.h>

void freq_analysis(const char text[], const char table[])
{
    int freq[26] = {0}; 
    int unique_count = 0;
    
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            int index = text[i] - 'A';
            if (freq[index] == 0)
                unique_count++;
            freq[index]++;
        }
    }
    
    struct LetterFreq {
        char letter;
        int count;
    } letters[26];
    
    int letter_index = 0;
    for (int i = 0; i < 26; i++)
    {
        if (freq[i] > 0)
        {
            letters[letter_index].letter = 'A' + i;
            letters[letter_index].count = freq[i];
            letter_index++;
        }
    }
    
    
    for (int i = 0; i < unique_count - 1; i++)
    {
        for (int j = 0; j < unique_count - i - 1; j++)
        {
            if (letters[j].count < letters[j + 1].count)
            {
               
                struct LetterFreq temp = letters[j];
                letters[j] = letters[j + 1];
                letters[j + 1] = temp;
            }
        }
    }
    
    char mapping[26]; 
    
    for (int i = 0; i < unique_count; i++)
    {
        int cipher_index = letters[i].letter - 'A';
        mapping[cipher_index] = table[i];
    }
    
    for (int i = 0; i < 26; i++)
    {
        if (freq[i] > 0) 
        {
            char cipher_letter = 'A' + i;
            char original_letter = mapping[i];
            printf("%c %c\n", cipher_letter, original_letter);
        }
    }
}
