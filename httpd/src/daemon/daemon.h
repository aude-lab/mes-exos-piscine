#ifndef DAEMON_H
#define DAEMON_H

#include "../config/config.h"

int daemonize(struct config *config);
void setup_signal_handlers(void);
int is_shutdown_requested(void);
int stop_daemon(struct config *config);

#endif /* !DAEMON_H */
