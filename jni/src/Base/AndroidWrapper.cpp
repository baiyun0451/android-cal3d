
#include <jni.h>
#include <android/log.h>
#include "AndroidWrapper.h"

#ifdef __cplusplus
extern "C"
{
#endif

NativeProgram * entity;


JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_initNativeProgram(JNIEnv * env, jobject obj, jint width, jint height)
{
	entity->InitializeProgram(env,obj,width, height);
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_initNative(JNIEnv * env, jobject obj)
{
	entity = NewEntity();
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_deinitNativeProgram(JNIEnv *, jobject)
{
	entity->DeinitializeProgram();
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_deinitNative(JNIEnv *, jobject)
{
	delete entity;
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_setActivityPortraitMode(JNIEnv *, jobject, jboolean isPortrait)
{
	entity->isActivityInPortraitMode = isPortrait;
}
JNIEXPORT int JNICALL Java_com_cal3d_android_Cal3DActivity_getOpenGlEsVersionNative(JNIEnv *, jobject)
{
#ifdef USE_OPENGL_ES_1_1
    return 1;
#else
    return 2;
#endif
}
JNIEXPORT void JNICALL Java_com_cal3d_android_ARRenderer_updateNativeRenderer(JNIEnv* env, jobject obj, jint width, jint height)
{
	entity->OnRendererChanged(width, height);
}
JNIEXPORT void JNICALL Java_com_cal3d_android_ARRenderer_initNativeRenderer(JNIEnv* env, jobject obj)
{
	entity->InitRenderer();
}
JNIEXPORT void JNICALL Java_com_cal3d_android_ARRenderer_deinitNativeRenderer(JNIEnv* env, jobject obj)
{
	entity->DeinitRenderer();
}
JNIEXPORT void JNICALL Java_com_cal3d_android_ARRenderer_renderFrame(JNIEnv *, jobject)
{
	entity->RenderFrame();
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_onNativeResume(JNIEnv *, jobject)
{
	entity->OnResume();
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_onNativePause(JNIEnv *, jobject)
{
	entity->OnPause();
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_onNativeMultiTouch(JNIEnv * env, jobject, jfloat x, jfloat y, jint touchMode, jint pointerID, jint pointerCount)
{
	entity->OnTouch(x,y,touchMode,pointerID,pointerCount);
}
JNIEXPORT void JNICALL Java_com_cal3d_android_Cal3DActivity_SetReadPath(JNIEnv * env, jobject, jstring path)
{
	entity->mReadPath = (char *)env->GetStringUTFChars(path, 0);
}

#ifdef __cplusplus
}
#endif


