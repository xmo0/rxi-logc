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
} rxilog_rolling;

typedef struct
{
    FILE          *filp; // in single file case, use FILE *
    rxilog_rolling roll; // in case of log-file exceeds the max length of file
    struct tm     *time;
    int            level;
    const char    *file;
    int            line;
    const char    *fmt;
    va_list        ap;
} rxilog_Event;

enum
{
    RXILOG_FATAL, // this log level cannot be muted by log_set_xxx_level()
    RXILOG_ERROR,
    RXILOG_WARN,
    RXILOG_INFO,
    RXILOG_DEBUG,
    RXILOG_TRACE
};

typedef void (*rxilog_LogFn)(rxilog_Event *ev);
typedef void (*rxilog_LockFn)(bool lock, void *udata);

#define rxilog_trace(...) rxilog_log(RXILOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define rxilog_debug(...) rxilog_log(RXILOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define rxilog_info(...)  rxilog_log(RXILOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define rxilog_warn(...)  rxilog_log(RXILOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define rxilog_error(...) rxilog_log(RXILOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define rxilog_fatal(...) rxilog_log(RXILOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char *rxilog_level_string(int level);
void        rxilog_set_lock(rxilog_LockFn lockFn, void *lockData);
void        rxilog_set_quiet(bool enable);
void        rxilog_set_console_level(int level);
int         rxilog_add_callback(rxilog_LogFn fn, FILE *filp, int level);
int         rxilog_add_fp(FILE *fp, int level);
int         rxilog_add_rolling(rxilog_rolling roll, int level);

void rxilog_log(int level, const char *file, int line, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
