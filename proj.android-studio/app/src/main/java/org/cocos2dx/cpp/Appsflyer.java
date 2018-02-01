package org.cocos2dx.cpp;

import android.app.Application;
import android.content.Context;
import com.appsflyer.AppsFlyerLib;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

public class Appsflyer {

    private static Context mContext = null;
    private static String mAppsFlyerAPIKey = "BzPYMg8dkYsCuDn8XBUN94";
    private AppsFlyerLib mAppsFlyer = null;

    public Appsflyer(Context context)
    {
        mContext = context;
        mAppsFlyer = AppsFlyerLib.getInstance();
    }

    public void startTracking(Application app)
    {
        mAppsFlyer.startTracking(app, mAppsFlyerAPIKey);
    }

    public void sendAppsFlyerEvent(String eventID, String jsonPropertiesString) {

        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        } catch (JSONException e) {
            _mixPanelProperties = null;
        }

        if (_mixPanelProperties != null) {
            Map<String, Object> _appsFlyerProperties = new HashMap<String, Object>();
            java.util.Iterator<?> keys = _mixPanelProperties.keys();

            while (keys.hasNext()) {
                String key = (String) keys.next();

                java.lang.Object properties = null;

                try {
                    properties = _mixPanelProperties.get(key);
                } catch (JSONException e) {
                }

                if (properties != null) {
                    _appsFlyerProperties.put(key, properties);
                }
            }
            mAppsFlyer.trackEvent(mContext, eventID, _appsFlyerProperties);
        } else
            mAppsFlyer.trackEvent(mContext, eventID, null);
    }

}
