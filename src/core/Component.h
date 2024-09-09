#ifndef COMPONENT_H_
#define COMPONENT_H_

/**
 * The base class all implemented components indirectly inherit from.
 */
class Component
{
public:
    virtual ~Component() { }
    /**
     * Called every physics step (if assigned to a Scene Object).
     */
    virtual void onFixedUpdate1(float stepTime) {
        (void)stepTime; // Prevent unused parameter warning
    }
    virtual void onFixedUpdate() {
         // Prevent unused parameter warning
    }
 

};

#endif /* COMPONENT_H_ */
