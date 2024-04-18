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

    FILE *filp_debug = fopen(FILE_NAME_DEBUG, "a");
    if (!filp_debug)
    {
        perror("fopen fail\n");
        return -1;
    }
    fwrite(string, sizeof(char), strlen(string), filp_debug);
    rlog_add_filp(RLOG_DEBUG, filp_debug);

    FILE *filp_warn = fopen(FILE_NAME_WARN, "a");
    if (!filp_warn)
    {
        perror("fopen fail\n");
        return -1;
    }
    fwrite(string, sizeof(char), strlen(string), filp_warn);
    rlog_add_filp(RLOG_WARN, filp_warn);

    rlog_trace("number %d", 1);
    rlog_debug("number %d", 1);
    rlog_info("number %d", 1);
    rlog_warn("number %d", 1);
    rlog_error("number %d", 1);
    rlog_fatal("number %d", 1);
}
