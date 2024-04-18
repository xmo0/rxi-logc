/*
 * Copyright (c) 2020 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <unistd.h>   // for usleep(), used in multi-thread test
#include <sys/time.h> // struct timeval
#include "log.h"

#define MAX_CALLBACKS 32
// #define MULTI_THREAD_SAFETY_TEST

typedef struct {
    rlog_LogFn   fn;
    int          level;
    FILE        *filp;
    char        *filename;
    off_t        max_log_size;
    unsigned int max_logs;
} Callback;

struct s_rlog_Event {
    FILE       *filp; // use with console logs
    Callback   *cb;   // use with file logs
    struct tm  *time;
    int         level;
    const char *src_file;
    int         src_line;
    const char *fmt;
    va_list     ap;
};

static struct {
    rlog_LockFn lockFn;
    void       *lockData;
    bool        quiet;
    bool        printfile; // print __FILE__
    bool        printline; // print __LINE__
    int         console_level;
    Callback    callbacks[MAX_CALLBACKS];
} L;

static const char *level_strings[] = { "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE" };

#ifdef LOG_USE_COLOR
static const char *level_colors[]
    = { "\x1b[35m", "\x1b[31m", "\x1b[33m", "\x1b[32m", "\x1b[36m", "\x1b[94m" };
#endif

static int get_next_available_callback()
{
    for (int i = 0; i < MAX_CALLBACKS; i++)
    {
        if (!L.callbacks[i].fn)
        {
            return i;
        }
    }
    return -1;
}

static int check_filp_valid(Callback *cb)
{
    if (cb->filp == NULL)
    {
        cb->filp = fopen(cb->filename, "a");
        if (cb->filp == NULL)
        {
            char *msg = (char *)calloc(strlen(cb->filename) + 1024, sizeof(char));
            sprintf(msg, "Unable to open log file: %s", cb->filename);
            perror(msg);
            free(msg);
            return -1;
        }
        else
        {
            fprintf(cb->filp, "--- Log file opened ---\n");
        }
    }
    return 0;
}

static void console_callback(rlog_Event *ev)
{
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
    fprintf(ev->filp, "%s %s%-5s\x1b[0m ", buf, level_colors[ev->level], level_strings[ev->level]);

    if (L.printfile)
    {
        fprintf(ev->filp, "\x1b[90m%s:\x1b[0m ", ev->src_file);
    }
    if (L.printline)
    {
        fprintf(ev->filp, "\x1b[90m%.4d:\x1b[0m ", ev->src_line);
    }
#else
    fprintf(ev->filp, "%s %-5s ", buf, level_strings[ev->level]);

    if (L.printfile)
    {
        fprintf(ev->filp, "%s: ", ev->src_file);
    }
    if (L.printline)
    {
        fprintf(ev->filp, "%.4d: ", ev->src_line);
    }
#endif
#ifdef MULTI_THREAD_SAFETY_TEST
    usleep(1);
#endif
    vfprintf(ev->filp, ev->fmt, ev->ap);
#ifdef MULTI_THREAD_SAFETY_TEST
    usleep(1);
#endif
    fprintf(ev->filp, "\n");
    fflush(ev->filp);
}

static void file_callback(rlog_Event *ev)
{
    check_filp_valid(ev->cb);
    FILE *filp = ev->cb->filp;
    char  buf[64];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    fprintf(filp, "%s %-5s ", buf, level_strings[ev->level]);
    if (L.printfile)
    {
        fprintf(filp, "%s: ", ev->src_file);
    }
    if (L.printline)
    {
        fprintf(filp, "%.4d: ", ev->src_line);
    }
    vfprintf(filp, ev->fmt, ev->ap);
    fprintf(filp, "\n");
    fflush(filp);
}

static void rolling_appender_callback(rlog_Event *ev)
{
    Callback *cb  = ev->cb;
    char     *msg = (char *)calloc(strlen(cb->filename) + 1024, sizeof(char));

    check_filp_valid(cb);
    file_callback(ev);

    struct stat buf;
    if (stat(cb->filename, &buf) < 0)
    {
        sprintf(msg, "Unable to stat log file: %s", cb->filename);
        perror(msg);
        free(msg);
        return;
    }

    free(msg);

    if (buf.st_size >= cb->max_log_size)
    {
        char *old = (char *)calloc(strlen(cb->filename) + 10, sizeof(char));
        char *new = (char *)calloc(strlen(cb->filename) + 10, sizeof(char));
        fclose(cb->filp);
        cb->filp = NULL;
        for (unsigned int i = cb->max_logs - 1; i >= 1; i--)
        {
            sprintf(old, "%s.%u", cb->filename, i);
            sprintf(new, "%s.%u", cb->filename, i + 1);
            rename(old, new);
        }
        sprintf(new, "%s.1", cb->filename);
        rename(cb->filename, new);

        free(old);
        free(new);
    }
}

static void lock(void)
{
    if (L.lockFn)
    {
        L.lockFn(true, L.lockData);
    }
}

static void unlock(void)
{
    if (L.lockFn)
    {
        L.lockFn(false, L.lockData);
    }
}

static void set_ev_time(rlog_Event *ev)
{
    if (!ev->time)
    {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
}

const char *rlog_level_string(int level)
{
    return level_strings[level];
}

void rlog_set_lock(rlog_LockFn lockFn, void *lockData)
{
    L.lockFn   = lockFn;
    L.lockData = lockData;
}

void rlog_set_console_level(int level)
{
    L.console_level = level;
}

void rlog_set_quiet(bool enable)
{
    L.quiet = enable;
}

void rlog_set_printfile(bool enable)
{
    L.printfile = enable;
}

void rlog_set_printline(bool enable)
{
    L.printline = enable;
}

int rlog_add_filp(int level, FILE *filp)
{
    int nac = get_next_available_callback();
    if (nac < 0)
    {
        return nac;
    }

    L.callbacks[nac] = (Callback) { file_callback, level, filp };
    return 0;
}

int rlog_add_file(int level, char *filename)
{
    int nac = get_next_available_callback();
    if (nac < 0)
    {
        return nac;
    }

    L.callbacks[nac] = (Callback) { file_callback, level, NULL, filename };
    return 0;
}

int rlog_add_rolling(int level, rlog_rolling_t *roll)
{
    int nac = get_next_available_callback();
    if (nac < 0)
    {
        return nac;
    }

    L.callbacks[nac] = (Callback) { .fn           = rolling_appender_callback,
                                    .level        = level,
                                    .filp         = NULL,
                                    .filename     = roll->filename,
                                    .max_log_size = roll->max_log_size,
                                    .max_logs     = roll->max_logs };

    return 0;
}

void rlog_log(int level, const char *file, int line, const char *fmt, ...)
{
    if (L.quiet)
    {
        return;
    }

    rlog_Event ev = {
        .level    = level,
        .src_file = file,
        .src_line = line,
        .fmt      = fmt,
    };

    lock();

    if (level <= L.console_level)
    {
        ev.filp = stderr;

        set_ev_time(&ev);
        va_start(ev.ap, fmt);
        console_callback(&ev);
        va_end(ev.ap);
    }

    for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++)
    {
        Callback *cb = &L.callbacks[i];
        if (level <= cb->level)
        {
            ev.cb = cb;

            set_ev_time(&ev);
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}

// currently not used
char time_string[25];
/* Write time to buf in format YYYY-MM-DD HH:MM:SS.ms */
static void time_to_str(char *buf)
{
    static struct timeval tv;
    static struct tm     *tm;

    static int year;
    static int month;
    static int day;
    static int hour;
    static int minutes;
    static int seconds;
    static int msec;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    /* Add 1900 to get the right year value read the manual page for localtime() */
    year = tm->tm_year + 1900;
    /* Months are 0 indexed in struct tm */
    month   = tm->tm_mon + 1;
    day     = tm->tm_mday;
    hour    = tm->tm_hour;
    minutes = tm->tm_min;
    seconds = tm->tm_sec;
    msec    = (int)(tv.tv_usec / 1000);
    // buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
    int len = sprintf(
        buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d ", year, month, day, hour, minutes, seconds, msec);
    buf[len] = '\0';
    printf("Time string length is : %d", len);
}
