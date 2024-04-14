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
    log_set_console_level(LOGC_FATAL);

    log_rolling roll = { FILE_NAME, 1500, 5 };
    log_add_rolling(roll, LOGC_DEBUG);

    while (1)
    {
        log_trace("number %d", 1);
        log_debug("number %d", 1);
        log_info("number %d", 1);
        log_warn("number %d", 1);
        log_error("number %d", 1);
        log_fatal("number %d", 1);

        usleep(500 * 1000);
    }
}
