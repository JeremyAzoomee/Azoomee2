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
                   ../../../Classes/LoginScene.cpp \
                   ../../../Classes/OnboardingScene.cpp \
                   ../../../Classes/ChildAccountScene.cpp \
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
                   ../../../Classes/ImageDownloader.cpp \
                   ../../../Classes/GameDataManager.cpp \
                   ../../../Classes/HttpRequestCreator.cpp \
                   ../../../Classes/ConfigStorage.cpp \
                   ../../../Classes/TextInputLayer.cpp \
                   ../../../Classes/TextInputChecker.cpp \
                   ../../../Classes/ElectricDreamsButton.cpp \
                   ../../../Classes/StringMgr.cpp \
                   ../../../Classes/HQSceneElementPositioner.cpp \
                   ../../../Classes/ParentDataStorage.cpp \
                   ../../../Classes/ParentDataParser.cpp \
                   ../../../Classes/ParentDataProvider.cpp \
                   ../../../Classes/ChildDataStorage.cpp \
                   ../../../Classes/ChildDataParser.cpp \
                   ../../../Classes/ChildDataProvider.cpp \
                   ../../../Classes/CookieDataStorage.cpp \
                   ../../../Classes/CookieDataParser.cpp \
                   ../../../Classes/CookieDataProvider.cpp \
                   ../../../Classes/HQDataStorage.cpp \
                   ../../../Classes/HQDataParser.cpp \
                   ../../../Classes/HQDataProvider.cpp \
                   ../../../Classes/CrashlyticsConfig.cpp \
                   ../../../Classes/Crashlytics_android.cpp \
                   ../../../Classes/IntroVideoScene.cpp \
                   ../../../Classes/MessageBox.cpp \
                   ../../../Classes/AwaitingAdultPinLayer.cpp \
                   ../../../Classes/ExitOrLogoutLayer.cpp \
                   ../../../Classes/ArtsAppHQElement.cpp \
                   ../../../Classes/DisplayChildNameLayer.cpp \
                   ../../../Classes/SlideShowScene.cpp \
                   ../../../Classes/ArtsPreviewLayer.cpp \
                   ../../../Classes/WebViewNativeCaller_android.cpp \
                   ../../../Classes/ImageDownloaderLogic.cpp \
                   ../../../Classes/HQHistoryManager.cpp \
                   ../../../Classes/AudioMixer.cpp \
                   ../../../Classes/AnalyticsSingleton.cpp \
                   ../../../Classes/Analytics_android.cpp \
                   ../../../Classes/ElectricDreamsTextStyles.cpp \
                   ../../../Classes/JWTToolForceParent.cpp

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
