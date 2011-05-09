#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState
{
	public:
		virtual bool onEnter() = 0;
		virtual bool onExit() = 0;
		virtual bool onPause() = 0;
		virtual bool onResume() = 0;

		virtual void onRender() = 0;
		virtual bool onTouch(float x,float y,int touchMode,int pointerID,int pointerCount) = 0;
		virtual bool onRendererChange(int,int) = 0;
};

#endif
