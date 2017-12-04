package com.tinizine.azoomee.common;

import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;


import org.cocos2dx.lib.Cocos2dxActivity;


public class AzoomeeActivity extends Cocos2dxActivity implements KeyboardHeightObserver
{
    private final static String TAG = "AzoomeeActivity";

    private static AzoomeeActivity sInstance = null;
    private KeyboardHeightProvider keyboardHeightProvider;
    private int keyboardHeight = 0;
    private int globalLayoutHeightDiff = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        sInstance = this;
        super.onCreate(savedInstanceState);

        // If mFrameLayout hasn't been created, then the activity is going to be destroyed
        // For context, see Cocos2dxActivity onCreate !isTaskRoot() workaround.
        if(mFrameLayout == null)
        {
            return;
        }

        keyboardHeightProvider = new KeyboardHeightProvider(this);

        // make sure to start the keyboard height provider after the onResume
        // of this activity. This is because a popup window must be initialised
        // and attached to the activity root view.
        mFrameLayout.post(new Runnable() {
            public void run() {
                keyboardHeightProvider.start();
            }
        });

        mFrameLayout.getViewTreeObserver().addOnGlobalLayoutListener(
            new ViewTreeObserver.OnGlobalLayoutListener()
            {
                @Override
                public void onGlobalLayout()
                {
                    Rect r = new Rect();
                    View rootView = getWindow().getDecorView();
                    rootView.getWindowVisibleDisplayFrame(r);

                    int screenHeight = mFrameLayout.getHeight();
                    globalLayoutHeightDiff = screenHeight - (r.bottom - r.top);
                    Log.d(TAG, "onGlobalLayout globalLayoutHeightDiff = " + globalLayoutHeightDiff);
                }
            }
        );
    }

    @Override
    public void onPause()
    {
        super.onPause();
        keyboardHeightProvider.setKeyboardHeightObserver(null);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        keyboardHeightProvider.setKeyboardHeightObserver(this);
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        keyboardHeightProvider.close();
    }

    /// Run a ask on the GL thread when code needs to sync with the Cocos main thread
    public static void RunOnGLThread(final Runnable pRunnable)
    {
        sInstance.runOnGLThread(pRunnable);
    }

    /// Run a ask on the UI thread when code needs to sync with the Android main thread
    public static void RunOnUIThread(final Runnable pRunnable)
    {
        sInstance.runOnUiThread(pRunnable);
    }

    @Override
    public void onKeyboardHeightChanged(int height, int orientation)
    {
        String or = orientation == Configuration.ORIENTATION_PORTRAIT ? "portrait" : "landscape";
        Log.d(TAG, "onKeyboardHeightChanged height = " + height + " " + or);

        // Usually height doesn't include the status bar, but globalLayoutHeightDiff does
        int fullHeight = height;
        if(height > 0)
        {
            // If the height diff from global layout is largest, use that only
            if(globalLayoutHeightDiff > height)
            {
                fullHeight = globalLayoutHeightDiff;
            }
            else if(globalLayoutHeightDiff < 100)
            {
                // Otherwise use both values combined
                // This is because in this instance, globalLayoutHeightDiff contains the status bar height
                // But height does not, so we need both
                fullHeight += globalLayoutHeightDiff;
            }
        }

        Log.d(TAG, "onKeyboardHeightChanged fullHeight = " + fullHeight + " " + or);

        if(fullHeight < keyboardHeight)
        {
            // If the height is getting smaller
            // Note it can shrink first down to the small black bar at the bottom
            keyboardHeight = fullHeight;
            // So if we're not shrinking to 0, trigger this as a keyboard shown
            if(keyboardHeight > 0)
            {
                onKeyboardShownThreadSafe(keyboardHeight);
            }
            // Otherwise its fully hidden and the views can return to normal
            else
            {
                onKeyboardHiddenThreadSafe(keyboardHeight);
            }
        }
        else if(keyboardHeight < 100 && fullHeight > 100)
        {
            // One shown event the first time the keyboard height is detected as over 100
            keyboardHeight = fullHeight;
            onKeyboardShownThreadSafe(keyboardHeight);
        }

        Log.d(TAG, "onKeyboardHeightChanged keyboardHeight = " + keyboardHeight);
    }


    private void onKeyboardShownThreadSafe(final int height)
    {
        runOnGLThread(new Runnable() {
            @Override
            public void run() {
                onKeyboardShown(height);
            }
        });
    }

    private void onKeyboardHiddenThreadSafe(final int height)
    {
        runOnGLThread(new Runnable() {
            @Override
            public void run() {
                onKeyboardHidden(height);
            }
        });
    }

    /// Called when keyboard is shown
    private native void onKeyboardShown(int height);
    /// Called when the keyboard is hidden
    private native void onKeyboardHidden(int height);


    //-----------------------FORCED ORIENTATION CHANGES------------------------

    public static void setOrientationPortrait()
    {
        sInstance.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    public static void setOrientationLandscape()
    {
        sInstance.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
    }

    public static void setOrientationAny()
    {
        sInstance.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
    }

}
