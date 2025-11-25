#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

    setup_signal_handlers();

    if (config->daemon == STOP)
    {
        int result = stop_daemon(config);
        config_destroy(config);
        return result;
    }

    if (config->daemon == RESTART)
    {
        stop_daemon(config);
        if (daemonize(config) != 0)
        {
            config_destroy(config);
            return 1;
        }
    }
    else if (config->daemon == START)
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
