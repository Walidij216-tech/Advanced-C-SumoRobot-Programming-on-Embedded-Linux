#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/app/state_wait.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/assert_handler.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/defines.h"

// No blocking code (e.g. busy wait) allowed in this function
void state_wait_enter(struct state_wait_data *data, state_e from, state_event_e event)
{
    UNUSED(data);
    UNUSED(event);
    ASSERT(from == STATE_WAIT);
    // Command triggers transition
    // Note in actual sumobot competition this signal would come from another IR transceiver
    // than the one used here.
}
