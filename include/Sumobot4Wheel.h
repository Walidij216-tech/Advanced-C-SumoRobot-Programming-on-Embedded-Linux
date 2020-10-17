#ifndef SUMOBOT_H_
#define SUMOBOT_H_

#include <vector>

// TODO: Can't use <> why?
// TODO: Forward declare?
#include "WheelMotor.h"
#include "RangeSensor.h"
#include "LineDetector.h"

class Sumobot4Wheel
{
public:
    // TODO: Why enum class over regular enum?
    // TODO: How to name enums when there is an inner enum doing the same thing
    // TODO: All caps?
    enum class Drive {Forward, Backward, None};
    enum class Turn {Left, Right, None};

    Sumobot4Wheel(b2World* world, float width, float length, float mass);
    void update(Drive drive, Turn turn);
    b2Body* getBody() { return m_body; }

private:
    void createBody(b2World* world);
    // TODO: Save ptr to the world inside this object?
    void createWheels(b2World* world);
    void createFrictionBody(b2World* world);
    void createSensors(b2World* world);
    void addFriction(b2World* world, b2Body* body, float maxFrictionForce);
    void updateSensors(void);

    WheelMotor::Drive driveToWheelDrive(Drive drive);
    WheelMotor::Drive motionToWheelDrive(Drive drive, Turn turn, WheelMotor::Type wheelType);

    // TODO: Initialize these here?
    b2Body* m_body = nullptr;
    std::vector<WheelMotor*> m_wheels;
    b2RevoluteJoint* m_flJoint = nullptr;
    b2RevoluteJoint* m_frJoint = nullptr;
    b2Body* m_frictionBody = nullptr;
    RangeSensor* m_rangeSensorLeft = nullptr;
    RangeSensor* m_rangeSensorFrontLeft = nullptr;
    RangeSensor* m_rangeSensorFront = nullptr;
    RangeSensor* m_rangeSensorFrontRight = nullptr;
    RangeSensor* m_rangeSensorRight = nullptr;
    LineDetector* m_lineDetectorFrontLeft = nullptr;
    LineDetector* m_lineDetectorFrontRight = nullptr;
    LineDetector* m_lineDetectorBackLeft = nullptr;
    LineDetector* m_lineDetectorBackRight = nullptr;

    float m_width = 0;
    float m_length = 0;
    float m_mass = 0;
    int m_numWheels = 4;
    // TODO: Introduce height for density calculation?

    // TODO: Is it okay to use const here? I can't use constexpr
    const float m_massBodyFactor = 0.9f;
    const float m_massWheelsFactor = 1.0f - m_massBodyFactor;
    const float m_widthBodyFactor = 0.8f;
    const float m_widthWheelsFactor = 1.0f - m_widthBodyFactor;
    const float m_frictionCoefficient = 0.05f;
};

#endif /* SUMOBOT_H_ */