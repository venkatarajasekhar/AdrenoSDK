package com.game3d.mygame_android;

import android.app.Activity;
import android.app.NativeActivity;
import android.content.Intent;
import android.os.Bundle;

public class MyGameNativeActivity extends NativeActivity {

    // Load SO
    static {
        System.load("libMyGame.so");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        nativeOnActivityCreated(this, savedInstanceState);
    }

    protected void onResume() {
        super.onResume();

        nativeOnActivityResumed(this);
    }

    protected void onPause() {
        super.onPause();

        nativeOnActivityPaused(this);
    }

    protected void onDestroy() {
        super.onDestroy();
        nativeOnActivityDestroyed(this);
    }

    protected void onStart() {
        super.onStart();
        nativeOnActivityStarted(this);
    }

    protected void onStop() {
        super.onStop();
        nativeOnActivityStopped(this);
    }

    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        nativeOnActivitySaveInstanceState(this, outState);
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        nativeOnActivityResult(this, requestCode,resultCode, data);
    }

    // Implemented in C++.

    public static native void nativeOnActivityResult(Activity activity, int requestCode, int resultCode, Intent data);

    public static native void nativeOnActivityCreated(Activity activity, Bundle savedInstanceState);

    private static native void nativeOnActivityDestroyed(Activity activity);

    private static native void nativeOnActivityPaused(Activity activity);

    private static native void nativeOnActivityResumed(Activity activity);

    private static native void nativeOnActivitySaveInstanceState( Activity activity, Bundle outState);

    private static native void nativeOnActivityStarted(Activity activity);

    private static native void nativeOnActivityStopped(Activity activity);
}
