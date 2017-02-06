/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 C
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import android.util.Base64;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;
import org.xwalk.core.XWalkCookieManager;
import org.xwalk.core.XWalkView;

import java.util.HashMap;
import java.util.Map;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import io.fabric.sdk.android.Fabric;

import com.mixpanel.android.mpmetrics.MixpanelAPI;

public class AppActivity extends Cocos2dxActivity {

    private static Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        MixpanelAPI mixpanel = MixpanelAPI.getInstance(this, "7e94d58938714fa180917f0f3c7de4c9");

        Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
        mContext = this;

    }

    public static void startWebView(String url, String cookieurl, String cookie, String userid) {

        Log.d("sent from cocos", url + " - " + cookieurl + " - " + cookie);

        Intent nvw = new Intent(mContext, NativeView.class);
        nvw.putExtra("url", url);
        nvw.putExtra("cookieurl", cookieurl);
        nvw.putExtra("cookie", cookie);
        nvw.putExtra("userid", userid);
        mContext.startActivity(nvw);

        //Intent i = new Intent(getApplicationContext(), NativeView.class);
        //startActivity(i);

    }

    public static String getAnswer()
    {
        return "AndroidAnswer";
    }
    
    public static String getHMACSHA256(String message, String secret) {
        String hash = "";
        try {
            Mac sha256_HMAC = Mac.getInstance("HmacSHA256");
            SecretKeySpec secret_key = new SecretKeySpec(secret.getBytes(), "HmacSHA256");
            sha256_HMAC.init(secret_key);

            hash = Base64.encodeToString(sha256_HMAC.doFinal(message.getBytes()), Base64.DEFAULT);
        } catch (Exception e) {

        }

        return hash.trim();

    }

    public static void CrashlyticsLogException(String message) {
        Crashlytics.logException(new Exception(message));
    }

    private static void CrashlyticsLogUser(String AdultIdentifier, String ChildIdentifier) {
        Crashlytics.setUserIdentifier(AdultIdentifier);
        Crashlytics.setUserName(ChildIdentifier);
    }

    public void sendMixPanelWithEventID(String eventID, mixPanelProperties propertiesObject)
    {
        MixpanelAPI mixpanel = MixpanelAPI.getInstance(this, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.track(eventID, propertiesObject.getProperties());
    }

}
