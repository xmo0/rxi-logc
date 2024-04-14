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
    log_set_console_level(LOGC_TRACE);

    FILE *filp_debug = fopen(FILE_NAME_DEBUG, "a");
    if (!filp_debug)
    {
        perror("fopen fail\n");
        return -1;
    }
    fwrite(string, sizeof(char), strlen(string), filp_debug);
    log_add_fp(filp_debug, LOGC_DEBUG);

    FILE *filp_warn = fopen(FILE_NAME_WARN, "a");
    if (!filp_warn)
    {
        perror("fopen fail\n");
        return -1;
    }
    fwrite(string, sizeof(char), strlen(string), filp_warn);
    log_add_fp(filp_warn, LOGC_WARN);

    log_trace("number %d", 1);
    log_debug("number %d", 1);
    log_info("number %d", 1);
    log_warn("number %d", 1);
    log_error("number %d", 1);
    log_fatal("number %d", 1);
}
