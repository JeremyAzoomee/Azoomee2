package org.cocos2dx.cpp;

import android.content.Context;
import android.util.Log;
import com.mixpanel.android.mpmetrics.MixpanelAPI;
import org.json.JSONObject;
import org.json.JSONException;

public class mixPanelProperties {

    private JSONObject _mixPanelProperties;

    public mixPanelProperties()
    {
        _mixPanelProperties = new JSONObject();
    }

    public void addToMixPanelProperties(String key, String property)
    {
        try {
            _mixPanelProperties.put(key, property);
        }catch(JSONException e) {
            Log.e(key, "MIXPANEL: Unable to add properties to JSONObject", e);
        }
    }

    public void print()
    {
        Log.d("MIXPANEL", "Properties Sent From Cocos");
    }

    public JSONObject getProperties()
    {
        return _mixPanelProperties;
    }
}
