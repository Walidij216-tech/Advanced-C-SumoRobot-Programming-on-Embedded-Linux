#ifndef RANGE_SENSOR_OBJECT_H_
#define RANGE_SENSOR_OBJECT_H_

#include "SceneObject.h"
#include "PhysicsWorld.h"

class RangeSensor;
class Body2D;

class RangeSensorObject : public SceneObject
{
public:
    struct Specification {
        const glm::vec2 relativePosition;
        const float relativeAngle = 0.0f;
        const float minDistance = 0.0f;
        const float maxDistance = 0.0f;
    };
    RangeSensorObject(Scene *scene, const PhysicsWorld &world, const Body2D &parentBody, bool show, const Specification &unscaledSpec);
    ~RangeSensorObject();
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine() const;

private:
    RangeSensor *m_rangeSensor = nullptr;
};

#endif /* RANGE_SENSOR_OBJECT_H_ */
