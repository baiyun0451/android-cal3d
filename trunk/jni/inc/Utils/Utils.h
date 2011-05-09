#ifndef _UTILS_H_
#define _UTILS_H_

// Includes:
#include <stdio.h>
#include <android/log.h>

// Utility for logging:
#define LOG_TAG    "Cal3D-Native"
#define LOG(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#endif 
