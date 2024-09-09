#ifndef RENDERABLE_COMPONENT_H_
#define RENDERABLE_COMPONENT_H_

#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/src/core/Component.h"
#include <glm/glm.hpp>

/**
 * Base class for components that renders.
 */
class RenderableComponent : public Component
{
    public:
        RenderableComponent() {}
        virtual ~RenderableComponent() {}
        /**
        * Called every simulation iteration (if assigned to a Scene Object).
        */
        virtual void onFixedUpdate1(float stepTime)=0;
        // Implementation of RenderableComponent's fixed update
        /**
         * Enable or disable rendering.
         */
        void setEnabled(float enabled)
        {
            m_enabled = enabled;
        }

    protected:
        bool m_enabled = true;
};

#endif /* RENDERABLE_COMPONENT_H_ */
