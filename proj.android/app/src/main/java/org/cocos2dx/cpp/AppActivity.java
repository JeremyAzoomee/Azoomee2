package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.util.Log;
import android.util.Base64;

import org.cocos2dx.cpp.util.IabBroadcastReceiver;
import org.cocos2dx.cpp.util.IabException;
import org.cocos2dx.cpp.util.IabHelper;
import org.cocos2dx.cpp.util.IabResult;
import org.cocos2dx.cpp.util.Inventory;
import org.cocos2dx.cpp.util.Purchase;

import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;

import com.tinizine.azoomee.R;
import com.tinizine.azoomee.BuildConfig;
import com.tinizine.azoomee.common.AzoomeeActivity;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

import com.amazon.device.iap.PurchasingService;
import com.amazon.device.iap.model.FulfillmentResult;
import com.amazon.device.iap.model.RequestId;
import com.amazon.iap.IapManager;
import com.amazon.iap.MySku;
import com.amazon.iap.PurchasingListenerClass;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.core.CrashlyticsCore;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import io.fabric.sdk.android.Fabric;

import com.urbanairship.UAirship;
import com.urbanairship.Autopilot;
import com.urbanairship.push.notifications.DefaultNotificationFactory;


public class AppActivity extends AzoomeeActivity implements IabBroadcastReceiver.IabBroadcastListener {

    private static boolean kRemoteDebugWebViewEnabled = false;

    private static Context mContext;
    private static Activity mActivity;
    private static AppActivity mAppActivity;
    private IapManager iapManager;
    private static String advertisingId;
    private Biometric biometric;
    private Mixpanel mixpanel;
    private Appsflyer appsflyer;

    private boolean _purchaseRequiredAfterSetup = false;
    private boolean _restorePurchase = false;

    //variables for google payment
    private IabHelper mHelper;
    private IabBroadcastReceiver mBroadcastReceiver;
    private boolean mIsPremium;
    private boolean IABHelperSetupComplete = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mContext = this;
        mActivity = this;
        mAppActivity = this;
        biometric = new Biometric(this);

        Autopilot.automaticTakeOff(this);
        mixpanel = new Mixpanel(this);
        appsflyer = new Appsflyer(this);

        // If mFrameLayout hasn't been created, then the activity is going to be destroyed
        // For context, see Cocos2dxActivity onCreate !isTaskRoot() workaround.
        if(mFrameLayout == null)
        {
            return;
        }

        appsflyer.startTracking(this.getApplication());

        // Set up Crashlytics, disabled for debug builds
        Crashlytics crashlyticsKit = new Crashlytics.Builder().core(new CrashlyticsCore.Builder().disabled(BuildConfig.DEBUG).build()).build();
        Fabric.with(this, crashlyticsKit, new CrashlyticsNdk());

        readAdvertisingIdFromDevice();

        //setting up urban airship push notification icons
        DefaultNotificationFactory defaultNotificationFactory = new DefaultNotificationFactory(getApplicationContext());
        defaultNotificationFactory.setSmallIconId(R.mipmap.ic_launcher);
        defaultNotificationFactory.setLargeIcon(R.mipmap.ic_launcher);
        defaultNotificationFactory.setColor(NotificationCompat.COLOR_DEFAULT);
        UAirship airship = UAirship.shared();
        airship.getPushManager().setNotificationFactory(defaultNotificationFactory);

