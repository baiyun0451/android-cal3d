MY_DIR := $(call my-dir)


include $(MY_DIR)/Android-build.mk
$(shell chmod 777 ../obj/local/armeabi/libstlport_static.a)

#$(shell chmod 777 ../obj/local/armeabi-v7a/libstlport_shared.so)