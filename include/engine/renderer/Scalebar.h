#ifndef SCALEBAR_H_
#define SCALEBAR_H_

#include "Texture.h"

class Scalebar
{
public:
    Scalebar();
    ~Scalebar();
    void render();
private:
    struct ScalebarRenderable
    {
        ScalebarRenderable(std::string textureName, float width, float height) :
            texture(textureName), width(width), height(height) {}
        Texture texture;
        float width;
        float height;
    };

    const ScalebarRenderable m_scalebar4m;
    const ScalebarRenderable m_scalebar2m;
    const ScalebarRenderable m_scalebar1m;
    const ScalebarRenderable m_scalebar50cm;
    const ScalebarRenderable m_scalebar25cm;
};

#endif /* SCALEBAR_H */
