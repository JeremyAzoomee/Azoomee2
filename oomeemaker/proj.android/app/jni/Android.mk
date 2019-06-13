LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../common/proj.android-studio)
$(call import-add-path, $(LOCAL_PATH)/../..)

LOCAL_MODULE := AzoomeeOomeeMaker_shared

LOCAL_MODULE_FILENAME := libAzoomeeOomeeMakerApp

define find-src-files
	$(patsubst ./%, %, \
  		$(shell cd $(LOCAL_PATH) ; \
        	find $(1) -name "*.cpp" -and -not -name ".*" -and -not -name "_ios.*" | grep -vF $(2) \
        ) \
 	)
endef
# Which files to exclude from compile?
EXCLUDE_FILES := ../../../Classes/AzoomeeOomeeMaker/

# Collect all source files to compile
LOCAL_SRC_FILES := $(call find-src-files, ../../../Classes, $(EXCLUDE_FILES))
LOCAL_SRC_FILES += main.cpp

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += azoomee_common
LOCAL_STATIC_LIBRARIES += azoomee_oomeemaker

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)
$(call import-module, azoomee_common/jni)
$(call import-module, azoomee_oomeemaker/jni)
