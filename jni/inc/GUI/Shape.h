#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
    public:
        Shape();
        Shape(float * ver, unsigned short * ind, int indCount, float * col = 0,int textureID = -1, float * texCoord = 0);
        ~Shape();
        void onRender();
    protected:
        int mIndicesCount;
        float * mVertex;
        float * mColor;
        float * mTextCoord;
        unsigned short * mIndices;
        int mTextureID;
};

#endif
