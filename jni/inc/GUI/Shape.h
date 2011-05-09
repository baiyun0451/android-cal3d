#ifndef SHAPE_H
#define SHAPE_H

#ifdef USE_OPENGL_ES_1_1
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

class Shape
{
    public:
        Shape():mColor(0),mTextCoord(0),mIndices(0),mVertex(0){}
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

Shape::Shape(float * ver, unsigned short * ind, int indCount, float * col,int textureID, float * texCoord)
{
    mVertex = ver;
    mColor = col;
    mTextCoord = texCoord;
    mIndices = ind;
    mIndicesCount = indCount;
    mTextureID = textureID;
}
Shape::~Shape()
{
    delete [] mVertex;
    delete [] mIndices;
    if(mTextCoord) delete [] mTextCoord;
    if(mColor)     delete [] mColor;
}
void Shape::onRender()
{
    if(mTextCoord)
    {
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, (const GLvoid *)&mTextCoord[0]);
    }
    if(mColor)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, (const GLvoid*)&mColor[0]);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (const GLvoid*)&mVertex[0]);
    glDrawElements(GL_TRIANGLES, mIndicesCount ,GL_UNSIGNED_SHORT,(const GLvoid *)&mIndices[0]);
    glDisableClientState(GL_VERTEX_ARRAY);

    if(mColor)
        glDisableClientState(GL_COLOR_ARRAY);
    if(mTextCoord)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }
}
#endif
