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

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import com.amazon.device.iap.PurchasingService;
import com.amazon.device.iap.model.FulfillmentResult;
import com.amazon.device.iap.model.RequestId;
import com.amazon.iap.IapManager;
import com.amazon.iap.MySku;
import com.amazon.iap.PurchasingListenerClass;
import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import io.fabric.sdk.android.Fabric;

import com.mixpanel.android.mpmetrics.MixpanelAPI;

import com.appsflyer.AppsFlyerLib;


public class AppActivity extends Cocos2dxActivity {

    private static Context mContext;
    private MixpanelAPI mixpanel;
    private IapManager iapManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setupIAPOnCreate();
        AppsFlyerLib.getInstance().startTracking(this.getApplication(),"BzPYMg8dkYsCuDn8XBUN94");

        mixpanel = MixpanelAPI.getInstance(this, "7e94d58938714fa180917f0f3c7de4c9");

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

    public static String getOSBuildManufacturer()
    {
        return android.os.Build.MANUFACTURER;
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

    //----Mix Panel------

    public static void sendMixPanelWithEventID(String eventID, String jsonPropertiesString)
    {
        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        }catch(JSONException e) {
            _mixPanelProperties = null;
        }

        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.track(eventID, _mixPanelProperties);
    }

    public static void sendMixPanelSuperProperties(String jsonPropertiesString)
    {
        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        }catch(JSONException e) {
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
        mixpanel.getPeople().set("First Name", parentID);
    }

    @Override
    protected void onDestroy() {
        mixpanel.flush();
        super.onDestroy();
    }

    //----- Appsflyer--------

    public static void sendAppsFlyerEvent(String eventID, String jsonPropertiesString) {

        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        }catch(JSONException e) {
            _mixPanelProperties = null;
        }

        if(_mixPanelProperties != null) {
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
            AppsFlyerLib.getInstance().trackEvent(mContext, eventID, _appsFlyerProperties);
        }
        else
            AppsFlyerLib.getInstance().trackEvent(mContext, eventID, null);
    }

    //----- AMAZON IAP -------------------------------------------

    private void setupIAPOnCreate()
    {
        if(android.os.Build.MANUFACTURER.equals("Amazon")) {
            iapManager = new IapManager(this);

            final PurchasingListenerClass purchasingListener = new PurchasingListenerClass(iapManager);
            purchasingListener.setMainActivity(this);

            PurchasingService.registerListener(this.getApplicationContext(), purchasingListener);

            PurchasingService.getUserData();
            PurchasingService.getPurchaseUpdates(false);

            final Set<String> productSkus = new HashSet<String>();
            for (final MySku mySku : MySku.values()) {
                productSkus.add(mySku.getSku());
            }

            PurchasingService.getProductData(productSkus);
        }
    }

    public String receiptId;
    private String amazonUserid;
    private String requestId;

    public static void startAmazonPurchase()
    {
        final RequestId requestId = PurchasingService.purchase("com.tinizine.azoomee.monthly.02");
        Log.d("IAPAPI", "Request id: " + requestId.toString());
        Log.d("IAPAPI", "purchase service started, app on pause");
    }

    public static void fulfillAmazonPurchase(String receiptId)
    {
        PurchasingService.notifyFulfillment(receiptId, FulfillmentResult.FULFILLED);
    }

    public void setReceiptId(String sentReceiptId)
    {
        receiptId = sentReceiptId;
    }

    public void setRequestId(String sentRequestId)
    {
        requestId = sentRequestId;
    }

    public void setAmazonUserid(String sentAmazonUserid)
    {
        amazonUserid = sentAmazonUserid;
    }

    public void sendCollectedDataToCocos()
    {
        Log.d("purchase data:", "purchase happened is called. requestid: " + requestId + " receiptid: " + receiptId + " amazonUserid: " + amazonUserid);
        purchaseHappened(requestId, receiptId, amazonUserid);
    }

    public void sendIAPFAILToCocos() { purchaseFailed(); }

    public void amazonAlreadyPurchased()
    {
        alreadyPurchased();
    }

    public static native void purchaseHappened(String requestId, String receiptId, String amazonUserid);

    public static native void purchaseFailed();

    public static native void alreadyPurchased();
}