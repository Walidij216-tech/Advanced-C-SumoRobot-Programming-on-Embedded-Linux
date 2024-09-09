#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/app/timer.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/drivers/millis.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/defines.h"

#define TIMER_CLEARED (0u)

void timer_start(timer_t *timer, uint32_t timeout_ms)
{
    *timer = millis() + timeout_ms;
}

bool timer_timeout(const timer_t *timer)
{
    if (*timer == TIMER_CLEARED) {
        return false;
    }
    return millis() > *timer;
}

void timer_clear(timer_t *timer)
{
    *timer = TIMER_CLEARED;
}
