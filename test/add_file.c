/***************************************************************************************************
 * xmo0 24.04.14
 *
 * A sample for outputing two unique logs with different levels
 *
 * *************************************************************************************************
 */

#include "../src/log.h"

#define FILE_NAME_DEBUG "build/filelog_debug.txt"
#define FILE_NAME_WARN  "build/filelog_warn.txt"

int main()
{
    const char string[] = "--- Program start ---\n";
    rxilog_set_console_level(RXILOG_TRACE);

    rxilog_add_file(RXILOG_DEBUG, FILE_NAME_DEBUG);
    rxilog_add_file(RXILOG_WARN, FILE_NAME_WARN);

    rxilog_trace("number %d", 1);
    rxilog_debug("number %d", 1);
    rxilog_info("number %d", 1);
    rxilog_warn("number %d", 1);
    rxilog_error("number %d", 1);
    rxilog_fatal("number %d", 1);
}
