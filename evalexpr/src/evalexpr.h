#ifndef EVALEXPR_H
#define EVALEXPR_H

#include <stddef.h>

struct stack
{
    int *data;
    size_t size;
    size_t capacity;
};

enum Token_type
{
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PAREN_LEFT,
    TOKEN_PAREN_RIGHT,
    TOKEN_END
};

struct Token
{
    enum Token_type type;
    int value;
    char op;
};


typedef struct {
    const char **ptr;
    const char **prev_token_end;
    struct stack *op_stack;
    char *output;
} ShuntingContext;



int stack_is_empty(struct stack *s);
struct stack *stack_create(void);
void stack_push(struct stack *s, int value);
int stack_pop(struct stack *s);
void stack_free(struct stack *s);
int stack_peek(struct stack *s);
struct Token get_next_token(const char **expr);
int stack_is_empty(struct stack *s);
int eval_rpn(const char *expression);
int is_a_operator(char c);
int get_operator_priority(char op);
int switch_operator_case(int a, int b, char op);
char *oneshunting_yard(const char *infix);
int evalex_infix(const char *expression);
int is_unary_operator(char op, const char *prev_token_end);

void process_token(struct Token *token, ShuntingContext *ctx);
void handle_number(struct Token *token, char *output);
void handle_operator(struct Token *token, const char **prev_token_end, struct stack *op_stack, char *output);
void handle_right_paren(struct stack *op_stack, char *output);
void append_operator(char *output, char op);
int flush_remaining_ops(struct stack *op_stack, char *output);

#endif /* !EVALEXPR_H */
