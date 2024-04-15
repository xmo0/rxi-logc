/***************************************************************************************************
 * xmo0 24.04.14
 *
 * A sample for logs rolling feature.
 *
 * *************************************************************************************************
 */

#include "../src/log.h"

#define FILE_NAME_DEBUG "build/rolling-log-debug"
#define FILE_NAME_FATAL "build/rolling-log-fatal"

int main()
{
    rxilog_set_console_level(RXILOG_FATAL);

    rxilog_rolling_t roll1 = { FILE_NAME_DEBUG, 1500, 5 };
    rxilog_add_rolling(RXILOG_DEBUG, &roll1);

    rxilog_rolling_t roll2 = { FILE_NAME_FATAL, 1500, 3 };
    rxilog_add_rolling(RXILOG_FATAL, &roll2);

    while (1)
    {
        rxilog_trace("number %d", 1);
        rxilog_debug("number %d", 1);
        rxilog_info("number %d", 1);
        rxilog_warn("number %d", 1);
        rxilog_error("number %d", 1);
        rxilog_fatal("number %d", 1);

        usleep(500 * 1000);
    }
}
