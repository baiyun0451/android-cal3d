#ifndef AR_GAME_PROGRAM_H


class Program : public NativeProgram
{
		void RenderFrame();
		bool OnTouch(float x,float y,int touchMode,int pointerID,int pointerCount);
};
#endif
