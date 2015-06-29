
LOCAL_PATH			     := $(call my-dir)

SDK_PATH                 := ../../../../../../../sdk

SDK_EXTERNAL_PATH        := $(SDK_PATH)/External

SDK_FRM_INC_PATH		    := $(SDK_PATH)/Framework/Inc

SDK_FRM_PATH			    := $(SDK_PATH)/Framework/Framework
SDK_FRM_ANDROID_PATH	    := $(SDK_FRM_PATH)/Android
SDK_FRM_MODEL_PATH	    := $(SDK_FRM_PATH)/Model
SDK_FRM_OPENGL_PATH		:= $(SDK_FRM_PATH)/OpenGLES

SDK_MY_FRM_PATH          := $(SDK_PATH)/MyFramework
SDK_MY_FRM_ANDROID_PATH  := $(SDK_MY_FRM_PATH)/Android
SDK_MY_FRM_OPENGL_PATH   := $(SDK_MY_FRM_PATH)/OpenGLES

GAME_PATH                := ../../../../../../
GAME_AI_PATH             := $(GAME_PATH)/AI
GAME_BASE_ENTS_PATH      := $(GAME_PATH)/BaseEntities
GAME_CORE_PATH           := $(GAME_PATH)/Core
GAME_GAME_OBJS_PATH      := $(GAME_PATH)/GameObjects
GAME_GRAPHICS_OBJS_PATH  := $(GAME_PATH)/GraphicsObjects
GAME_MISC_PATH           := $(GAME_PATH)/Misc
GAME_SCREENS_PATH        := $(GAME_PATH)/Screens

include $(CLEAR_VARS)

LOCAL_MODULE    := MyGame

LOCAL_SRC_FILES := $(SDK_FRM_PATH)/FrmApplication.cpp \
				   $(SDK_FRM_PATH)/FrmInput.cpp \
				   $(SDK_FRM_PATH)/FrmPackedResource.cpp \
				   $(SDK_FRM_PATH)/FrmUtils.cpp \
				   $(SDK_FRM_ANDROID_PATH)/FrmApplication_Platform.cpp \
				   $(SDK_FRM_ANDROID_PATH)/FrmFile_Platform.cpp \
				   $(SDK_FRM_ANDROID_PATH)/FrmNoise_Platform.cpp \
				   $(SDK_FRM_ANDROID_PATH)/FrmStdlib_Platform.cpp \
				   $(SDK_FRM_ANDROID_PATH)/FrmUtils_Platform.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmAnimation.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmAnimationTrack.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmIndexBuffer.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmJoint.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmMaterial.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmMemoryBuffer.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmMeshSurfaceArray.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmModel.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmModelIO.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmModelMesh.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmNamedId.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmVertexBuffer.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmVertexFormat.cpp \
				   $(SDK_FRM_MODEL_PATH)/FrmVertexProperty.cpp \
				   $(SDK_FRM_OPENGL_PATH)/FrmMesh.cpp \
				   $(SDK_FRM_OPENGL_PATH)/FrmPackedResourceGLES.cpp \
				   $(SDK_FRM_OPENGL_PATH)/FrmResourceGLES.cpp \
				   $(SDK_FRM_OPENGL_PATH)/FrmShader.cpp \
				   $(SDK_FRM_OPENGL_PATH)/FrmUtilsGLES.cpp \
				   $(SDK_MY_FRM_PATH)/MyCamera.cpp \
				   $(SDK_MY_FRM_PATH)/MyCamera2D.cpp \
				   $(SDK_MY_FRM_PATH)/MyEventListener.cpp \
				   $(SDK_MY_FRM_PATH)/MyInitFile.cpp \
				   $(SDK_MY_FRM_PATH)/MyInput.cpp \
				   $(SDK_MY_FRM_PATH)/MyMath.cpp \
				   $(SDK_MY_FRM_PATH)/MyOrthoCamera.cpp \
				   $(SDK_MY_FRM_PATH)/MyPerspectiveCamera.cpp \
				   $(SDK_MY_FRM_PATH)/MyScreen.cpp \
				   $(SDK_MY_FRM_PATH)/MyScreenManager.cpp \
				   $(SDK_MY_FRM_PATH)/MyTimer.cpp \
				   $(SDK_MY_FRM_PATH)/MyUIButton.cpp \
				   $(SDK_MY_FRM_PATH)/MyUILabel.cpp \
				   $(SDK_MY_FRM_PATH)/MyUIList.cpp \
				   $(SDK_MY_FRM_PATH)/MyUIProgressBar.cpp \
				   $(SDK_MY_FRM_PATH)/MyUIWidget.cpp \
				   $(SDK_MY_FRM_PATH)/MyUtils.cpp \
				   $(SDK_MY_FRM_ANDROID_PATH)/MyAudio.cpp \
				   $(SDK_MY_FRM_ANDROID_PATH)/MyGameServer.cpp \
				   $(SDK_MY_FRM_ANDROID_PATH)/MyJNIBridge.cpp \
				   $(SDK_MY_FRM_ANDROID_PATH)/MyUtils_Platform.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyBasicMesh.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyBillboard.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyFileMesh1.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyFont.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyInputLayout.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyMesh.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyQuad3D.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyShader.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MySkinnedMesh1.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MySkinnedMesh2.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MySpriteBatch.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MySpriteSheet.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyTexture.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyUtilsGLES.cpp \
				   $(SDK_MY_FRM_OPENGL_PATH)/MyVertex.cpp \
				   $(GAME_BASE_ENTS_PATH)/BaseEntity.cpp \
				   $(GAME_BASE_ENTS_PATH)/LivingEntity.cpp \
				   $(GAME_BASE_ENTS_PATH)/MovingEntity.cpp \
				   $(GAME_CORE_PATH)/MainGame.cpp \
				   $(GAME_GAME_OBJS_PATH)/Hero.cpp \
				   $(GAME_GAME_OBJS_PATH)/Hero_AI.cpp \
				   $(GAME_GAME_OBJS_PATH)/Hero_Controlled.cpp \
				   $(GAME_GAME_OBJS_PATH)/IFV.cpp \
				   $(GAME_GAME_OBJS_PATH)/Notify.cpp \
				   $(GAME_GAME_OBJS_PATH)/Pawn.cpp \
				   $(GAME_GAME_OBJS_PATH)/Projectile.cpp \
				   $(GAME_GAME_OBJS_PATH)/Tower.cpp \
				   $(GAME_GAME_OBJS_PATH)/Tower_Main.cpp \
				   $(GAME_GRAPHICS_OBJS_PATH)/BloodBar.cpp \
				   $(GAME_GRAPHICS_OBJS_PATH)/FlatTerrain.cpp \
				   $(GAME_GRAPHICS_OBJS_PATH)/InfoBar.cpp \
				   $(GAME_GRAPHICS_OBJS_PATH)/MiniMap.cpp \
				   $(GAME_GRAPHICS_OBJS_PATH)/PlayerInfo.cpp \
				   $(GAME_GRAPHICS_OBJS_PATH)/Shop.cpp \
				   $(GAME_MISC_PATH)/EventListener.cpp \
				   $(GAME_MISC_PATH)/HeroItem.cpp \
				   $(GAME_MISC_PATH)/HeroSkill.cpp \
				   $(GAME_MISC_PATH)/TPSCamera.cpp \
				   $(GAME_SCREENS_PATH)/Layer_HUD.cpp \
				   $(GAME_SCREENS_PATH)/Layer_World.cpp \
				   $(GAME_SCREENS_PATH)/Screen_GameOver.cpp \
				   $(GAME_SCREENS_PATH)/Screen_Menu.cpp \
				   $(GAME_SCREENS_PATH)/Screen_Menu_BuyHero.cpp \
				   $(GAME_SCREENS_PATH)/Screen_Menu_GameMode.cpp \
				   $(GAME_SCREENS_PATH)/Screen_Menu_Main.cpp \
				   $(GAME_SCREENS_PATH)/Screen_Play.cpp \
				   $(GAME_SCREENS_PATH)/Screen_Shop.cpp
				   
