LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := helloworld/main.cpp
MY_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)
                   
MY_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*/*/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*/*/*/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*/*/*/*/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)				   

MY_FILES := $(wildcard $(LOCAL_PATH)/../../libs/chipmunk/src/*.c)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../libs/chipmunk/src/*/*.c)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   
LOCAL_C_INCLUDES +=	$(LOCAL_PATH)/../../libs/chipmunk/include/chipmunk
LOCAL_C_INCLUDES +=	$(LOCAL_PATH)/../../Classes/artpig/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2dx/platform/third_party/android

LOCAL_STATIC_LIBRARIES := curl_static_prebuilt 
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static

  
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) $(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
