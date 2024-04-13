#include "../src/log.h"

int main()
{
    log_trace("number %d", 1);
    log_debug("number %d", 1);
    log_info("number %d", 1);
    log_warn("number %d", 1);
    log_error("number %d", 1);
    log_fatal("number %d", 1);
}