#ifndef NATIVE_PROGRAM_H

#include <jni.h>

class NativeProgram
{
public:
	virtual void RenderFrame() = 0;
    char * mReadPath;
};
#endif