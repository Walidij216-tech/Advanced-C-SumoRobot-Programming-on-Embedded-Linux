#ifndef INPUT_HISTORY
#define INPUT_HISTORY

#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/app/enemy.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/app/line.h"

struct ring_buffer;

struct input
{
    struct enemy enemy;
    line_e line;
};

void input_history_save(struct ring_buffer *history, const struct input *input);
struct enemy input_history_last_directed_enemy(const struct ring_buffer *history);

#endif // INPUT_HISTORY
