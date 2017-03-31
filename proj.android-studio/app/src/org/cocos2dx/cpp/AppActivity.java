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

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import android.util.Base64;

import org.cocos2dx.cpp.util.IabBroadcastReceiver;
import org.cocos2dx.cpp.util.IabHelper;
import org.cocos2dx.cpp.util.IabResult;
import org.cocos2dx.cpp.util.Inventory;
import org.cocos2dx.cpp.util.Purchase;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
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

public class AppActivity extends Cocos2dxActivity implements IabBroadcastReceiver.IabBroadcastListener {

    private static Context mContext;
    private static Activity mActivity;
    private static AppActivity mAppActivity;
    private MixpanelAPI mixpanel;
    private IapManager iapManager;

    //variables for google payment
    private IabHelper mHelper;
    private IabBroadcastReceiver mBroadcastReceiver;
    private boolean mIsPremium;
    String payload = "ASDFAGASDFXCYVFASDFASREWRQWER";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mContext = this;
        mActivity = this;
        mAppActivity = this;

        if (getOSBuildManufacturer() == "AMAZON")
        {
            setupIAPOnCreate();
        }

        AppsFlyerLib.getInstance().startTracking(this.getApplication(), "BzPYMg8dkYsCuDn8XBUN94");
        mixpanel = MixpanelAPI.getInstance(this, "7e94d58938714fa180917f0f3c7de4c9");

        Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
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

    public static String getAnswer() {
        return "AndroidAnswer";
    }

    public static String getOSBuildManufacturer() {
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

    public static void sendMixPanelWithEventID(String eventID, String jsonPropertiesString) {
        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        } catch (JSONException e) {
            _mixPanelProperties = null;
        }

        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.track(eventID, _mixPanelProperties);
    }

    public static void sendMixPanelSuperProperties(String jsonPropertiesString) {
        JSONObject _mixPanelProperties = null;

        try {
            _mixPanelProperties = new JSONObject(jsonPropertiesString);
        } catch (JSONException e) {
            _mixPanelProperties = null;
        }

        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.registerSuperProperties(_mixPanelProperties);
    }

    public static void sendMixPanelPeopleProperties(String parentID) {
        MixpanelAPI mixpanel = MixpanelAPI.getInstance(mContext, "7e94d58938714fa180917f0f3c7de4c9");
        mixpanel.identify(parentID);
        mixpanel.getPeople().identify(parentID);
        mixpanel.getPeople().set("First Name", parentID);
    }

    @Override
    protected void onDestroy() {
        mixpanel.flush();

        if (mBroadcastReceiver != null) {
            unregisterReceiver(mBroadcastReceiver);
        }

        if (mHelper != null) {
            mHelper.disposeWhenFinished();
            mHelper = null;
        }

        super.onDestroy();
    }

    //----- Appsflyer--------

