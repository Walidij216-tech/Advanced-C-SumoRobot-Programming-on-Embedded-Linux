#ifndef STATE_MANUAL_H
#define STATE_MANUAL_H

#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/app/state_common.h"

// Manual control with IR remote

struct state_manual_data
{
    const struct state_common_data *common;
};

void state_manual_enter(struct state_manual_data *data, state_e from, state_event_e event);

#endif // STATE_MANUAL_H
