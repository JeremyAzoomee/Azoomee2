LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../common/proj.android-studio)
$(call import-add-path, $(LOCAL_PATH)/../../../chat/proj.android-studio)
$(call import-add-path, $(LOCAL_PATH)/../../../artapp/proj.android-studio)
$(call import-add-path, $(LOCAL_PATH)/../../../oomeemaker/proj.android-studio)

LOCAL_MODULE := MyGame_shared
LOCAL_MODULE_FILENAME := libMyGame

# Find all files of type cpp which are not named _ios.*
define find-src-files
	$(patsubst ./%, %, \
  		$(shell cd $(LOCAL_PATH) ; \
        	find $(1) -name "*.cpp" -and -not -name ".*" -and -not -name "_ios.*" | \
            grep -vF $(2) \
        ) \
 	)
endef

# Which files to exclude from compile?
EXCLUDE_FILES := ../../../Classes/exDataStorage.cpp

# Collect all source files to compile
LOCAL_SRC_FILES := $(call find-src-files, ../../../Classes, $(EXCLUDE_FILES))
LOCAL_SRC_FILES += main.cpp

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += azoomee_common
LOCAL_STATIC_LIBRARIES += azoomee_chat
LOCAL_STATIC_LIBRARIES += azoomee_art
LOCAL_STATIC_LIBRARIES += azoomee_oomeemaker

include $(BUILD_SHARED_LIBRARY)


$(call import-module, cocos)
$(call import-module, azoomee_common/jni)
$(call import-module, azoomee_chat/jni)
$(call import-module, azoomee_artapp/jni)
$(call import-module, azoomee_oomeemaker/jni)
