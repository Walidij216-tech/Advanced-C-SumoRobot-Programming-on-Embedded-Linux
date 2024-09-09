#ifndef STATE_WAIT_H
#define STATE_WAIT_H

#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/app/state_common.h"

// Wait for start signal (remote command)

struct state_wait_data
{
    const struct state_common_data *common;
};

void state_wait_enter(struct state_wait_data *data, state_e from, state_event_e event);

#endif // STATE_WAIT_H
