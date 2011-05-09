LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Cal3DNative
LOCAL_SRC_FILES = ../obj/libCal3DNative.so
include $(PREBUILT_SHARED_LIBRARY)
LOCAL_EXPORT_C_INCLUDES := cal3d

include $(CLEAR_VARS)

LOCAL_MODULE := AndroidWrapper
TARGET_PLATFORM := android-5
	
USE_OPENGL_ES_1_1 := true

ifeq ($(USE_OPENGL_ES_1_1), true)
    OPENGLES_LIB  := -lGLESv1_CM
	OPENGLES_DEF  := -DUSE_OPENGL_ES_1_1
else
    OPENGLES_LIB  := -lGLESv2
	OPENGLES_DEF  := -DUSE_OPENGL_ES_2_0
endif

LOCAL_CFLAGS := -Wno-write-strings $(OPENGLES_DEF)

LOCAL_SHARED_LIBRARIES := Cal3DNative

LOCAL_LDLIBS := \
    -llog $(OPENGLES_LIB)
	
ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS += -fno-stack-protector 
endif
	
#LOCAL_SHARED_LIBRARIES := QCAR-prebuilt
	
LOCAL_C_INCLUDES :=	inc	\
					inc/Base \
					inc/GameState \
					inc/Utils	\
					inc/GUI	\
					program
					
LOCAL_SRC_FILES :=  src/Base/ARGameProgram.cpp	\
					src/Base/GameStateManager.cpp \
					src/Base/AndroidWrapper.cpp	\
					program/model.cpp	 
				
LOCAL_ARM_MODE := arm
include $(BUILD_SHARED_LIBRARY)
