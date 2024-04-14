/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define LOG_VERSION "0.2.0"

typedef struct
{
    char        *file_name;
    off_t        max_log_size;
    unsigned int max_logs;
} rolling_appender;

typedef struct
{
    FILE            *filp; // the original repo use type (void *) here, why?
    struct tm       *time;
    int              level;
    const char      *file;
    int              line;
    const char      *fmt;
    va_list          ap;
    rolling_appender ra;
} log_Event;

enum
{
    LOGC_FATAL, // this log level cannot be muted by log_set_xxx_level()
    LOGC_ERROR,
    LOGC_WARN,
    LOGC_INFO,
    LOGC_DEBUG,
    LOGC_TRACE
};

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

#define log_trace(...) log_log(LOGC_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOGC_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOGC_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOGC_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOGC_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOGC_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char *log_level_string(int level);
void        log_set_lock(log_LockFn fn, void *udata);
void        log_set_console_level(int level);
void        log_set_file_level(int level);
void        log_set_quiet(bool enable);
int         log_add_callback(log_LogFn fn, FILE *filp, int level);
int         log_add_rolling_appender(rolling_appender ra, int level);
int         log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
