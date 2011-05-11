
#include "Sprite.h"
#include <GLES/gl.h>
#include <GLES/glext.h>

#include "Texture.h"

Sprite::Sprite()
{
}
Sprite::Sprite(float pos [], float size [], int textureID)
{
    mColor = 0;

    mSize[0] = size[0];
    mSize[1] = size[1];

    mVertex = new float [3*4];
    mVertex[0] = pos[0];             mVertex[1] = pos[1];              mVertex[2] = 0;
    mVertex[3] = pos[0] + size[0];   mVertex[4] = pos[1];              mVertex[5] = 0;
    mVertex[6] = pos[0];             mVertex[7] = pos[1] + size[1];    mVertex[8] = 0;
    mVertex[9] = pos[0] + size[0];   mVertex[10] = pos[1] + size[1];   mVertex[11] = 0;

    mIndices = new unsigned short [3*2];
    mIndices[0] = 0; mIndices[1] = 1; mIndices[2] = 2;
    mIndices[3] = 1; mIndices[4] = 3; mIndices[5] = 2;

    mTextCoord = new float [2*4];
    mTextCoord[0] = 0; mTextCoord[1] = 1;
    mTextCoord[2] = 1; mTextCoord[3] = 1;
    mTextCoord[4] = 0; mTextCoord[5] = 0;
    mTextCoord[6] = 1; mTextCoord[7] = 0;

    mTextureID = textureID;
    mIndicesCount = 6;
}
Sprite::Sprite(float pos[],float size[], float color[])
{
    mTextCoord = 0;
    /*
   1 +----+ 2
     |   /|
     |  / |
     | /  |
     |/   |
   3 +----+ 4

    */
    mSize[0] = size[0];
    mSize[1] = size[1];

    mVertex = new float [3*4];
    mVertex[0] = pos[0];             mVertex[1] = pos[1];              mVertex[2] = 0;
    mVertex[3] = pos[0] + size[0];   mVertex[4] = pos[1];              mVertex[5] = 0;
    mVertex[6] = pos[0];             mVertex[7] = pos[1] + size[1];    mVertex[8] = 0;
    mVertex[9] = pos[0] + size[0];   mVertex[10] = pos[1] + size[1];   mVertex[11] = 0;

    mIndices = new unsigned short [3*2];
    mIndices[0] = 0; mIndices[1] = 1; mIndices[2] = 2;
    mIndices[3] = 1; mIndices[4] = 3; mIndices[5] = 2;

    mColor = new float [4*4];
    mColor[0]  = color[0]; mColor[1]  = color[1]; mColor[2]  = color[2]; mColor[3]  = color[3];
    mColor[4]  = color[0]; mColor[5]  = color[1]; mColor[6]  = color[2]; mColor[7]  = color[3];
    mColor[8]  = color[0]; mColor[9]  = color[1]; mColor[10] = color[2]; mColor[11] = color[3];
    mColor[12] = color[0]; mColor[13] = color[1]; mColor[14] = color[2]; mColor[15] = color[3];

    mIndicesCount = 6;
}
float * Sprite::getTextureCoord()
{
    return mTextCoord;
}
void Sprite::setColor(float color[4])
{
    mColor[0]  = color[0]; mColor[1]  = color[1]; mColor[2]  = color[2]; mColor[3]  = color[3];
    mColor[4]  = color[0]; mColor[5]  = color[1]; mColor[6]  = color[2]; mColor[7]  = color[3];
    mColor[8]  = color[0]; mColor[9]  = color[1]; mColor[10] = color[2]; mColor[11] = color[3];
    mColor[12] = color[0]; mColor[13] = color[1]; mColor[14] = color[2]; mColor[15] = color[3];
}
void Sprite::setPosition(float x, float y)
{
    mVertex[0] = x;              mVertex[1] = y;              mVertex[2] = 0;
    mVertex[3] = x + mSize[0];   mVertex[4] = y;              mVertex[5] = 0;
    mVertex[6] = x;              mVertex[7] = y + mSize[1];   mVertex[8] = 0;
    mVertex[9] = mVertex[3];     mVertex[10] = mVertex[7];    mVertex[11] = 0;
}
void Sprite::setSize(float x, float y)
{
    mSize[0] = x;
    mSize[1] = y;
}
