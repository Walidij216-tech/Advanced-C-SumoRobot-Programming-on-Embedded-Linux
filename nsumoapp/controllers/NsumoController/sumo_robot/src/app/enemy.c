#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/app/enemy.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/drivers/vl53l0x.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/assert_handler.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/trace.h"

#define RANGE_DETECT_THRESHOLD (600u) // mm
#define INVALID_RANGE (UINT16_MAX)
#define RANGE_CLOSE (100u) // mm
#define RANGE_MID (200u) // mm
#define RANGE_FAR (300u) // mm

struct enemy enemy_get(void)
{
    struct enemy enemy = { ENEMY_POS_NONE, ENEMY_RANGE_NONE };
    vl53l0x_ranges_t ranges;
    bool fresh_values = false;
    vl53l0x_result_e result = vl53l0x_read_range_multiple(ranges, &fresh_values);
    if (result) {
        TRACE("read range failed %u", result);
        return enemy;
    }

    const uint16_t range_front = ranges[VL53L0X_IDX_FRONT];
    const uint16_t range_front_left = ranges[VL53L0X_IDX_FRONT_LEFT];
    const uint16_t range_front_right = ranges[VL53L0X_IDX_FRONT_RIGHT];
#if 0 // Skip left and right (badly mounted on the robot)
    const uint16_t range_left = ranges[VL53L0X_IDX_LEFT];
    const uint16_t range_right = ranges[VL53L0X_IDX_RIGHT];
#endif

    const bool front = range_front < RANGE_DETECT_THRESHOLD;
    const bool front_left = range_front_left < RANGE_DETECT_THRESHOLD;
    const bool front_right = range_front_right < RANGE_DETECT_THRESHOLD;
#if 0 // Skip left and right (badly mounted on the robot)
    const bool left = range_left < RANGE_DETECT_THRESHOLD;
    const bool right = range_right < RANGE_DETECT_THRESHOLD;
#endif

    uint16_t range = INVALID_RANGE;
#if 0 // Skip left and right (badly mounted on the robot)
    if (left) {
        if (front_right || right) {
            enemy.position = ENEMY_POS_IMPOSSIBLE;
        } else {
            enemy.position = ENEMY_POS_LEFT;
            range = range_left;
        }
    } else if (right) {
        if (front_left || left) {
            enemy.position = ENEMY_POS_IMPOSSIBLE;
        } else {
            enemy.position = ENEMY_POS_RIGHT;
            range = range_right;
        }
    }
#endif

    if (front_left && front && front_right) {
        enemy.position = ENEMY_POS_FRONT_ALL;
        // Average
        range = ((((range_front_left + range_front) / 2) + range_front_right) / 2);
    } else if (front_left && front_right) {
        enemy.position = ENEMY_POS_IMPOSSIBLE;
    } else if (front_left) {
        if (front) {
            enemy.position = ENEMY_POS_FRONT_AND_FRONT_LEFT;
            // Average
            range = (range_front_left + range_front) / 2;
        } else {
            enemy.position = ENEMY_POS_FRONT_LEFT;
            range = range_front_left;
        }
    } else if (front_right) {
        if (front) {
            enemy.position = ENEMY_POS_FRONT_AND_FRONT_RIGHT;
            // Average
            range = (range_front_right + range_front) / 2;
        } else {
            enemy.position = ENEMY_POS_FRONT_RIGHT;
            range = range_front_right;
        }
    } else if (front) {
        enemy.position = ENEMY_POS_FRONT;
        range = range_front;
    } else {
        enemy.position = ENEMY_POS_NONE;
    }

    if (range == INVALID_RANGE) {
        return enemy;
    }

    if (range < RANGE_CLOSE) {
        enemy.range = ENEMY_RANGE_CLOSE;
    } else if (range < RANGE_MID) {
        enemy.range = ENEMY_RANGE_MID;
    } else {
        enemy.range = ENEMY_RANGE_FAR;
    }

    return enemy;
}

bool enemy_detected(const struct enemy *enemy)
{
    return enemy->position != ENEMY_POS_NONE && enemy->position != ENEMY_POS_IMPOSSIBLE;
}

bool enemy_at_left(const struct enemy *enemy)
{
    return enemy->position == ENEMY_POS_LEFT || enemy->position == ENEMY_POS_FRONT_LEFT
        || enemy->position == ENEMY_POS_FRONT_AND_FRONT_LEFT;
}

bool enemy_at_right(const struct enemy *enemy)
{
    return enemy->position == ENEMY_POS_RIGHT || enemy->position == ENEMY_POS_FRONT_RIGHT
        || enemy->position == ENEMY_POS_FRONT_AND_FRONT_RIGHT;
}

bool enemy_at_front(const struct enemy *enemy)
{
    return enemy->position == ENEMY_POS_FRONT || enemy->position == ENEMY_POS_FRONT_ALL;
}

static bool initialized = false;
void enemy_init(void)
{
    ASSERT(!initialized);
    vl53l0x_result_e result = vl53l0x_init();
    if (result) {
        TRACE("Failed to initialize vl53l0x %u", result);
        return;
    }
    initialized = true;
}
