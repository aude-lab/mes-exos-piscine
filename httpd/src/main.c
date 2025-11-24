#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/config.h"
#include "daemon/daemon.h"
#include "logger/logger.h"
#include "server/server.h"
#include "utils/string/string.h"

static int run_server(struct config *config)
{
    if (logger_init(config) != 0)
    {
        fprintf(stderr, "Failed to initialize logger\n");
        return 1;
    }

    printf("Configuration loaded successfully!\n");
    printf("   Port: %s\n", config->servers->port);
    printf("   IP: %s\n", config->servers->ip);
    printf("   Logging: %s\n", config->log ? "enabled" : "disabled");

    if (config->log_file)
        printf("   Log file: %s\n", config->log_file);

    if (start_http_server(config->servers->ip, config->servers->port, config)
        != 0)
    {
        fprintf(stderr, "Failed to start server\n");
        logger_cleanup();
        return 1;
    }

    logger_cleanup();
    return 0;
}

int main(int argc, char *argv[])
{
    struct config *config = parse_configuration(argc, argv);

    if (!config)
    {
        fprintf(stderr, "Error: Invalid configuration\n");
        return 2;
    }

    if (config->daemon == STOP)
    {
        int result = stop_daemon(config);
        config_destroy(config);
        return result;
    }

    setup_signal_handlers();

    if (config->daemon == START)
    {
        if (daemonize(config) != 0)
        {
            config_destroy(config);
            return 1;
        }
    }

    int result = run_server(config);
    config_destroy(config);
    return result;
}
