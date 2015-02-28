
LOCAL_PATH			  := $(call my-dir)

GLOB_INC_PATH		  := ../../../../../../AdrenoSDK/Inc

EXTERNAL_LIB_PATH     := ../../../../../../AdrenoSDK/External

FRM_PATH			  := ../../../../../../AdrenoSDK/Framework
FRM_OPENGL_PATH		  := ../../../../../../AdrenoSDK/Framework/OpenGLES
FRM_ANDROID_PATH	  := ../../../../../../AdrenoSDK/Framework/Android

EXT_FRM_PATH          := ../../../../../../AdrenoSDK/ExtFramework
EXT_FRM_OPENGL_PATH   := ../../../../../../AdrenoSDK/ExtFramework/OpenGLES
EXT_FRM_ANDROID_PATH  := ../../../../../../AdrenoSDK/ExtFramework/Android

MY_GAME_PATH          := ../../../../../../AdrenoSDK/MyGame

include $(CLEAR_VARS)

LOCAL_MODULE    := MyGame

LOCAL_SRC_FILES := $(MY_GAME_PATH)/TestScene.cpp \
				   $(FRM_OPENGL_PATH)/FrmFontGLES.cpp \
				   $(FRM_OPENGL_PATH)/FrmMesh.cpp \
				   $(FRM_OPENGL_PATH)/FrmPackedResourceGLES.cpp \
				   $(FRM_OPENGL_PATH)/FrmResourceGLES.cpp \
				   $(FRM_OPENGL_PATH)/FrmShader.cpp \
				   $(FRM_OPENGL_PATH)/FrmUserInterfaceGLES.cpp \
				   $(FRM_OPENGL_PATH)/FrmUtilsGLES.cpp \
				   $(FRM_PATH)/FrmApplication.cpp \
				   $(FRM_PATH)/FrmFont.cpp \
				   $(FRM_PATH)/FrmInput.cpp \
				   $(FRM_PATH)/FrmPackedResource.cpp \
				   $(FRM_PATH)/FrmUserInterface.cpp \
				   $(FRM_PATH)/FrmUtils.cpp \
				   $(FRM_ANDROID_PATH)/FrmApplication_Platform.cpp \
				   $(FRM_ANDROID_PATH)/FrmFile_Platform.cpp \
				   $(FRM_ANDROID_PATH)/FrmStdlib_Platform.cpp \
				   $(FRM_ANDROID_PATH)/FrmUtils_Platform.cpp \
				   $(FRM_ANDROID_PATH)/FrmNoise_Platform.cpp \
				   $(EXT_FRM_ANDROID_PATH)/ExtFrmAudio.cpp \
				   $(EXT_FRM_ANDROID_PATH)/ExtFrmAudioManager.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmBasicMesh.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmInputLayout.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmMesh.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmShader.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmSkinnedMesh.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmTexture2D.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmUIButton.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmUIWidget.cpp \
				   $(EXT_FRM_OPENGL_PATH)/ExtFrmVertex.cpp \
				   $(EXT_FRM_PATH)/ExtFrmCamera.cpp \
				   $(EXT_FRM_PATH)/ExtFrmFPSCamera.cpp \
				   $(EXT_FRM_PATH)/ExtFrmInput.cpp \
				   $(EXT_FRM_PATH)/ExtFrmMath.cpp \
				   $(EXT_FRM_PATH)/ExtFrmScreen.cpp \
				   $(EXT_FRM_PATH)/ExtFrmScreenManager.cpp \
				   $(EXT_FRM_PATH)/ExtFrmUtils.cpp
				   

LOCAL_C_INCLUDES	:= $(LOCAL_PATH)/$(GLOB_INC_PATH) \
					   $(LOCAL_PATH)/$(EXTERNAL_LIB_PATH) \
					   $(LOCAL_PATH)/$(FRM_PATH)	\
					   $(LOCAL_PATH)/$(FRM_OPENGL_PATH) \
					   $(LOCAL_PATH)/$(FRM_ANDROID_PATH) \
					   $(LOCAL_PATH)/$(EXT_FRM_PATH)	\
					   $(LOCAL_PATH)/$(EXT_FRM_OPENGL_PATH) \
					   $(LOCAL_PATH)/$(EXT_FRM_ANDROID_PATH)
				   
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES

LOCAL_CFLAGS	+= -Wno-write-strings -Wno-conversion-null

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
