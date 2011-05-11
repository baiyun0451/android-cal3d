#ifndef _UTILS_H_
#define _UTILS_H_

// Includes:
#include <stdio.h>
#include <android/log.h>
#include <time.h>

// Utility for logging:
#define LOG_TAG    "Cal3D-Native"
#define LOG(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define MINN(x,y) ((x > y)?y:x)
#define MAXX(x,y) ((x > y)?x:y)
#define CLAMP(x,s,b) MINN(b, MAXX(x, s))
//return milli seconds
namespace Utils
{

static float getCurrentTime()
{
    struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_sec*1000 + now.tv_nsec / 1000000.f;
}

};

#endif
