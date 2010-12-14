
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Database-Jni
LOCAL_SRC_FILES := DatabaseJniBridge.cpp ../../Common/Model/Hymn.cpp ../../Common/Model/Hymnal.cpp ../../Common/Model/DatabaseHelper.cpp sqlite3.c
LOCAL_LDLIBS	:= -llog -ldl

include $(BUILD_SHARED_LIBRARY)