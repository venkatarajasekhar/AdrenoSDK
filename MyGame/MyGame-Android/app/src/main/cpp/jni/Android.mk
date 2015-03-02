
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SRC_PATH := src

LOCAL_MODULE    := MyGame

LOCAL_SRC_FILES := $(SRC_PATH)/Main.cpp\
				   $(SRC_PATH)/StateManager.cpp
				   
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lz
LOCAL_STATIC_LIBRARIES := android_native_app_glue gpg-1 gnustl-static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,gpg-cpp-sdk/android)
$(call import-module,android/native_app_glue)
