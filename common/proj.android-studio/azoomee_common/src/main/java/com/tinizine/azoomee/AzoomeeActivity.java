package com.tinizine.azoomee;

import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowManager;

import org.cocos2dx.lib.Cocos2dxActivity;


public class AzoomeeActivity extends Cocos2dxActivity
{
    private static int keyboardHeight = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

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
                    int heightDifference = screenHeight - (r.bottom - r.top);
                    Log.d("Keyboard Size", "Size: " + heightDifference);

                    if(heightDifference < keyboardHeight)
                    {
                        // If the height is getting smaller
                        // Note it can shrink first down to the small black bar at the bottom (usually around 48px or so)
                        keyboardHeight = heightDifference;
                        // So if we're not shrinking to 0, trigger this as a keyboard shown
                        if(keyboardHeight > 0)
                        {
                            onKeyboardShown(keyboardHeight);
                        }
                        // Otherwise its fully hidden and the views can return to normal
                        else
                        {
                            onKeyboardHidden(keyboardHeight);
                        }
                    }
                    else if(keyboardHeight < 100 && heightDifference > 100)
                    {
                        // One shown event the first time the keyboard height is detected as over 100
                        keyboardHeight = heightDifference;
                        onKeyboardShown(keyboardHeight);
                    }
                }
            }
        );

        // Override cocos' default soft mode
        // This is needed so we get onGlobalLayout events when keyboard is dismissed
        Window window = this.getWindow();
        window.setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
    }

    private native void onKeyboardShown(int height);
    private native void onKeyboardHidden(int height);

}
