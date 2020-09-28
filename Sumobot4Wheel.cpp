#include "Sumobot4Wheel.h"
#include "WheelMotor.h"
#include <iostream>

// TODO: Move to separate file?, should I ever use defs in c++?
// TODO: pragma once?
// TODO: Same here, need "" not <>
#include "Constants.h"

// TODO: Proper cleanup of bodies and fixtures

using namespace constants;

// TODO: Initializer list like this?
Sumobot4Wheel::Sumobot4Wheel(b2World* world, float width, float length, float mass)
    : m_width(width)
    , m_length(length)
    , m_mass(mass)
{
    createBody(world);
    createFrictionBody(world);
    createWheels(world);
}

void Sumobot4Wheel::createFrictionBody(b2World* world)
{
    b2BodyDef frictionBodyDef;
    m_frictionBody = world->CreateBody(&frictionBodyDef);
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    // Since the fixture is just an "invisible" sensor the
    // exact shape doesn't matter, but it still needs a shape.
    b2CircleShape circleShape;
    circleShape.m_radius = 1.0f;
    fixtureDef.shape = &circleShape;
    // TODO: Keep pointer to fixture?
    m_frictionBody->CreateFixture(&fixtureDef);
}

// TODO: What order to place private and public functiosn in?
//       Any name difference between the two?
WheelMotor::Drive Sumobot4Wheel::driveToWheelDrive(Drive drive) {
    switch (drive) {
    case Drive::Forward:
        return WheelMotor::Drive::Forward;
    case Drive::Backward:
        return WheelMotor::Drive::Backward;
    default:
        return WheelMotor::Drive::None;
    }
}

WheelMotor::Drive Sumobot4Wheel::motionToWheelDrive(Drive drive, Turn turn, WheelMotor::Type wheelType)
{
    if (drive == Drive::None) {
        return WheelMotor::Drive::None;
    }

    // TODO: Place this before or after the top if statement?
    Drive oppositeDrive = Drive::None;

    switch(drive) {
        case Drive::Forward:
            oppositeDrive = Drive::Backward;
            break;
        case Drive::Backward:
            oppositeDrive = Drive::Forward;
            break;
        default:
            oppositeDrive = Drive::None;
    }

    switch(turn)
    {
        case Turn::Left:
            switch(wheelType)
            {
                case WheelMotor::Type::Left: return driveToWheelDrive(oppositeDrive);
                case WheelMotor::Type::Right: return driveToWheelDrive(drive);
            }
        case Turn::Right:
            switch(wheelType)
            {
                case WheelMotor::Type::Left: return driveToWheelDrive(drive);
                case WheelMotor::Type::Right: return driveToWheelDrive(oppositeDrive);
            }
        default:
            return driveToWheelDrive(drive);
    }
}

void Sumobot4Wheel::update(Drive drive, Turn turn) {
    for (auto &wheel : m_wheels) {
        wheel->updateFriction(motionToWheelDrive(drive, turn, wheel->getType()));
    }
    for (auto &wheel : m_wheels) {
        // TODO: Currently just applying 6 V
        wheel->updateDrive(motionToWheelDrive(drive, turn, wheel->getType()), 6.0f);
    }

    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    float currentSpeed = b2Dot(currentForwardNormal, m_body->GetLinearVelocity());
    float angularDamping = 0.3f; // Tweaked
    m_body->ApplyAngularImpulse(angularDamping * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);
}

// TODO: Define m_width... or pass them as parameters?
void Sumobot4Wheel::createBody(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);
    //m_body->SetAngularDamping(10000); //TODO: Add angular friction like this or some other way?

    const float scaledWidth = m_width * lengthScaleFactor * m_widthBodyFactor;
    const float scaledLength = m_length * lengthScaleFactor;
    const float scaledMass = m_mass * massScaleFactor * m_massBodyFactor;
    const float density = scaledMass / (scaledWidth * scaledLength);

    b2Vec2 vertices[4];
    vertices[0].Set(scaledWidth / 2, scaledLength / 2);
    vertices[1].Set(scaledWidth / 2, -scaledLength / 2);
    vertices[2].Set(-scaledWidth / 2, -scaledLength / 2);
    vertices[3].Set(-scaledWidth / 2, scaledLength / 2);
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 4);

    m_body->CreateFixture(&polygonShape, density);
}

void Sumobot4Wheel::addFriction(b2World* world, b2Body* body, float maxFrictionForce)
{
    b2FrictionJointDef jointDef;
    jointDef.bodyA = m_frictionBody;
    jointDef.bodyB = body;
    // TODO: This should scale according to how big the normal force
    // from the ground is (i.e. how heavy the vehicle is)
    jointDef.maxForce = maxFrictionForce;
    world->CreateJoint(&jointDef);
}

void Sumobot4Wheel::createWheels(b2World* world)
{
    // TODO: Scale the member variables at initialization instead?
    const float scaledWheelWidth = (m_width * lengthScaleFactor * m_widthWheelsFactor) / 2.0f;
    const float scaledWheelRadius = ((m_length / 2.0f) * lengthScaleFactor) / 4.0f;
    const float scaledWheelWeight = (m_mass * massScaleFactor * m_massWheelsFactor) / m_numWheels;
    const float scaledBodyWidth = m_width * lengthScaleFactor * m_widthBodyFactor;
    const float scaledBodyLength = m_length * lengthScaleFactor;
    const float normalForce = 9.82 * m_mass * forceScaleFactor;
    const float maxFrictionForcePerWheel = normalForce * m_frictionCoefficient / m_numWheels;

    const float frontRightX = (scaledBodyWidth / 2.0f) + (scaledWheelRadius / 2.0f);
    const float frontRightY = scaledBodyLength / 4.0f;
    const float frontLeftX = -frontRightX;
    const float frontLeftY = frontRightY;
    const float backRightX = frontRightX;
    const float backRightY = -frontRightY;
    const float backLeftX = frontLeftX;
    const float backLeftY = -frontLeftY;

    // Prepare common joint parameters
    // TODO: Use a weld joint here instead
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero(); //center of wheel

    // TODO: Create new fucntion and pass params as struct?

    // TODO: When to use AUTO?
    // TODO: When to use new?
    WheelMotor* backLeftWheel = new WheelMotor(world, WheelMotor::Type::Left, scaledWheelWidth,
                                               scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = backLeftWheel->getBody();
    jointDef.localAnchorA.Set(backLeftX, backLeftY);
    world->CreateJoint(&jointDef);
    m_wheels.push_back(backLeftWheel);

    WheelMotor* backRightWheel = new WheelMotor(world, WheelMotor::Type::Right, scaledWheelWidth,
                                                scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = backRightWheel->getBody();
    jointDef.localAnchorA.Set(backRightX, backRightY);
    world->CreateJoint(&jointDef);
    m_wheels.push_back(backRightWheel);

    WheelMotor* frontLeftWheel = new WheelMotor(world, WheelMotor::Type::Left, scaledWheelWidth,
                                                scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = frontLeftWheel->getBody();
    jointDef.localAnchorA.Set(frontLeftX, frontLeftY);
    m_flJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(frontLeftWheel);

    WheelMotor* frontRightWheel = new WheelMotor(world, WheelMotor::Type::Right, scaledWheelWidth,
                                                 scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = frontRightWheel->getBody();
    jointDef.localAnchorA.Set(frontRightX, frontRightY);
    m_frJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(frontRightWheel);

    for (auto &wheel : m_wheels) {
        addFriction(world, wheel->getBody(), maxFrictionForcePerWheel);
    }
}