    public static void sendAppsFlyerEvent(String eventID, String jsonPropertiesString) {

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
            AppsFlyerLib.getInstance().trackEvent(mContext, eventID, _appsFlyerProperties);
        } else
            AppsFlyerLib.getInstance().trackEvent(mContext, eventID, null);
    }

    //----- AMAZON IAP -------------------------------------------

    private void setupIAPOnCreate() {
        if (android.os.Build.MANUFACTURER.equals("Amazon")) {
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

    public static void startAmazonPurchase() {
        final RequestId requestId = PurchasingService.purchase("com.azoomee.premium.monthly");
        Log.d("IAPAPI", "Request id: " + requestId.toString());
        Log.d("IAPAPI", "purchase service started, app on pause");
    }

    public static void fulfillAmazonPurchase(String receiptId) {
        PurchasingService.notifyFulfillment(receiptId, FulfillmentResult.FULFILLED);
    }

    public void setReceiptId(String sentReceiptId) {
        receiptId = sentReceiptId;
    }

    public void setRequestId(String sentRequestId) {
        requestId = sentRequestId;
    }

    public void setAmazonUserid(String sentAmazonUserid) {
        amazonUserid = sentAmazonUserid;
    }

    public void sendCollectedDataToCocos() {
        Log.d("purchase data:", "purchase happened is called. requestid: " + requestId + " receiptid: " + receiptId + " amazonUserid: " + amazonUserid);
        purchaseHappened(requestId, receiptId, amazonUserid);
    }

    public void sendIAPFAILToCocos() {
        purchaseFailed();
    }

    public void sendUserDataFAILToCocos() {
        userDataFailed();
    }

    public void amazonAlreadyPurchased() {
        alreadyPurchased();
    }

    public static native void purchaseHappened(String requestId, String receiptId, String amazonUserid);

    public static native void purchaseFailed();

    public static native void userDataFailed();

    public static native void alreadyPurchased();

//-------------------------------GOOGLE IAB----------------------------------------------

    public static void startGooglePurchase()
    {
        AppActivity currentActivity = mAppActivity;
        currentActivity.setupGoogleIAB();
    }

    public void setupGoogleIAB() {
        String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAyugVV2DZkSQShZYR+Zk6N8XTqUdtgJhNLPOOXAjmiXWuMV6Vq9/3wYrWBDiFwzZMAipoQWmsCUNIaC9b7FVJ8pwSSTpfH4VfqADdHJxHSM6VeaE5ZiT/2yWwNORFiibf6tEmYD3ikA6j1OGpkGUT4E3UsSRh+mx0jRqNHXEgT0iOblPaaP4FPiuimtBWJgqSn0oO9va+hF8GzOtWnEWlBkft/Yri7mY/Z9OhmIrFGTfdzSiAHa5W3gDPpT5SoRMwz2RVcSgpQPBo4uhtSBmVT/AJfWf3U5vYmnOIbjPjFaZ4T5YHHCdoKY9DeFaQBn/w98Qc6eMujFKDkGGNOGMZMQIDAQAB";
        mHelper = new IabHelper(this, base64EncodedPublicKey);

        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
                if (!result.isSuccess()) {
                    Log.d("GOOGLEPAY", "Problem setting up In-app Billing: " + result);
                    return;
                }

                Log.d("GOOGLEPAY", "INITIALISED");

                Log.d("GOOGLEPAY", "Setup successful. Querying inventory.");
                try {
                    mHelper.queryInventoryAsync(mGotInventoryListener);
                } catch (IabHelper.IabAsyncInProgressException e) {
                    Log.d("GOOGLEPAY", "Error querying inventory. Another async operation in progress.");
                }
            }
        });

        mBroadcastReceiver = new IabBroadcastReceiver(this);
        IntentFilter broadcastFilter = new IntentFilter(IabBroadcastReceiver.ACTION);
        registerReceiver(mBroadcastReceiver, broadcastFilter);

    }

    @Override
    public void receivedBroadcast() {
        // Received a broadcast notification that the inventory of items has changed
        Log.d("GOOGLEPAY", "Received broadcast notification. Querying inventory.");

        try {
            mHelper.queryInventoryAsync(mGotInventoryListener);
        } catch (IabHelper.IabAsyncInProgressException e) {
            Log.d("GOOGLEPAY", "Error querying inventory. Another async operation in progress.");
        }

    }

    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            Log.d("GOOGLEPLAY", "Query inventory finished.");

            // Have we been disposed of in the meantime? If so, quit.
            if (mHelper == null) return;

            // Is it a failure?
            if (result.isFailure()) {
                Log.d("GOOGLEPLAY", "Failed to query inventory: " + result);
                return;
            }

            Log.d("GOOGLEPLAY", "Query inventory was successful.");

            // Do we have the premium upgrade?
            Purchase premiumPurchase = inventory.getPurchase("com.tinizine.azoomee.monthly");
            mIsPremium = (premiumPurchase != null);
            Log.d("GOOGLEPLAY", "User is " + (mIsPremium ? "PREMIUM" : "NOT PREMIUM"));

            //If the user is not premium, we can start the upgrade process

            try {
                mHelper.launchPurchaseFlow(mActivity, "com.tinizine.azoomee.monthly", 10001,
                        mPurchaseFinishedListener, payload);
            } catch (IabHelper.IabAsyncInProgressException e) {
                Log.d("GOOGLEPAY", "Error launching purchase flow. Another async operation in progress.");
            }
        }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d("GOOGLEPAY", "onActivityResult(" + requestCode + "," + resultCode + "," + data);
        if (mHelper == null) return;

        // Pass on the activity result to the helper for handling
        if (!mHelper.handleActivityResult(requestCode, resultCode, data)) {
            // not handled, so handle it ourselves (here's where you'd
            // perform any handling of activity results not related to in-app
            // billing...
            super.onActivityResult(requestCode, resultCode, data);
        } else {
            Log.d("GOOGLEPAY", "onActivityResult handled by IABUtil.");
        }
    }

    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.d("GOOGLEPAY", "Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            if (result.isFailure()) {
                Log.d("GOOGLEPAY", "Error purchasing: " + result);
                googlePurchaseFailed();
                return;
            }

            Log.d("GOOGLEPAY", "Purchase successful.");
            googlePurchaseHappened(purchase.getDeveloperPayload(), purchase.getOrderId(), purchase.getToken());

            //purchase.getDeveloperPayload();             //developer specified string - possible to check if the same at the end of the purchase - possible to use this as the user id, possibly encoded?
            //purchase.getOrderId();                      //google payments order id, in sandbox it is 0
            //purchase.getSignature();                    //signature of the purchase data, signed with dev private key. RSASSA-PKCS1-v1_5 scheme.
            //purchase.getToken();                        //unique identifier based on the item and the user
            //purchase.getPurchaseState();                //0: purchased, 1: cancelled, 2: refunded
            //result.getResponse();                       //0: success, error otherwise
            //result.getMessage();
            //result.isFailure();
        }
    };

    public static native void googlePurchaseHappened(String developerPayload, String orderId, String token);

    public static native void googlePurchaseFailed();

    public static native void googleAlreadyPurchased();

}