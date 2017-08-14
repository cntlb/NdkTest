LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := native-lib
LOCAL_SRC_FILES := native_memory.cpp native_reference.cpp

LOCAL_CPPFLAGS := -std=c++11
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)