#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "GameState.h"


class RenderState : public GameState
{
	public:
        RenderState();
        ~RenderState();
		bool onEnter();
		bool onExit();
		bool onPause();
		bool onResume();
		void onRender();
		bool onTouch(float x,float y,int touchMode,int pointerID,int pointerCount);
		bool onRendererChange(int width,int height);
};
#endif

