LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/BaseScene.cpp \
                   ../../../Classes/ChildSelectorScene.cpp \
                   ../../../Classes/HQScene.cpp \
                   ../../../Classes/IntroScene.cpp \
                   ../../../Classes/LoginScene.cpp \
                   ../../../Classes/MainHubBgElements.cpp \
                   ../../../Classes/MainHubScene.cpp \
                   ../../../Classes/NavigationLayer.cpp \
                   ../../../Classes/WebViewSelector.cpp \
                   ../../../Classes/ImageContainer.cpp \
                   ../../../Classes/OomeeLayer.cpp \
                   ../../../Classes/HQSceneElement.cpp \
                   ../../../Classes/ModalMessages.cpp \
                   ../../../Classes/BackEndCaller.cpp \
                   ../../../Classes/JWTTool.cpp \
                   ../../../Classes/HMACSHA256_javaCaller.cpp \
                   ../../../Classes/HMACSHA256.cpp \
                   ../../../Classes/DataStorage.cpp \
                   ../../../Classes/HQDataProvider.cpp \
                   ../../../Classes/ImageDownloader.cpp \
                   ../../../Classes/GameDataManager.cpp \
                   ../../../Classes/HttpRequestCreator.cpp \
                   ../../../Classes/ConfigStorage.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
