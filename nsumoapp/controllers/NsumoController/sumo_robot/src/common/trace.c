#ifndef DISABLE_TRACE
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/trace.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/assert_handler.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/drivers/uart.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/external/printf/printf.h"
#include <stdbool.h>

static bool initialized = false;
void trace_init(void)
{
    ASSERT(!initialized);
    uart_init();
    initialized = true;
}

void trace(const char *format, ...)
{
    ASSERT(initialized);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

#endif // DISABLE_TRACE
