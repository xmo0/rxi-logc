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

    FILE *filp_debug = fopen(FILE_NAME_DEBUG, "a");
    if (!filp_debug)
    {
        perror("fopen fail\n");
        return -1;
    }
    fwrite(string, sizeof(char), strlen(string), filp_debug);
    rxilog_add_fp(filp_debug, RXILOG_DEBUG);

    FILE *filp_warn = fopen(FILE_NAME_WARN, "a");
    if (!filp_warn)
    {
        perror("fopen fail\n");
        return -1;
    }
    fwrite(string, sizeof(char), strlen(string), filp_warn);
    rxilog_add_fp(filp_warn, RXILOG_WARN);

    rxilog_trace("number %d", 1);
    rxilog_debug("number %d", 1);
    rxilog_info("number %d", 1);
    rxilog_warn("number %d", 1);
    rxilog_error("number %d", 1);
    rxilog_fatal("number %d", 1);
}
