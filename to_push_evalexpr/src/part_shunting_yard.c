#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evalexpr.h"

int get_operator_priority(char op)
{
    switch (op)
    {
    case '^':
        return 4;
    case '*':
        return 3;
    case '/':
        return 3;
    case '%':
        return 3;
    case '+':
        return 2;
    case '-':
        return 2;
    default:
        return 0;
    }
}

int is_right_associative(char op)
{
    return op == '^';
}

int is_unary_operator(char op, const char *prev_token_end)
{
    if (prev_token_end == NULL || *prev_token_end == '\0'
        || is_a_operator(*prev_token_end) || *prev_token_end == '(')
    {
        return (op == '+' || op == '-');
    }
    return 0;
}

char *oneshunting_yard(const char *infix)
{
    struct stack *op_stack = stack_create();
    if (!op_stack)
        return NULL;
    char *output = malloc(1024 * sizeof(char));
    if (!output)
    {
        stack_free(op_stack);
        return NULL;
    }
    output[0] = '\0';
    size_t output_len = 0;

    const char *ptr = infix;
    const char *prev_token_end = NULL;
    struct Token token;

    while ((token = get_next_token(&ptr)).type != TOKEN_END)
    {
        if (token.type == TOKEN_NUMBER)
        {
            char num_str[20];
            snprintf(num_str, sizeof(num_str), "%d", token.value);
            strcat(output, num_str);
            strcat(output, " ");
            output_len += strlen(num_str) + 1;

            prev_token_end = ptr - 1;
        }
        else if (token.type == TOKEN_OPERATOR)
        {
            int unary = is_unary_operator(token.op, prev_token_end);
            int current_op = token.op;

            if (unary)
            {
                current_op = (token.op == '+') ? 'u' : 'm';
            }

            while (!stack_is_empty(op_stack))
            {
                int top_op = stack_peek(op_stack);

                if (top_op == '(')
                    break;

                int top_prec = get_operator_priority(top_op);
                int curr_prec = get_operator_priority(current_op);

                if ((!is_right_associative(current_op) && curr_prec <= top_prec)
                    || (is_right_associative(current_op)
                        && curr_prec < top_prec))
                {
                    char op_char = stack_pop(op_stack);
                    char op_str[4] = { op_char, ' ', '\0' };
                    strcat(output, op_str);
                    output_len += 2;
                }
                else
                {
                    break;
                }
            }

            stack_push(op_stack, current_op);
            prev_token_end = ptr - 1;
        }
        else if (token.type == TOKEN_PAREN_LEFT)
        {
            stack_push(op_stack, '(');
            prev_token_end = ptr - 1;
        }
        else if (token.type == TOKEN_PAREN_RIGHT)
        {
            while (!stack_is_empty(op_stack))
            {
                int op = stack_pop(op_stack);
                if (op == '(')
                    break;

                char op_str[4] = { op, ' ', '\0' };
                strcat(output, op_str);
                output_len += 2;
            }
            prev_token_end = ptr - 1;
        }
    }

    while (!stack_is_empty(op_stack))
    {
        int op = stack_pop(op_stack);
        if (op == '(')
        {
            free(output);
            stack_free(op_stack);
            return NULL;
        }

        char op_str[4] = { op, ' ', '\0' };
        strcat(output, op_str);
        output_len += 2;
    }

    if (output_len > 0 && output[output_len - 1] == ' ')
    {
        output[output_len - 1] = '\0';
    }

    stack_free(op_stack);
    return output;
}

int evalex_infix(const char *expression)
{
    char *rpn = oneshunting_yard(expression);
    if (rpn == NULL)
    {
        return 0;
    }

    int result = eval_rpn(rpn);
    free(rpn);
    return result;
}
