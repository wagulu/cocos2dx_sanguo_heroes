LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared
LOCAL_MODULE_FILENAME := libcocos2dlua

#1 定义查找所有cpp文件的宏
define all-cpp-files-under
$(patsubst ./%,%, $(shell find $(LOCAL_PATH)/../../Classes -name "platform" -prune -o -name "*.cpp" -and -not -name ".*"))
endef

define all-subdir-cpp-files
$(call all-cpp-files-under,.)
endef

#2 定义查找所有c文件的宏
define all-c-files-under
$(patsubst ./%,%, $(shell find $(LOCAL_PATH)/../../Classes -name "platform" -prune -o -name "*.c" -and -not -name ".*"))
endef

define all-subdir-c-files
$(call all-c-files-under,.)
endef

#3 定义查找所有hpp文件的宏
define all-hpp-files-under
$(patsubst ./%,%, $(shell find $(LOCAL_PATH)/../../Classes -name "platform" -prune -o -name "*.hpp" -and -not -name ".*"))
endef

define all-subdir-hpp-files
$(call all-hpp-files-under,.)
endef

#4 定义查找所有cc文件的宏
define all-cc-files-under
$(patsubst ./%,%, $(shell find $(LOCAL_PATH)/../../Classes -name "platform" -prune -o -name "*.cc" -and -not -name ".*"))
endef

define all-subdir-cc-files
$(call all-cc-files-under,.)
endef

CPP_FILE_LIST := hellolua/main.cpp 
CPP_FILE_LIST += $(call all-subdir-cpp-files) 
C_FILE_LIST := $(call all-subdir-c-files)
HPP_FILE_LIST := $(call all-subdir-hpp-files)
CC_FILE_LIST := $(call all-subdir-cc-files)

# 查找所有 .c .cpp .hpp .cc 可编译文件
LOCAL_SRC_FILES := $(CPP_FILE_LIST:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(C_FILE_LIST:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(HPP_FILE_LIST:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(CC_FILE_LIST:$(LOCAL_PATH)/%=%) 

# 查找所有 .h 头文件
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(shell ls -FR $(LOCAL_C_INCLUDES) | grep $(LOCAL_PATH)/$ )
LOCAL_C_INCLUDES += $(LOCAL_C_INCLUDES:$(LOCAL_PATH)/%:=$(LOCAL_PATH)/%)

LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
LOCAL_STATIC_LIBRARIES += cocos2d_simulator_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,scripting/lua-bindings/proj.android/prebuilt-mk)
$(call import-module,tools/simulator/libsimulator/proj.android/prebuilt-mk)

