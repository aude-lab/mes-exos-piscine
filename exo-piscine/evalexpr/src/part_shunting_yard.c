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





void process_token(struct Token *token, ShuntingContext *ctx)
{
    switch (token->type) {
        case TOKEN_NUMBER:
            handle_number(token, ctx->output);
            *(ctx->prev_token_end) = *(ctx->ptr) - 1;
            break;

        case TOKEN_OPERATOR:
            handle_operator(token, ctx->prev_token_end, ctx->op_stack, ctx->output);
            *(ctx->prev_token_end) = *(ctx->ptr) - 1;
            break;

        case TOKEN_PAREN_LEFT:
            stack_push(ctx->op_stack, '(');
            *(ctx->prev_token_end) = *(ctx->ptr) - 1;
            break;

        case TOKEN_PAREN_RIGHT:
            handle_right_paren(ctx->op_stack, ctx->output);
            *(ctx->prev_token_end) = *(ctx->ptr) - 1;
            break;

        default:
            break;
    }
}



void handle_number(struct Token *token, char *output)
{
    char num_str[20];
    snprintf(num_str, sizeof(num_str), "%d ", token->value);
    strcat(output, num_str);
}


void handle_operator(struct Token *token, const char **prev_token_end,
                     struct stack *op_stack, char *output)
{
    int current_op = token->op;
    if (is_unary_operator(token->op, *prev_token_end))
        current_op = (token->op == '+') ? 'u' : 'm';

    while (!stack_is_empty(op_stack)) {
        int top_op = stack_peek(op_stack);
        if (top_op == '(') break;

        int top_prec = get_operator_priority(top_op);
        int curr_prec = get_operator_priority(current_op);

        if ((!is_right_associative(current_op) && curr_prec <= top_prec)
            || (is_right_associative(current_op) && curr_prec < top_prec)) {
            append_operator(output, stack_pop(op_stack));
        } else break;
    }

    stack_push(op_stack, current_op);
}


void handle_right_paren(struct stack *op_stack, char *output)
{
    while (!stack_is_empty(op_stack)) {
        int op = stack_pop(op_stack);
        if (op == '(') break;
        append_operator(output, op);
    }
}


void append_operator(char *output, char op)
{
    char op_str[4] = { op, ' ', '\0' };
    strcat(output, op_str);
}


int flush_remaining_ops(struct stack *op_stack, char *output)
{
    while (!stack_is_empty(op_stack)) {
        int op = stack_pop(op_stack);
        if (op == '(')
            return 0;
        append_operator(output, op);
    }

    size_t len = strlen(output);
    if (len > 0 && output[len - 1] == ' ')
        output[len - 1] = '\0';

    return 1;
}





char *oneshunting_yard(const char *infix)
{
    struct stack *op_stack = stack_create();
    if (!op_stack) return NULL;

    char *output = malloc(1024 * sizeof(char));
    if (!output) { stack_free(op_stack); return NULL; }
    output[0] = '\0';

    const char *ptr = infix;
    const char *prev_token_end = NULL;
    struct Token token;

    ShuntingContext ctx = { &ptr, &prev_token_end, op_stack, output };

    while ((token = get_next_token(&ptr)).type != TOKEN_END) {
        process_token(&token, &ctx);
    }

    if (!flush_remaining_ops(op_stack, output)) {
        free(output);
        output = NULL;
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
