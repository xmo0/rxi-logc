/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef RLOG_H
#define RLOG_H

#ifdef __cplusplus
extern "C" {
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

// only the structure of rlog_rolling_t is exposed outside
// other struct(s) shall be used inside
typedef struct {
    char        *filename;
    off_t        max_log_size;
    unsigned int max_logs;
} rlog_rolling_t;

typedef struct s_rlog_Event rlog_Event;

enum {
    RLOG_FATAL, // this log level cannot be muted by log_set_xxx_level()
    RLOG_ERROR,
    RLOG_WARN,
    RLOG_INFO,
    RLOG_DEBUG,
    RLOG_TRACE,
};

typedef void (*rlog_LogFn)(rlog_Event *ev);
typedef void (*rlog_LockFn)(bool lock, void *udata);

#define rlog_trace(...) rlog_log(RLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define rlog_debug(...) rlog_log(RLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define rlog_info(...)  rlog_log(RLOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define rlog_warn(...)  rlog_log(RLOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define rlog_error(...) rlog_log(RLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define rlog_fatal(...) rlog_log(RLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void rlog_log(int level, const char *file, int line, const char *fmt, ...);

const char *rlog_level_string(int level);
void        rlog_set_lock(rlog_LockFn lockFn, void *lockData);
void        rlog_set_quiet(bool enable);
void        rlog_set_printfile(bool enable);
void        rlog_set_printline(bool enable);
void        rlog_set_console_level(int level);
int         rlog_add_filp(int level, FILE *filp);
int         rlog_add_file(int level, char *filename);
int         rlog_add_rolling(int level, rlog_rolling_t *roll);

#ifdef __cplusplus
}
#endif

#endif
