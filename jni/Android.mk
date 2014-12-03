LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := xdsu
LOCAL_SRC_FILES := su.c bignum.c su_client.c su_log.c su_util.c
include $(BUILD_EXECUTABLE)
