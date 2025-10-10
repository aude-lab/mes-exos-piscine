#include "evalexpr.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int is_a_operator(char c)
{
    return c == '-' || c == '+' || c == '*' || c == '/';
}
static void skipe_space(const char **expr)
{
    while (**expr && isspace(**expr))
        (*expr)++;
}

int switch_operator_case(int a, int b, char operat)
{
    switch (operat)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0)
            return 0;
        return a / b;
    default:
        return 0;
    }
}

struct Token get_next_token(const char **expr)
{
    struct Token the_token = { TOKEN_END, 0, '\0' };

    skipe_space(expr);

    if (**expr == '\0')
    {
        the_token.type = TOKEN_END;
        return the_token;
    }

    if (isdigit(**expr))
    {
        the_token.type = TOKEN_NUMBER;
        the_token.value = 0;
        while (isdigit(**expr))
        {
            the_token.value = the_token.value * 10 + (**expr - '0');
            (*expr)++;
        }
        return the_token;
    }
    if (is_a_operator(**expr))
    {
        the_token.type = TOKEN_OPERATOR;
        the_token.op = **expr;
        (*expr)++;
        return the_token;
    }

    if (**expr == '(')  
    {
        the_token.type = TOKEN_PAREN_LEFT;
        (*expr)++;
        return the_token;
    }
    if (**expr == ')') 
    {
        the_token.type = TOKEN_PAREN_RIGHT;
        (*expr)++;
        return the_token;
    }



    the_token.type = TOKEN_END;
    return the_token;
}

int eval_rpn(const char *expression)
{
    struct stack *stack = stack_create();
    if (!stack)
        return 0;

    const char *ptr = expression;
    struct Token the_token = get_next_token(&ptr);

    while (the_token.type != TOKEN_END)
    {
        if (the_token.type == TOKEN_NUMBER)
        {
            stack_push(stack, the_token.value);
        }
        else if (the_token.type == TOKEN_OPERATOR)
        {
            if (stack->size < 2)
            {
                stack_free(stack);
                return 0;
            }

            int b = stack_pop(stack);
            int a = stack_pop(stack);
            int result = switch_operator_case(a, b, the_token.op);
            stack_push(stack, result);
        }
        else if (the_token.type == TOKEN_PAREN_LEFT || the_token.type == TOKEN_PAREN_RIGHT)
        {
            stack_free(stack);
            return 0;
        }
        
        the_token = get_next_token(&ptr);
    }

    if (stack->size != 1)
    {
        stack_free(stack);
        return 0;
    }

    int result = stack_pop(stack);
    stack_free(stack);
    return result;
}

void print_usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s [-rpn]\n", program_name);
}

int main(int argc, char *argv[])
{
    int rpn_mode = 0;

    if (argc > 1)
    {
        if (strcmp(argv[1], "-rpn") == 0)
        {
            rpn_mode = 1;
        }
        else
        {
            print_usage(argv[0]);
            return 4;
        }
    }

    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
    {
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    int result;
    if (rpn_mode)
    {
        result = eval_rpn(buffer);
    }
    else
    {
        result = eval_infix(buffer);
    }

    printf("%d\n", result);
    return 0;
}
