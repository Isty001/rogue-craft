#ifndef ROGUE_CRAFT_LOGGER_H
#define ROGUE_CRAFT_LOGGER_H


void log_init(void);

void log_debug(char *msg, ...);

void log_info(char *msg, ...);

void log_alert(char *msg, ...);

void log_error(char *msg, ...);

void log_cleanup(void);


#endif
