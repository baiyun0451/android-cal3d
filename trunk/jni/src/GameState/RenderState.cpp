#include "RenderState.h"
#include "demo.h"
#include "ARGameProgram.h"

enum TouchMode {DOWN = 0,UP = 1,MOVE = 2,CANCEL = 3,OUTSIDE = 4,POINTER_DOWN= 5,POINTER_UP = 6};

RenderState::RenderState()
{

}
RenderState::~RenderState()
{
}bool RenderState::onEnter()
{
    theDemo.onInit();
	return true;
}
bool RenderState::onExit()
{
    theDemo.onShutdown();
	return true;
}
bool RenderState::onPause()
{
	return true;
}
bool RenderState::onResume()
{
	return true;
}
void RenderState::onRender()
{
    theDemo.onIdle();
    theDemo.onRender();
}

bool RenderState::onTouch(float x,float y,int touchMode,int pointerID,int pointerCount)
{
    y = Program::getInstance()->mHeight - y;
    if(touchMode == POINTER_DOWN)
    {
        theDemo.onMouseButtonUp(0,x,y);
        theDemo.onMouseButtonDown(1,x,y);
    }
    else if(touchMode == DOWN)
        theDemo.onMouseButtonDown(0,x,y);
    else if(touchMode == POINTER_UP)
        theDemo.onMouseButtonUp(1,x,y);
    else if(touchMode == UP)
        theDemo.onMouseButtonUp(0,x,y);
    else if(touchMode == MOVE)
        theDemo.onMouseMove(x,y);
	return true;
}

bool RenderState::onRendererChange(int width,int height)
{
    theDemo.setDimension(width,height);
}
