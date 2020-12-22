#include "components/RangeSensor.h"
#include "components/Body2D.h"
#include "components/Transforms.h"

#include <box2d/box2d.h>

namespace {
    const float drawWidth = 0.01f;
}

RangeSensor::RangeSensor(const PhysicsWorld &world, LineTransform *transform,
                         const glm::vec2 &unscaledPosition, float angle, float minDistance, float maxDistance) :
    PhysicsComponent(world),
    m_lineTransform(transform),
    m_relativeAngle(angle),
    m_minDistance(PhysicsWorld::scalePosition(minDistance)),
    m_maxDistance(PhysicsWorld::scaleLengthNoAssert(maxDistance)),
    m_detectedDistance(m_maxDistance)
{
    if (m_lineTransform) {
        m_lineTransform->width = drawWidth;
    }

    /* Create tiny body for attaching and keeping track of ray cast start position */
    const Body2D::Specification bodySpec { true, false, 0.001f };
    m_body2D = std::make_unique<Body2D>(world, unscaledPosition, 0.0f, 0.0005f, bodySpec);
}

RangeSensor::~RangeSensor()
{
}

Body2D *RangeSensor::getBody() const
{
    return m_body2D.get();
}

void RangeSensor::onFixedUpdate(double stepTime)
{
    const float rayAngleStart = -m_body2D->getAngle();
    const float rayAngleEnd = m_relativeAngle - m_body2D->getAngle();
    const glm::vec2 bodyPosition = m_body2D->getPosition();

    /* Recalculate the casted ray */
    const float sinAngle = sinf(-rayAngleStart);
    const float cosAngle = cosf(-rayAngleStart);
    const glm::vec2 rayStartPosition = bodyPosition;
    const glm::vec2 rayEndPosition = rayStartPosition + glm::vec2{sinf(rayAngleEnd), cosf(rayAngleEnd)} * m_maxDistance;
    updateDetectedDistance(rayStartPosition, rayEndPosition);
    const glm::vec2 detectedEndPosition = rayStartPosition + glm::vec2{sinf(rayAngleEnd), cosf(rayAngleEnd)} * m_detectedDistance;

    if (m_lineTransform) {
        m_lineTransform->start = { rayStartPosition.x, rayStartPosition.y };
        m_lineTransform->end = { detectedEndPosition.x, detectedEndPosition.y };
    }
    updateVoltage();
}

void RangeSensor::updateDetectedDistance(const glm::vec2 &start, const glm::vec2 &end)
{
    b2RayCastInput rayInput;
    b2RayCastOutput rayOutput;
    rayInput.p1.x = start.x;
    rayInput.p1.y = start.y;
    rayInput.p2.x = end.x;
    rayInput.p2.y = end.y;
    rayInput.maxFraction = 1.0f;

    float closestFraction = rayInput.maxFraction;
    b2Vec2 intersectionNormal(0,0);
    for (b2Body *b = m_world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetFixtureList() && b->GetFixtureList()[0].IsSensor()) {
            /* Don't detect non-collidable objects */
            continue;
        }
        for (b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext()) {
            if (!f->RayCast(&rayOutput, rayInput, 1)) {
                continue;
            }
            if (rayOutput.fraction < closestFraction) {
                closestFraction = rayOutput.fraction;
                intersectionNormal = rayOutput.normal;
            }
        }
    }

    m_detectedDistance = closestFraction * m_maxDistance;
}

float RangeSensor::getDistance() const
{
    return m_detectedDistance;
}

void RangeSensor::updateVoltage()
{
    m_distanceVoltage = m_detectedDistance / m_maxDistance;
}

float *RangeSensor::getVoltageLine()
{
    return &m_distanceVoltage;
}
