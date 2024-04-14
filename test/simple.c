#include "../src/log.h"

int main()
{
    rxilog_set_console_level(RXILOG_TRACE);

    rxilog_trace("number %d", 1);
    rxilog_debug("number %d", 1);
    rxilog_info("number %d", 1);
    rxilog_warn("number %d", 1);
    rxilog_error("number %d", 1);
    rxilog_fatal("number %d", 1);
}