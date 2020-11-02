#ifndef CIRCLE_COMPONENT_H_
#define CIRCLE_COMPONENT_H_

#include "RenderableComponent.h"
#include "Transforms.h"
#include "Renderer.h"
#include <cassert>

class CircleComponent : public RenderableComponent
{
    public:
        /* TODO: Const ptr to obj ? */
        CircleComponent(const glm::vec4& color) :
            RenderableComponent(color) {}
        /* TODO: Template this? */
        void render() {
            CircleTransform *transform = dynamic_cast<CircleTransform*>(m_parent->getTransform());
            /* Transform of parent must be a circle transform! */
            assert(transform != nullptr);
            Renderer::drawCircle(transform->position, transform->radius, m_color);
        }
};

#endif /* CIRCLE_COMPONENT_H_ */