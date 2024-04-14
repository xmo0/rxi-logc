/***************************************************************************************************
 * xmo0 24.04.14
 *
 * A sample for logs rolling feature.
 *
 * *************************************************************************************************
 */

#include "../src/log.h"

#define FILE_NAME "build/rolling-log"

int main()
{
    rxilog_set_console_level(RXILOG_FATAL);

    rxilog_file_t roll = { FILE_NAME, true, 1500, 5 };
    rxilog_add_file(&roll, RXILOG_DEBUG);

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
