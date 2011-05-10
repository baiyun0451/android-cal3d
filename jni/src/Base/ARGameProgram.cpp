#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ARGameProgram.h"
#include "GameStateManager.h"
#include "Utils.h"

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "RenderState.h"

Program * Program::mInstance = 0;

NativeProgram * NewEntity(){	return Program::getInstance();}

Program * Program::getInstance()
{
	if(mInstance == NULL)
		mInstance = new Program();
	return mInstance;
}Program::Program()
{
	mGameStateManager = new GameStateManager();
}
Program::~Program()
{
	//DeinitializeProgram();
	mInstance = NULL;
}
bool Program::InitRenderer()
{
	// Define clear color
	mGameStateManager->pushState(new RenderState);
    //mGameStateManager->pushState(new TestState);
    return true;
}

bool Program::DeinitRenderer()
{
    return true;
}
bool Program::OnRendererChanged(int width, int height)
{
	mWidth = width;
	mHeight = height;
	if(mGameStateManager->getCurrentState() != NULL)
		mGameStateManager->getCurrentState()->onRendererChange(width,height);
    return true;
}


void Program::RenderFrame()
{
	mGameStateManager->getCurrentState()->onRender();
}

bool Program::InitializeProgram(JNIEnv* env, jobject obj, int width, int height)
{
	 // Store screen dimensions
    LOG("InitializingNativeProgram");
    mWidth = width;
    mHeight = height;

    LOG("InitializedNativeProgram");
	return true;
}
bool Program::DeinitializeProgram(){
	mGameStateManager->~GameStateManager();
	return true;
}

bool Program::OnResume()
{
	if(mGameStateManager->getCurrentState() != NULL)		mGameStateManager->getCurrentState()->onResume();
    return true;
}

bool Program::OnPause()
{
	if(mGameStateManager->getCurrentState() != NULL)
		mGameStateManager->getCurrentState()->onPause();
    return true;
}

bool Program::OnTouch(float x,float y,int touchMode,int pointerID,int pointerCount)
{
	if(mGameStateManager->getCurrentState() != NULL)
		mGameStateManager->getCurrentState()->onTouch(x,y,touchMode, pointerID, pointerCount);
    return true;
}
