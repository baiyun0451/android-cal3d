#ifndef SPRITE_H
#define SPRITE_H

#include "Shape.h"
class Sprite : public Shape
{
    public:
        Sprite();
        Sprite(float pos [], float size [], float color[]);
        Sprite(float pos [], float size [], int textureID);
        void setColor(float color[4]);
        void setPosition(float x, float y);
        void setSize(float x, float y);
        float * getTextureCoord();
    private:
        float mSize   [2];
};

#endif
