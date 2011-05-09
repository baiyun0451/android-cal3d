#ifndef NATIVE_PROGRAM_H#define NATIVE_PROGRAM_H

#include <jni.h>

class NativeProgram
{
public:
	virtual void RenderFrame() = 0;	virtual bool InitializeProgram(JNIEnv* env, jobject obj, int w, int h) = 0;	virtual bool DeinitializeProgram() = 0;	virtual bool InitRenderer() = 0;	virtual bool DeinitRenderer() = 0;	virtual bool OnRendererChanged(int w,int h) = 0;	virtual bool OnResume() = 0;	virtual bool OnPause() = 0;	virtual bool OnTouch(float x, float y, int touchMode,int pointerID, int pointerCount) = 0;	virtual ~NativeProgram(){};public:
    char * mReadPath;	int mWidth;	int mHeight;	bool isActivityInPortraitMode;
};
#endif
