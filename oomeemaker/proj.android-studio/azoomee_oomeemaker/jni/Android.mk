LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Add search paths for import module
$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../common/proj.android-studio)


# Name and filename of this module
LOCAL_MODULE := azoomee_oomeemaker
LOCAL_MODULE_FILENAME := libazoomee_oomeemaker

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
LOCAL_SRC_FILES := $(call find-src-files, ../../../Classes/AzoomeeOomeeMaker, $(EXCLUDE_FILES))

# Header search paths
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# Link
LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += azoomee_common

# Export header files
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../Classes/

include $(BUILD_STATIC_LIBRARY)


# Cocos2d module
$(call import-module, cocos)
$(call import-module, azoomee_common/jni)
