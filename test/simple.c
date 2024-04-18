#include "../src/log.h"

int main()
{
    rlog_set_console_level(RLOG_TRACE);

    rlog_trace("number %d", 1);
    rlog_debug("number %d", 1);
    rlog_info("number %d", 1);
    rlog_warn("number %d", 1);
    rlog_error("number %d", 1);
    rlog_fatal("number %d", 1);
}