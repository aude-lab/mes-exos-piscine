#include "config.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/string/string.h"

static char *my_strdup(const char *str)
{
    if (!str)
        return NULL;
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
        return NULL;
    memcpy(copy, str, strlen(str) + 1);
    return copy;
}

static struct config *init_config(void)
{
    struct config *new = malloc(sizeof(struct config));
    if (!new)
        return NULL;
    new->pid_file = NULL;
    new->log_file = NULL;
    new->log = true;

    new->servers = malloc(sizeof(struct server_config));
    if (!new->servers)
        return NULL;
    new->servers->server_name = string_create(NULL, 0);
    new->servers->port = NULL;
    new->servers->ip = NULL;
    new->servers->root_dir = NULL;
    new->servers->default_file = my_strdup("index.html");

    new->daemon = NO_OPTION;
    return new;
}

static enum daemon parse_daemon_option(const char *val)
{
    if (val == NULL)
        return NO_OPTION;
    if (strcmp(val, "start") == 0)
        return START;
    else if (strcmp(val, "stop") == 0)
        return STOP;
    else if (strcmp(val, "restart") == 0)
        return RESTART;
    else
        return NO_OPTION;
}

static bool parse_log_option(const char *value)
{
    if (value == NULL)
        return true;

    if (strcmp(value, "true") == 0)
        return true;
    else
        return false;
}

static bool verify_mandatory_option(struct config *my_config)
{
    if (!my_config)
        return false;
    if (!my_config->pid_file)
        return false;
    if (my_config->daemon == STOP)
        return true;
    if (!my_config->servers->server_name
        || my_config->servers->server_name->size == 0)
        return false;
    if (!my_config->servers->port)
        return false;
    if (!my_config->servers->ip)
        return false;
    if (!my_config->servers->root_dir)
        return false;
    return true;
}

static bool set_default_log_file(struct config *config)
{
    if (config->daemon != NO_OPTION && config->log_file == NULL)
    {
        config->log_file = my_strdup("HTTPd.log");
        if (config->log_file == NULL)
            return false;
    }
    return true;
}

static bool handle_pid_file(struct config *config, const char *optarg)
{
    free(config->pid_file);
    config->pid_file = my_strdup(optarg);
    return (config->pid_file != NULL);
}

static bool handle_log_file(struct config *config, const char *optarg)
{
    free(config->log_file);
    config->log_file = my_strdup(optarg);
    return (config->log_file != NULL);
}

static bool handle_daemon(struct config *config, const char *optarg)
{
    config->daemon = parse_daemon_option(optarg);
    return !(config->daemon == NO_OPTION && optarg != NULL);
}

static bool handle_server_name(struct config *config, const char *optarg)
{
    if (config->servers->server_name != NULL)
        string_destroy(config->servers->server_name);

    config->servers->server_name = string_create(optarg, strlen(optarg));
    return (config->servers->server_name != NULL);
}

static bool handle_port(struct config *config, const char *optarg)
{
    free(config->servers->port);
    config->servers->port = my_strdup(optarg);
    return (config->servers->port != NULL);
}

static bool handle_ip(struct config *config, const char *optarg)
{
    free(config->servers->ip);
    config->servers->ip = my_strdup(optarg);
    return (config->servers->ip != NULL);
}

static bool handle_root_dir(struct config *config, const char *optarg)
{
    free(config->servers->root_dir);
    config->servers->root_dir = my_strdup(optarg);
    return (config->servers->root_dir != NULL);
}

static bool handle_default_file(struct config *config, const char *optarg)
{
    free(config->servers->default_file);
    config->servers->default_file = my_strdup(optarg);
    return (config->servers->default_file != NULL);
}

static struct option *get_long_options(void)
{
    static struct option long_options[] = {
        { "pid_file", required_argument, 0, 'p' },
        { "log_file", required_argument, 0, 'l' },
        { "log", required_argument, 0, 'g' },
        { "daemon", required_argument, 0, 'd' },
        { "server_name", required_argument, 0, 's' },
        { "port", required_argument, 0, 'o' },
        { "ip", required_argument, 0, 'i' },
        { "root_dir", required_argument, 0, 'r' },
        { "default_file", required_argument, 0, 'f' },
        { 0, 0, 0, 0 }
    };
    return long_options;
}

static bool process_option(struct config *config, int option,
                           const char *optarg)
{
    switch (option)
    {
    case 'p':
        return handle_pid_file(config, optarg);
    case 'l':
        return handle_log_file(config, optarg);
    case 'g':
        config->log = parse_log_option(optarg);
        return true;
    case 'd':
        return handle_daemon(config, optarg);
    case 's':
        return handle_server_name(config, optarg);
    case 'o':
        return handle_port(config, optarg);
    case 'i':
        return handle_ip(config, optarg);
    case 'r':
        return handle_root_dir(config, optarg);
    case 'f':
        return handle_default_file(config, optarg);
    case '?':
        return false;
    default:
        return false;
    }
}

static bool parse_options(struct config *config, int argc, char **argv)
{
    struct option *long_options = get_long_options();
    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        if (!process_option(config, c, optarg))
            return false;
    }
    return true;
}

struct config *parse_configuration(int argc, char **argv)
{
    struct config *config = init_config();
    if (config == NULL)
        return NULL;

    if (!parse_options(config, argc, argv))
    {
        config_destroy(config);
        return NULL;
    }

    if (!verify_mandatory_option(config))
    {
        config_destroy(config);
        return NULL;
    }

    if (!set_default_log_file(config))
    {
        config_destroy(config);
        return NULL;
    }

    return config;
}

void config_destroy(struct config *config)
{
    if (config == NULL)
        return;

    free(config->pid_file);
    free(config->log_file);

    if (config->servers != NULL)
    {
        if (config->servers->server_name != NULL)
            string_destroy(config->servers->server_name);

        free(config->servers->port);
        free(config->servers->ip);
        free(config->servers->root_dir);
        free(config->servers->default_file);

        free(config->servers);
    }
    free(config);
}
