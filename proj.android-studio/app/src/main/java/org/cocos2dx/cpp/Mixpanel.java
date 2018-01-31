package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import com.mixpanel.android.mpmetrics.MixpanelAPI;

import org.json.JSONException;
import org.json.JSONObject;


public class Mixpanel {
    private static Context mContext = null;
    private static String mMixpanelAPIKey = "7e94d58938714fa180917f0f3c7de4c9";

    private MixpanelAPI mMixpanelAPI = null;

    public Mixpanel(Context context) {
        this.mContext = context;
        this.mMixpanelAPI = MixpanelAPI.getInstance(mContext , mMixpanelAPIKey);
    }


    public void sendMixPanelWithEventID(String eventID, String jsonPropertiesString) {
        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        } catch (JSONException e) {
            _mixPanelProperties = null;
        }
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        mMixpanelAPI.track(eventID, _mixPanelProperties);
    }

    public void sendMixPanelSuperProperties(String jsonPropertiesString) {
        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        } catch (JSONException e) {
            _mixPanelProperties = null;
        }
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        mMixpanelAPI.registerSuperProperties(_mixPanelProperties);
    }

    public void sendMixPanelPeopleProperties(String parentID) {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        mMixpanelAPI.identify(parentID);
        mMixpanelAPI.getPeople().identify(parentID);
        mMixpanelAPI.getPeople().set("parentID", parentID);
    }

    public void showMixpanelNotification() {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        mMixpanelAPI.getPeople().showNotificationIfAvailable((Activity) mContext);
    }

    public void showMixpanelNotificationWithID(int notificationID) {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        mMixpanelAPI.getPeople().showNotificationById(notificationID, (Activity) mContext);
    }

    public void flush()
    {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        mMixpanelAPI.flush();
    }

    private boolean isMixpanelContextAvailable()
    {
        if(mContext != null)
        {
            if(mMixpanelAPI == null)
            {
                mMixpanelAPI = MixpanelAPI.getInstance(mContext, mMixpanelAPIKey);
                return true;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
}
