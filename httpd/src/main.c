#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/config.h"
#include "logger/logger.h"
#include "server/server.h"
#include "utils/string/string.h"

int main(int argc, char *argv[])
{
    struct config *config = parse_configuration(argc, argv);

    if (!config)
    {
        fprintf(stderr, "Error: Invalid configuration\n");
        return 2;
    }

    if (logger_init(config) != 0)
    {
        fprintf(stderr, "Failed to initialize logger\n");
        config_destroy(config);
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
        config_destroy(config);
        return 1;
    }

    logger_cleanup();
    config_destroy(config);
    return 0;
}
