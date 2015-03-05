
#include <jni.h>
#include "gpg/gpg.h"

extern const char* g_nativeActivityName;

extern "C"
{
	void nativeOnActivityResult(JNIEnv *env, jobject thiz,
		jobject activity, jint requestCode, jint resultCode, jobject data)
	{
		gpg::AndroidSupport::OnActivityResult(env, activity, requestCode, resultCode, data);
	}

	void nativeOnActivityCreated(JNIEnv *env, jobject thiz,
		jobject activity, jobject saved_instance_state)
	{
		gpg::AndroidSupport::OnActivityCreated(env, activity, saved_instance_state);
	}

	void nativeOnActivityDestroyed(JNIEnv *env, jobject thiz,
		jobject activity)
	{
		gpg::AndroidSupport::OnActivityDestroyed(env, activity);
	}

	void nativeOnActivityPaused(JNIEnv *env, jobject thiz,
		jobject activity)
	{
		gpg::AndroidSupport::OnActivityPaused(env, activity);
	}

	void nativeOnActivityResumed(JNIEnv *env, jobject thiz,
		jobject activity)
	{
		gpg::AndroidSupport::OnActivityResumed(env, activity);
	}

	void nativeOnActivitySaveInstanceState(JNIEnv *env, jobject thiz,
		jobject activity, jobject out_state)
	{
		gpg::AndroidSupport::OnActivitySaveInstanceState(env, activity, out_state);
	}

	void nativeOnActivityStarted(JNIEnv *env, jobject thiz,
		jobject activity)
	{
		gpg::AndroidSupport::OnActivityStarted(env, activity);
	}

	void nativeOnActivityStopped(JNIEnv *env, jobject thiz,
		jobject activity)
	{
		gpg::AndroidSupport::OnActivityStopped(env, activity);
	}

	// Registering JNI Functions

	static const int NUM_REGISTERED_METHODS = 8;

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pVm, void* reserved)
	{
		JNIEnv* env;
		if (pVm->GetEnv((void **)&env, JNI_VERSION_1_6))
		{
			return -1;
		}

		JNINativeMethod nm[NUM_REGISTERED_METHODS];

		// Registering JNI Functions

		nm[0].name = "nativeOnActivityResult";
		nm[0].signature = "(Landroid/app/Activity;IILandroid/content/Intent;)V";
		nm[0].fnPtr = (void*)nativeOnActivityResult;

		nm[1].name = "nativeOnActivityCreated";
		nm[1].signature = "(Landroid/app/Activity;Landroid/os/Bundle;)V";
		nm[1].fnPtr = (void*)nativeOnActivityCreated;

		nm[2].name = "nativeOnActivityDestroyed";
		nm[2].signature = "(Landroid/app/Activity;)V";
		nm[2].fnPtr = (void*)nativeOnActivityDestroyed;

		nm[3].name = "nativeOnActivityPaused";
		nm[3].signature = "(Landroid/app/Activity;)V";
		nm[3].fnPtr = (void*)nativeOnActivityPaused;

		nm[4].name = "nativeOnActivityResumed";
		nm[4].signature = "(Landroid/app/Activity;)V";
		nm[4].fnPtr = (void*)nativeOnActivityResumed;

		nm[5].name = "nativeOnActivitySaveInstanceState";
		nm[5].signature = "(Landroid/app/Activity;Landroid/os/Bundle;)V";
		nm[5].fnPtr = (void*)nativeOnActivitySaveInstanceState;

		nm[6].name = "nativeOnActivityStarted";
		nm[6].signature = "(Landroid/app/Activity;)V";
		nm[6].fnPtr = (void*)nativeOnActivityStarted;

		nm[7].name = "nativeOnActivityStopped";
		nm[7].signature = "(Landroid/app/Activity;)V";
		nm[7].fnPtr = (void*)nativeOnActivityStopped;

		// Registering java class containing JNI Functions

		jclass cls = env->FindClass(g_nativeActivityName);

		env->RegisterNatives(cls, nm, NUM_REGISTERED_METHODS);

		return JNI_VERSION_1_6;
	}
}