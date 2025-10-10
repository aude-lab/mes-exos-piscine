#ifndef EVALEXPR_H
#define EVALEXPR_H

#include <stddef.h>

typedef struct Stack {
    int *data;
    size_t size;
    size_t capacity;
} Stack;

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PAREN_LEFT,
    TOKEN_PAREN_RIGHT,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    int value;    
    char op;  
} Token;

Stack *stack_create(void);
void stack_push(Stack *s, int value);
int stack_pop(Stack *s);
int stack_peek(const Stack *s);
int stack_is_empty(const Stack *s);
void stack_free(Stack *s);

int eval_rpn(const char *expression);
int is_operator(char c);
int get_operator_precedence(char op);
int apply_operator(int a, int b, char op);

Token get_next_token(const char **expr);
void skip_whitespace(const char **expr);

#endif
