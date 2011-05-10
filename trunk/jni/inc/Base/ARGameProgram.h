#ifndef AR_GAME_PROGRAM_H#define AR_GAME_PROGRAM_H
#include "AndroidWrapper.h"#include "GameStateManager.h"

class Program : public NativeProgram
{	public:		static Program * getInstance();
		void RenderFrame();		bool InitializeProgram(JNIEnv* env, jobject obj, int w, int h);		bool DeinitializeProgram();		bool InitRenderer();		bool DeinitRenderer();		bool OnRendererChanged(int w, int h);		bool OnResume();		bool OnPause();
		bool OnTouch(float x,float y,int touchMode,int pointerID,int pointerCount);	private:		Program();		~Program();		static Program * mInstance;		GameStateManager * mGameStateManager;		friend NativeProgram * NewEntity();
};NativeProgram * NewEntity();
#endif

