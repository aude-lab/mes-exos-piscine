#include "daemon.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../config/config.h"

static volatile sig_atomic_t g_shutdown = 0;

static void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        g_shutdown = 1;
    }
}

int is_shutdown_requested(void)
{
    return g_shutdown;
}


void setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}

static int write_pid_file(const char *pid_file)
{
    FILE *file = fopen(pid_file, "w");
    if (!file)
    {
        fprintf(stderr, "Failed to create pid file: %s\n", strerror(errno));
        return -1;
    }

    fprintf(file, "%d\n", getpid());
    fclose(file);
    return 0;
}

static int check_existing_process(const char *pid_file)
{
    FILE *file = fopen(pid_file, "r");
    if (!file)
        return 0;

    pid_t existing_pid;
    if (fscanf(file, "%d", &existing_pid) == 1)
    {
        if (kill(existing_pid, 0) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int stop_daemon(struct config *config)
{
    if (!config || !config->pid_file)
    {
        fprintf(stderr, "Error: No pid file specified\n");
        return 1;
    }
    FILE *file = fopen(config->pid_file, "r");
    if (!file)
    {
        fprintf(stderr, "Error: pid file '%s' not found\n", config->pid_file);
        return 1;
    }
    pid_t pid;
    if (fscanf(file, "%d", &pid) != 1)
    {
        fprintf(stderr, "Error: Invalid pid file format\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    if (kill(pid, 0) != 0)
    {
        fprintf(stderr, "Error: Process %d is not running\n", pid);
        remove(config->pid_file);
        return 1;
    }

    printf("Stopping server (PID: %d)...\n", pid);
    if (kill(pid, SIGINT) != 0)
    {
        fprintf(stderr, "Error: Failed to send signal to process %d\n", pid);
        return 1;
    }

    remove(config->pid_file);
    printf("Server stopped successfully\n");

    return 0;
}

int daemonize(struct config *config)
{
    if (check_existing_process(config->pid_file))
    {
        fprintf(stderr, "Error: Server is already running\n");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid > 0)
    {
        printf("Server started with PID: %d\n", pid);
        _exit(0);
    }

    int devnull = open("/dev/null", O_RDWR);
    if (devnull != -1)
    {
        dup2(devnull, STDIN_FILENO);
        dup2(devnull, STDOUT_FILENO);
        dup2(devnull, STDERR_FILENO);
        close(devnull);
    }

    if (write_pid_file(config->pid_file) != 0)
    {
        return -1;
    }

    return 0;
}
