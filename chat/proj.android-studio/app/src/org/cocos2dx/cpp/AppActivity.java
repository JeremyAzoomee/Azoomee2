package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowManager;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.mixpanel.android.mpmetrics.MixpanelAPI;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

import io.fabric.sdk.android.Fabric;


public class AppActivity extends Cocos2dxActivity
{
    private static Context mContext;
    private static Activity mActivity;
    private MixpanelAPI mixpanel;
    private static int keyboardHeight = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mixpanel = MixpanelAPI.getInstance(this, "7e94d58938714fa180917f0f3c7de4c9");

        Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
        mContext = this;
        mActivity = this;

        mFrameLayout.getViewTreeObserver().addOnGlobalLayoutListener(
            new ViewTreeObserver.OnGlobalLayoutListener() {
                @Override
                public void onGlobalLayout() {
                    Rect r = new Rect();
                    View rootView = getWindow().getDecorView();
                    rootView.getWindowVisibleDisplayFrame(r);

                    int screenHeight = mFrameLayout.getHeight();
                    int heightDifference = screenHeight - (r.bottom - r.top);
                    Log.d("Keyboard Size", "Size: " + heightDifference);

                    if(heightDifference < keyboardHeight) {
                        // If the height is getting smaller
                        // Note it can shrink first down to the small black bar at the bottom (usually around 48px or so)
                        keyboardHeight = heightDifference;
                        // So if we're not shrinking to 0, trigger this as a keyboard shown
                        if(keyboardHeight > 0) {
                            onKeyboardShown(keyboardHeight);
                        }
                        // Otherwise its fully hidden and the views can return to normal
                        else {
                            onKeyboardHidden(keyboardHeight);
                        }
                    }
                    else if(keyboardHeight < 100 && heightDifference > 100) {
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

    public static int getKeyboardHeight()
    {
        return keyboardHeight;
    }

    private native void onKeyboardShown( int height );
    private native void onKeyboardHidden( int height );

    public static String getAnswer()
    {
        return "AndroidAnswer";
    }

    public static String getOSBuildManufacturer()
    {
        return android.os.Build.MANUFACTURER;
    }
    
    public static String getHMACSHA256(String message, String secret)
    {
        String hash = "";
        try
        {
            Mac sha256_HMAC = Mac.getInstance("HmacSHA256");
            SecretKeySpec secret_key = new SecretKeySpec(secret.getBytes(), "HmacSHA256");
            sha256_HMAC.init(secret_key);

            hash = Base64.encodeToString(sha256_HMAC.doFinal(message.getBytes()), Base64.DEFAULT);
        }
        catch (Exception e) {

        }

        return hash.trim();
    }

    public static void CrashlyticsLogException(String message)
    {
        Crashlytics.logException(new Exception(message));
    }

    private static void CrashlyticsLogUser(String AdultIdentifier, String ChildIdentifier)
    {
        Crashlytics.setUserIdentifier(AdultIdentifier);
        Crashlytics.setUserName(ChildIdentifier);
    }

    //----Mix Panel------

    public static void sendMixPanelWithEventID(String eventID, String jsonPropertiesString)
    {
        JSONObject _mixPanelProperties = null;

        try
        {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        }
        catch(JSONException e)
        {
            _mixPanelProperties = null;
        }

        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.track(eventID, _mixPanelProperties);
    }

    public static void sendMixPanelSuperProperties(String jsonPropertiesString)
    {
        JSONObject _mixPanelProperties = null;

        try
        {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        }
        catch(JSONException e)
        {
            _mixPanelProperties = null;
        }

        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.registerSuperProperties(_mixPanelProperties);
    }

    public static void sendMixPanelPeopleProperties(String parentID)
    {
        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.identify(parentID);
        mixpanel.getPeople().identify(parentID);
        //mixpanel.getPeople().set("First Name", parentID);
    }

    public static void showMixpanelNotification()
    {
        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.getPeople().showNotificationIfAvailable(mActivity);
    }

    public static void showMixpanelNotificationWithID(int notificationID)
    {
        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.getPeople().showNotificationById(notificationID,mActivity);
    }

    @Override
    protected void onDestroy()
    {
        mixpanel.flush();
        super.onDestroy();
    }

//    //----- Appsflyer--------
//
//    public static void sendAppsFlyerEvent(String eventID, String jsonPropertiesString)
//    {
//        // Does nothing in standalone chat app for now
//    }

}
