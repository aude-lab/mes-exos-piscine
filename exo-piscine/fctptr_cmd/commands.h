#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
typedef int (*handler)(const char *arg1);

struct cmd
{
    handler handle;
    const char *command_name;
};

int cmd_help(const char *arg);
int cmd_hello(const char *arg);
int cmd_print(const char *arg);
int cmd_exit(const char *arg);
int cmd_cat(const char *arg);
extern const struct cmd commands[];
size_t get_commands_count(void);

#endif /* COMMANDS_H */
