package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Base64;

import com.tinizine.azoomee.common.AzoomeeActivity;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.mixpanel.android.mpmetrics.MixpanelAPI;

import org.json.JSONException;
import org.json.JSONObject;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

import io.fabric.sdk.android.Fabric;


public class AppActivity extends AzoomeeActivity
{
    private static Context mContext;
    private static Activity mActivity;
    private MixpanelAPI mixpanel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mixpanel = MixpanelAPI.getInstance(this, "7e94d58938714fa180917f0f3c7de4c9");

        Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
        mContext = this;
        mActivity = this;
    }

    @Override
    public void onDestroy()
    {
        mixpanel.flush();
        super.onDestroy();
    }

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

//    //----- Appsflyer--------
//
//    public static void sendAppsFlyerEvent(String eventID, String jsonPropertiesString)
//    {
//        // Does nothing in standalone chat app for now
//    }

}
