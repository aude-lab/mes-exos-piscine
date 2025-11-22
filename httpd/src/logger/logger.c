#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
static FILE *log_file = NULL;
static int logging_enabled = 0;

static int get_gmt_time(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm *gmt_time = gmtime(&now);
    
    if (gmt_time == NULL)
        return -1;
        
    strftime(buffer, size, "%a, %d %b %Y %H:%M:%S GMT", gmt_time);
    return 0;
}

static void write_log(const char *format, ...)
{
    if (!logging_enabled || !log_file)
        return;
        
    char timestamp[64];
    if (get_gmt_time(timestamp, sizeof(timestamp)) != 0)
        return;
        
    va_list args;
    va_start(args, format);
    
    fprintf(log_file, "%s ", timestamp);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");
    fflush(log_file);
    
    va_end(args);
}

int logger_init(struct config *config)
{
    if (config == NULL)
        return -1;
        
    logging_enabled = config->log;
    
    if (!logging_enabled)
        return 0;
        
    if (config->log_file != NULL)
    {
        log_file = fopen(config->log_file, "a");
        if (log_file == NULL)
        {
            log_file = stderr;
            fprintf(stderr, "Warning: Could not open log file '%s', using stderr\n", 
                    config->log_file);
        }
    }
    else
    {
        if (config->daemon != NO_OPTION)
        {
            log_file = fopen("HTTPd.log", "a");
            if (log_file == NULL)
                log_file = stderr;
        }
        else
        {
            log_file = stdout;
        }
    }
    
    return 0;
}

void log_request(const char *server_name, const char *request_type, 
                 const char *target, const char *client_ip)
{
    write_log("[%s] received %s on '%s' from %s", 
              server_name, request_type, target, client_ip);
}

void log_response(const char *server_name, int status_code, 
                  const char *client_ip, const char *request_type, 
                  const char *target)
{
    write_log("[%s] responding with %d to %s for %s on '%s'", 
              server_name, status_code, client_ip, request_type, target);
}

void log_bad_request(const char *server_name, const char *client_ip)
{
    write_log("[%s] received Bad Request from %s", server_name, client_ip);
    write_log("[%s] responding with 400 to %s", server_name, client_ip);
}

void log_method_not_allowed(const char *server_name, int status_code,
                            const char *client_ip, const char *target)
{
    write_log("[%s] received UNKNOWN on '%s' from %s", 
              server_name, target, client_ip);
    write_log("[%s] responding with %d to %s for UNKNOWN on '%s'", 
              server_name, status_code, client_ip, target);
}

void logger_cleanup(void)
{
    if (log_file != NULL && log_file != stdout && log_file != stderr)
    {
        fclose(log_file);
    }
    log_file = NULL;
    logging_enabled = 0;
}
