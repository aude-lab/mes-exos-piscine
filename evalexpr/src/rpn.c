#include "evalexpr.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

int apply_operator(int a, int b, char op)
{
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) return 0; 
            return a / b;
        case '%':
            if (b == 0) return 0; 
            return a % b;
        case '^':
            if (b < 0) return 0;
            int result = 1;
            for (int i = 0; i < b; i++)
                result *= a;
            return result;
        default: return 0;
    }
}

Token get_next_token(const char **expr)
{
    Token token = {TOKEN_END, 0, '\0'};
    
    skip_whitespace(expr);
    
    if (**expr == '\0') {
        token.type = TOKEN_END;
        return token;
    }
    
    if (isdigit(**expr)) {
        token.type = TOKEN_NUMBER;
        token.value = 0;
        
        while (isdigit(**expr)) {
            token.value = token.value * 10 + (**expr - '0');
            (*expr)++;
        }
        return token;
    }
    
    if (is_operator(**expr)) {
        token.type = TOKEN_OPERATOR;
        token.op = **expr;
        (*expr)++;
        return token;
    }
    
    token.type = TOKEN_END;
    return token;
}

void skip_whitespace(const char **expr)
{
    while (**expr && isspace(**expr))
        (*expr)++;
}

int eval_rpn(const char *expression)
{
    Stack *stack = stack_create();
    if (!stack) return 0;
    
    const char *ptr = expression;
    Token token;
    
    while ((token = get_next_token(&ptr)).type != TOKEN_END) {
        if (token.type == TOKEN_NUMBER) {
            stack_push(stack, token.value);
        } else if (token.type == TOKEN_OPERATOR) {
            if (stack->size < 2) {
                stack_free(stack);
                return 0; 
            }
            
            int b = stack_pop(stack);
            int a = stack_pop(stack);
            int result = apply_operator(a, b, token.op);
            stack_push(stack, result);
        }
    }
    
    if (stack->size != 1) {
        stack_free(stack);
        return 0; 
    }
    
    int result = stack_pop(stack);
    stack_free(stack);
    return result;
}
