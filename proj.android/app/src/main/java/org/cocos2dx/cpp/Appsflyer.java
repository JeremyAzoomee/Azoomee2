package org.cocos2dx.cpp;

import android.app.Application;
import android.content.Context;
import com.appsflyer.AppsFlyerLib;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

public class Appsflyer
{

    private static Context _context = null;
    private static final String kAppsFlyerAPIKey = "BzPYMg8dkYsCuDn8XBUN94";
    private AppsFlyerLib _appsFlyer = null;

    public Appsflyer(Context context)
    {
        _context = context;
        _appsFlyer = AppsFlyerLib.getInstance();
    }

    public void startTracking(Application app)
    {
        _appsFlyer.startTracking(app, kAppsFlyerAPIKey);
    }

    public void sendAppsFlyerEvent(String eventID, String jsonPropertiesString)
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

        if (mixPanelProperties != null)
        {
            Map<String, Object> appsFlyerProperties = new HashMap<String, Object>();
            java.util.Iterator<?> keys = mixPanelProperties.keys();

            while (keys.hasNext())
            {
                String key = (String) keys.next();

                java.lang.Object properties = null;

                try
                {
                    properties = mixPanelProperties.get(key);
                }
                catch (JSONException e)
                {
                }

                if (properties != null)
                {
                    appsFlyerProperties.put(key, properties);
                }
            }
            _appsFlyer.trackEvent(_context, eventID, appsFlyerProperties);
        }
        else
        {
            _appsFlyer.trackEvent(_context, eventID, null);
        }
    }

}
