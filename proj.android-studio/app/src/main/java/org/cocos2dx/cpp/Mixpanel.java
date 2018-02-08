package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import com.mixpanel.android.mpmetrics.MixpanelAPI;

import org.json.JSONException;
import org.json.JSONObject;


public class Mixpanel
{
    private static Context _context = null;
    private static final String kMixpanelAPIKey = "7e94d58938714fa180917f0f3c7de4c9";

    private MixpanelAPI _mixpanelAPI = null;

    public Mixpanel(Context context)
    {
        this._context = context;
        this._mixpanelAPI = MixpanelAPI.getInstance(_context , kMixpanelAPIKey);
    }


    public void sendMixPanelWithEventID(String eventID, String jsonPropertiesString)
    {
        JSONObject mixPanelProperties = null;

        try
        {
            mixPanelProperties = new JSONObject(jsonPropertiesString);
        }
        catch (JSONException e)
        {
            mixPanelProperties = null;
        }
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        _mixpanelAPI.track(eventID, mixPanelProperties);
    }

    public void sendMixPanelSuperProperties(String jsonPropertiesString)
    {
        JSONObject mixPanelProperties = null;

        try
        {
            mixPanelProperties = new JSONObject(jsonPropertiesString);
        }
        catch (JSONException e)
        {
            mixPanelProperties = null;
        }
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        _mixpanelAPI.registerSuperProperties(mixPanelProperties);
    }

    public void sendMixPanelPeopleProperties(String parentID)
    {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        _mixpanelAPI.identify(parentID);
        _mixpanelAPI.getPeople().identify(parentID);
        _mixpanelAPI.getPeople().set("parentID", parentID);
    }

    public void showMixpanelNotification()
    {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        _mixpanelAPI.getPeople().showNotificationIfAvailable((Activity) _context);
    }

    public void showMixpanelNotificationWithID(int notificationID)
    {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        _mixpanelAPI.getPeople().showNotificationById(notificationID, (Activity) _context);
    }

    public void flush()
    {
        if(!isMixpanelContextAvailable())
        {
            return;
        }
        _mixpanelAPI.flush();
    }

    private boolean isMixpanelContextAvailable()
    {
        if(_context == null)
        {
            return false;
        }

        if(_mixpanelAPI == null)
        {
            _mixpanelAPI = MixpanelAPI.getInstance(_context, kMixpanelAPIKey);
        }

        return true;
    }
}
