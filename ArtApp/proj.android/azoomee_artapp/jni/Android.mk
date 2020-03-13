LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../common/proj.android)

LOCAL_MODULE := azoomee_art

LOCAL_MODULE_FILENAME := libazoomee_art

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
LOCAL_SRC_FILES := $(call find-src-files, ../../../Classes/AzoomeeArt, $(EXCLUDE_FILES))

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes/AzoomeeArt

LOCAL_STATIC_LIBRARIES := cc_static
LOCAL_STATIC_LIBRARIES += tinizine_common

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../Classes/

include $(BUILD_STATIC_LIBRARY)

$(call import-module, cocos)
$(call import-module, tinizine_common/jni)