#include "evalexpr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void print_usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s [-rpn]\n", program_name);
}

int main(int argc, char *argv[])
{
    int rpn_mode = 0;
    
    if (argc > 1) {
        if (strcmp(argv[1], "-rpn") == 0) {
            rpn_mode = 1;
        } else {
            print_usage(argv[0]);
            return 4;
        }
    }
    
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        return 0;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    int result;
    if (rpn_mode) {
        result = eval_rpn(buffer);
    } else {
        result = eval_rpn(buffer);
    }
    
    printf("%d\n", result);
    return 0;
}
