#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include "storage/storage.h"
#include "ui/ncurses.h"
#include "logger.h"
#include "environment.h"


#define add_entry_with_args(level, msg)             \
    va_list args;                                   \
    va_start(args, msg);                            \
    add_entry(level, #level, msg, args);            \
    va_end(args);                                   \


#define DEBUG 4
#define INFO  3
#define ALERT 2
#define ERROR 1

#define MAX_LEN 500


static FILE *LOG_FILE;
static uint16_t LOG_LEVEL;


void log_init(void)
{
    LOG_FILE = file_open(getenv(ENV_LOG_FILE), "ab");
    LOG_LEVEL = atoi(getenv(ENV_LOG_LEVEL) ?: ALERT);
}

static format(char *buff, char *level_name, char *msg, va_list args)
{
    sprintf(buff, "[%s] - ", level_name);

    time_t now = time(NULL);
    strftime(buff + strlen(buff), 100, "%Y-%m-%d %H:%M:%S - ", localtime(&now));

    vsprintf(buff + strlen(buff), msg, args);
    strcat(buff, "\n");
}

static void add_entry(uint16_t level, char *level_name, char *msg, va_list args)
{
    if (level > LOG_LEVEL) {
        return;
    }

    char buff[MAX_LEN];
    format(buff, level_name, msg, args);

    fprintf(LOG_FILE, "%s", buff);
    fflush(LOG_FILE);
}

void log_debug(char *msg, ...)
{
    add_entry_with_args(DEBUG, msg);
}

void log_info(char *msg, ...)
{
    add_entry_with_args(INFO, msg);
}

void log_alert(char *msg, ...)
{
    add_entry_with_args(ALERT, msg);
}

void log_error(char *msg, ...)
{
    add_entry_with_args(ERROR, msg);
}

void log_cleanup(void)
{
    fclose(LOG_FILE);
}
