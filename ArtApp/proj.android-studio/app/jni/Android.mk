LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

#LOCAL_MODULE := MyGame_shared

#LOCAL_MODULE_FILENAME := libMyGame

LOCAL_MODULE := azoomeeartapp_shared

LOCAL_MODULE_FILENAME := libAzoomeeArtApp

# Find all files of type cpp which are not named _ios.*
define find-src-files
	$(patsubst ./%, %, \
  		$(shell cd $(LOCAL_PATH) ; \
        	find $(1) -name "*.cpp" -and -not -name ".*" -and -not -name "_ios.*" | grep -vF $(2) \
        ) \
 	)
endef

# Which files to exclude from compile?
EXCLUDE_FILES := -

# Collect all source files to compile
LOCAL_SRC_FILES := $(call find-src-files, ../../../Classes, $(EXCLUDE_FILES))

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../Classes/

include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
