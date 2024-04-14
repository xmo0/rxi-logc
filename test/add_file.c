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

    rxilog_file_t file_debug = { FILE_NAME_DEBUG, false, 0, 0 };
    rxilog_add_file(&file_debug, RXILOG_DEBUG);

    rxilog_file_t file_warn = { FILE_NAME_WARN, false, 0, 0 };
    rxilog_add_file(&file_warn, RXILOG_WARN);

    rxilog_trace("number %d", 1);
    rxilog_debug("number %d", 1);
    rxilog_info("number %d", 1);
    rxilog_warn("number %d", 1);
    rxilog_error("number %d", 1);
    rxilog_fatal("number %d", 1);
}
