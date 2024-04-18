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
    rlog_set_console_level(RLOG_FATAL);
    rlog_set_printfile(true);
    rlog_set_printline(true);

    // rlog_rolling_t roll1 = { FILE_NAME_DEBUG, 1500, 5 };
    // rlog_add_rolling(RXILOG_DEBUG, &roll1);

    // rlog_rolling_t roll2;
    // // roll2 = { FILE_NAME_FATAL, 1500, 3 }; // compile error
    // roll2 = (rlog_rolling_t) { FILE_NAME_FATAL, 1500, 3 }; // this is compoud literals
    // rlog_add_rolling(RXILOG_FATAL, &roll2);

    // use compoud literals instead of '&object-of-struct', since C99
    rlog_add_rolling(RLOG_DEBUG,
                     &(rlog_rolling_t) {
                         FILE_NAME_DEBUG,
                         1500,
                         5,
                     }); // trailing comma with initilizer list
    rlog_add_rolling(RLOG_FATAL, &(rlog_rolling_t) { FILE_NAME_FATAL, 1500, 3 });

    while (1)
    {
        rlog_trace("number %d", 1);
        rlog_debug("number %d", 1);
        rlog_info("number %d", 1);
        rlog_warn("number %d", 1);
        rlog_error("number %d", 1);
        rlog_fatal("number %d", 1);

        usleep(500 * 1000);
    }
}