        getGLSurfaceView().setMultipleTouchEnabled(false);
    }

    public static void startWebView(String url, String userid, int orientation, float closeButtonAnchorX, float closeButtonAnchorY, int videoProgressSeconds) {
        Intent nvw;
        if (url.substring(url.length() - 4).equals("m3u8"))
        {
            nvw = new Intent(mContext, NativeMediaPlayer.class);
        }
        else
        {
            nvw = new Intent(mContext, NativeViewUI.class);
            nvw.putExtra("remoteDebugWebViewEnabled", kRemoteDebugWebViewEnabled);
        }

        nvw.putExtra("url", url);
        nvw.putExtra("userid", userid);
        nvw.putExtra("orientation", orientation);
        nvw.putExtra("closeAnchorX", closeButtonAnchorX);
        nvw.putExtra("closeAnchorY", closeButtonAnchorY);
        nvw.putExtra("videoProgressSeconds", videoProgressSeconds);

        mContext.startActivity(nvw);

    }

    private void readAdvertisingIdFromDevice()
    {
        Thread thread = new Thread() {
            @Override
            public void run() {
                try {
                    AdvertisingIdClient.Info adInfo = AdvertisingIdClient.getAdvertisingIdInfo(getApplicationContext());
                    advertisingId = adInfo != null ? adInfo.getId() : null;
                } catch (IOException | GooglePlayServicesRepairableException | GooglePlayServicesNotAvailableException exception) {
                    exception.printStackTrace();
                }
            }
        };

        // call thread start for background process
        thread.start();
    }

    public static void startAndroidReviewProcess()
    {
        final Uri uri = Uri.parse("market://details?id=" + mContext.getApplicationContext().getPackageName());
        final Intent rateAppIntent = new Intent(Intent.ACTION_VIEW, uri);

        if (mContext.getPackageManager().queryIntentActivities(rateAppIntent, 0).size() > 0)
        {
            mContext.startActivity(rateAppIntent);
        }
        else
        {
    /* handle your error case: the device has no way to handle market urls */
        }
    }

    public static String getOSBuildManufacturer() {
        return android.os.Build.MANUFACTURER;
    }

    public static String getAndroidDeviceModel() {
        return android.os.Build.DEVICE;
    }

    public static String getAndroidDeviceAdvertisingId() { //AAID must not be read on the main thread, so it's being read during onCreate, and now the result is being returned.
        if(advertisingId == null || advertisingId.equals("")) return "NA";
        else return advertisingId;
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

    public static String getDeviceLanguage()
    {
        return Locale.getDefault().getLanguage();
    }


    public static void CrashlyticsLogException(String message) {
        Crashlytics.logException(new Exception(message));
    }

    private static void CrashlyticsLogUser(String AdultIdentifier, String ChildIdentifier) {
        Crashlytics.setUserIdentifier(AdultIdentifier);
        Crashlytics.setUserName(ChildIdentifier);
    }

    private static void CrashlyticsKeyWithString(String key, String dataString) {
        Crashlytics.setString(key,dataString);
    }

    //----Mix Panel------

    public static void identifyMixpanel()
    {
        if(!getAndroidDeviceAdvertisingId().equals(""))
        {
            mAppActivity.mixpanel.identifyMixpanelWithId(getAndroidDeviceAdvertisingId());
        }
    }

    public static void sendMixPanelWithEventID(String eventID, String jsonPropertiesString) {
        if(mAppActivity.mixpanel == null)
        {
            return;
        }

        mAppActivity.mixpanel.sendMixPanelWithEventID(eventID, jsonPropertiesString);
    }

    public static void sendMixPanelSuperProperties(String jsonPropertiesString) {
        if(mAppActivity.mixpanel == null)
        {
            return;
        }

        mAppActivity.mixpanel.sendMixPanelSuperProperties(jsonPropertiesString);
    }

    public static void sendMixPanelPeopleProperties(String parentID) {
        if(mAppActivity.mixpanel == null)
        {
            return;
        }

        mAppActivity.mixpanel.sendMixPanelPeopleProperties(parentID);
    }

    public static void setMixpanelAlias(String newID) {
        if(mAppActivity.mixpanel == null)
        {
            return;
        }

        mAppActivity.mixpanel.setMixpanelAlias(newID);
    }

    public static void updateMixpanelPeopleProperties(String jsonPropertiesString) {
        if(mAppActivity.mixpanel == null)
        {
            return;
        }

        mAppActivity.mixpanel.updateMixpanelPeopleProperties(jsonPropertiesString);
    }

    public static void showMixpanelNotification() {
        if(mAppActivity.mixpanel == null)
        {
            return;
        }

        mAppActivity.mixpanel.showMixpanelNotification();
    }

    public static void showMixpanelNotificationWithID(int notificationID) {
        if(mAppActivity.mixpanel == null)
        {
            return;
        }

        mAppActivity.mixpanel.showMixpanelNotificationWithID(notificationID);
    }

    @Override
    public void onDestroy() {
        if(mixpanel != null)
        {
            mixpanel.flush();
        }
        
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

        if(mAppActivity.appsflyer == null)
        {
            return;
        }
        mAppActivity.appsflyer.sendAppsFlyerEvent(eventID, jsonPropertiesString);
    }


    //------IN-APP-PURCHASES COMMON-------------------------------
    public static void setupInAppPurchase()
    {
        mAppActivity._purchaseRequiredAfterSetup = false;

        if (android.os.Build.MANUFACTURER.equals("Amazon"))
        {
            mAppActivity.setupIAPOnCreate();
        }
        else
        {
            mAppActivity.setupGoogleIAB();
        }
    }

    public static native void setHumanReadablePrice(String price);
    public static native void priceFetchFailed();



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
        final RequestId requestId = PurchasingService.purchase(getAmazonSku());
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

    public static native String getAmazonSku();

//-------------------------------GOOGLE IAB----------------------------------------------

    public static void startGooglePurchase() {
        AppActivity currentActivity = mAppActivity;

        if(currentActivity.mHelper == null)
        {
            currentActivity._purchaseRequiredAfterSetup = true;
            currentActivity.setupGoogleIAB();
        }
        else
        {
            currentActivity.startGoogleSubscriptionProcess();
        }
    }

    public static void startGoogleRestorePurchase()
    {
        AppActivity currentActivity = mAppActivity;
        currentActivity._restorePurchase = true;
        if(currentActivity.mHelper == null)
        {
            currentActivity.setupGoogleIAB();
        }
        else
        {
            try {
                String googleSku = getGoogleSku();
                String[] moreSkus = {googleSku, "SKU_ITEMONE"}; //fake skus required by queryInventoryAsync...
                String[] moreSubSkus = {googleSku, "SUB_ITEMONE", "SUB_ITEMTWO"}; //fake skus required by queryInventoryAsync...
                Inventory inv = currentActivity.mHelper.queryInventory(true, Arrays.asList(moreSkus), Arrays.asList(moreSubSkus));
                // Do we have the premium upgrade?
                Purchase premiumPurchase = inv.getPurchase(googleSku);
                currentActivity.mIsPremium = (premiumPurchase != null);
                Log.d("GOOGLEPLAY", "User is " + (currentActivity.mIsPremium ? "PREMIUM" : "NOT PREMIUM"));

                if(currentActivity._restorePurchase)
                {
                    currentActivity._restorePurchase = false;
                    if(currentActivity.mIsPremium)
                    {
                        googlePurchaseHappened(premiumPurchase.getDeveloperPayload(), premiumPurchase.getOrderId(), premiumPurchase.getToken());
                    }
                    else
                    {
                        googleNoPurchaseFound();
                    }
                }
            } catch (IabException e) {
                Log.d("GOOGLEPAY", "Error querying inventory.");
                googlePurchaseFailed();
            }
        }
    }


    public void setupGoogleIAB() {
        String base64EncodedPublicKey = getDeveloperKey();

        Log.d("GOOGLEPAY", "Key: " + base64EncodedPublicKey);

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
                    mBroadcastReceiver = new IabBroadcastReceiver(mAppActivity);
                    IntentFilter broadcastFilter = new IntentFilter(IabBroadcastReceiver.ACTION);
                    registerReceiver(mBroadcastReceiver, broadcastFilter);

                    if(!mHelper.mSetupDone)
                    {
                        googlePurchaseFailed();
                        return;
                    }

                    String[] moreSkus = {getGoogleSku(), "SKU_ITEMONE"}; //fake skus required by queryInventoryAsync...
                    String[] moreSubSkus = {getGoogleSku(), "SUB_ITEMONE", "SUB_ITEMTWO"}; //fake skus required by queryInventoryAsync...
                    mHelper.queryInventoryAsync(true, Arrays.asList(moreSkus), Arrays.asList(moreSubSkus), mGotInventoryListener);
                } catch (IabHelper.IabAsyncInProgressException e) {
                    Log.d("GOOGLEPAY", "Error querying inventory. Another async operation in progress.");
                }
            }
        });

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
                priceFetchFailed();
                return;
            }

            Log.d("GOOGLEPLAY", "Query inventory was successful.");
            Log.d("GOOGLEPLAY", "Google SKU: " + getGoogleSku());

            if(inventory.hasDetails(getGoogleSku()))
            {
                String price = inventory.getSkuDetails(getGoogleSku()).getPrice();

                if(price != null || price != "")
                {
                    setHumanReadablePrice(inventory.getSkuDetails(getGoogleSku()).getPrice());
                }
                else
                {
                    priceFetchFailed();
                }
            }

            // Do we have the premium upgrade?
            Purchase premiumPurchase = inventory.getPurchase(getGoogleSku());
            mIsPremium = (premiumPurchase != null);
            Log.d("GOOGLEPLAY", "User is " + (mIsPremium ? "PREMIUM" : "NOT PREMIUM"));

            IABHelperSetupComplete = true;

            if(_restorePurchase)
            {
                _restorePurchase = false;
                if(mIsPremium)
                {
                    googlePurchaseHappened(premiumPurchase.getDeveloperPayload(), premiumPurchase.getOrderId(), premiumPurchase.getToken());
                }
                else
                {
                    googleNoPurchaseFound();
                }
            }
            else if(_purchaseRequiredAfterSetup)
            {
                startGoogleSubscriptionProcess();
            }
        }
    };

    public void startGoogleSubscriptionProcess() {
        _purchaseRequiredAfterSetup = false;

        if(!IABHelperSetupComplete)
        {
            googlePurchaseFailed();
            return;
        }

        if(mIsPremium)
        {
            googlePurchaseFailedAlreadyPurchased();
            return;
        }

        if(mHelper == null) //mHelper got disposed in the meantime
        {
            googlePurchaseFailed();
            return;
        }

        try
        {

            String userId = getLoggedInParentUserId();

            if(userId.equals(""))
            {
                userId = Long.toHexString(Double.doubleToLongBits(Math.random()));
            }


            mHelper.launchSubscriptionPurchaseFlow(mActivity, getGoogleSku(), 10001,
                    mPurchaseFinishedListener, userId);
        }
        catch (IabHelper.IabAsyncInProgressException e)
        {
            Log.d("GOOGLEPAY", "Error launching purchase flow. Another async operation in progress.");
            googlePurchaseFailed();
        }
    }

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
                if(result.getResponse() == IabHelper.IABHELPER_USER_CANCELLED)
                {
                    googlePurchaseCancelled();
                }
                else {
                    googlePurchaseFailed();
                }
                return;
            }

            Log.d("GOOGLEPAY", "Purchase successful.");
            googlePurchaseHappened(purchase.getDeveloperPayload(), purchase.getOrderId(), purchase.getToken());
        }
    };

    public static native void googlePurchaseHappened(String developerPayload, String orderId, String token);

    public static native void googlePurchaseFailed();

    public static native void googleNoPurchaseFound();

    public static native void googlePurchaseCancelled();

    public static native void googlePurchaseFailedAlreadyPurchased();

    public static native String getGoogleSku();

    public static native String getLoggedInParentUserId();

    public static native String getDeveloperKey();


    //--------------------- SHARE ACTION PROVIDER --------------------

    public static void shareActionProviderWithString(String textToShare) {
        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        shareIntent.setType("text/plain");
        shareIntent.putExtra(Intent.EXTRA_TEXT, textToShare);

        mActivity.startActivity(shareIntent);
    }

    // PUSH NOTIFICATIONS NATIVE ANDRODID FUNCTIONS

    public static void jniSetNamedUserIdentifierForPushChannel(String channelName)
    {
        UAirship.shared().getNamedUser().setId(channelName);
        UAirship.shared().getPushManager().setChannelTagRegistrationEnabled(false);
    }

    public static void jniSetTagForPushChannel(String tagGroup, String tag)
    {
        UAirship.shared().getNamedUser().editTagGroups().setTag(tagGroup, tag);
        UAirship.shared().getNamedUser().editTagGroups().apply();
    }

    public static void jniEnablePushNotifications()
    {
        UAirship.shared().getPushManager().setUserNotificationsEnabled(true);
        UAirship.shared().getInAppMessageManager().setAutoDisplayEnabled(false);
        UAirship.shared().getInAppMessageManager().setDisplayAsapEnabled(false);
    }

    public static void jniDisablePushNotifications()
    {
        UAirship.shared().getPushManager().setUserNotificationsEnabled(false);
        UAirship.shared().getInAppMessageManager().setAutoDisplayEnabled(false);
    }

    public static void jniClearNotificationCenter()
    {
        NotificationManagerCompat.from(mContext).cancelAll();
    }

    // FINGERPRINT NATIVE ANDROID FUNCTIONS------------------------------

    public static boolean fingerPrintAuthenticationAvailable()
    {
        if(mAppActivity.biometric == null)
        {
            return false;
        }

        return mAppActivity.biometric.fingerprintAuthenticationPossible();
    }

    public static void startFingerprintAuthentication()
    {
        if(mAppActivity.biometric == null)
        {
            return;
        }

        mAppActivity.biometric.startAuth();
    }

    public static void stopFingerprintAuthentication()
    {
        if(mAppActivity.biometric == null)
        {
            return;
        }

        mAppActivity.biometric.stopAuth();
    }
}