LOCAL_C_INCLUDES	:= $(LOCAL_PATH)/$(SDK_EXTERNAL_PATH) \
					   $(LOCAL_PATH)/$(SDK_FRM_INC_PATH) \
					   $(LOCAL_PATH)/$(SDK_FRM_PATH)	\
					   $(LOCAL_PATH)/$(SDK_FRM_ANDROID_PATH) \
					   $(LOCAL_PATH)/$(SDK_FRM_MODEL_PATH) \
					   $(LOCAL_PATH)/$(SDK_FRM_OPENGL_PATH) \
					   $(LOCAL_PATH)/$(SDK_MY_FRM_PATH)	\
					   $(LOCAL_PATH)/$(SDK_MY_FRM_ANDROID_PATH) \
					   $(LOCAL_PATH)/$(SDK_MY_FRM_OPENGL_PATH) \
					   $(LOCAL_PATH)/$(GAME_AI_PATH) \
					   $(LOCAL_PATH)/$(GAME_BASE_ENTS_PATH) \
					   $(LOCAL_PATH)/$(GAME_CORE_PATH) \
					   $(LOCAL_PATH)/$(GAME_GAME_OBJS_PATH) \
					   $(LOCAL_PATH)/$(GAME_GRAPHICS_OBJS_PATH) \
					   $(LOCAL_PATH)/$(GAME_MISC_PATH) \
					   $(LOCAL_PATH)/$(GAME_SCREENS_PATH)
					   
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES -lz

LOCAL_CFLAGS	+= -Wno-write-strings -Wno-conversion-null

LOCAL_STATIC_LIBRARIES := android_native_app_glue gpg-1

include $(BUILD_SHARED_LIBRARY)

$(call import-module,gpg-cpp-sdk/android)
$(call import-module,android/native_app_glue)
