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
    rlog_set_console_level(RLOG_TRACE);

    rlog_add_file(RLOG_DEBUG, FILE_NAME_DEBUG);
    rlog_add_file(RLOG_WARN, FILE_NAME_WARN);

    rlog_trace("number %d", 1);
    rlog_debug("number %d", 1);
    rlog_info("number %d", 1);
    rlog_warn("number %d", 1);
    rlog_error("number %d", 1);
    rlog_fatal("number %d", 1);
}
