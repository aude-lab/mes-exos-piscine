#include <stdio.h>
#include <string.h>


struct leterfreq {
    char letter;
    int count;
};


void count_frequencies(const char text[], int freq[], int *unique_count) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            int index = text[i] - 'A';
            if (freq[index] == 0) (*unique_count)++;
            freq[index]++;
        }
    }
}


int constc_letter_array(int freq[], struct leterfreq letters[]) {
    int index = 0;
    for (int i = 0; i < 26; i++)
        if (freq[i] > 0) {
            letters[index].letter = 'A' + i;
            letters[index].count = freq[i];
            index++;
        }
    return index;
}


void sort_letters(struct leterfreq letters[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (letters[j].count < letters[j + 1].count) {
                struct leterfreq tmp = letters[j];
                letters[j] = letters[j + 1];
                letters[j + 1] = tmp;
            }
}


void freq_analysis(const char text[], const char table[]) {
    int freq[26] = {0}, unique_count = 0;
    struct leterfreq letters[26];
    char mapping[26];

    count_frequencies(text, freq, &unique_count);
    int letter_count = constc_letter_array(freq, letters);
    sort_letters(letters, letter_count);

    for (int i = 0; i < letter_count; i++)
        mapping[letters[i].letter - 'A'] = table[i];

    for (int i = 0; i < 26; i++)
        if (freq[i] > 0)
            printf("%c %c\n", 'A' + i, mapping[i]);
}










