package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;

import com.tinizine.azoomee.R;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.HashMap;
import java.util.Map;

import org.cocos2dx.cpp.util.IabBroadcastReceiver;
import org.cocos2dx.cpp.util.IabHelper;
import org.cocos2dx.cpp.util.IabResult;
import org.cocos2dx.cpp.util.Inventory;
import org.cocos2dx.cpp.util.Purchase;
import org.xwalk.core.XWalkActivity;
import org.xwalk.core.XWalkView;
import org.xwalk.core.XWalkCookieManager;


import static com.loopj.android.http.AsyncHttpClient.log;

public class IabPurchaseManager extends Activity implements IabBroadcastReceiver.IabBroadcastListener {

    private IabHelper mHelper;
    private IabBroadcastReceiver mBroadcastReceiver;
    private boolean mIsPremium;
    String payload = "ASDFAGASDFXCYVFASDFASREWRQWER";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setupGoogleIABOnCreate(this);
    }

//-------------------------------GOOGLE IAB----------------------------------------------

    public void startInit(Context mContext)
    {
        setupGoogleIABOnCreate(mContext);
    }

//-------------------------------ALL METHODS ARE PRIVATE BEYOND THIS LINE-----------------

    private void setupGoogleIABOnCreate(Context mContext)
    {
        String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAyugVV2DZkSQShZYR+Zk6N8XTqUdtgJhNLPOOXAjmiXWuMV6Vq9/3wYrWBDiFwzZMAipoQWmsCUNIaC9b7FVJ8pwSSTpfH4VfqADdHJxHSM6VeaE5ZiT/2yWwNORFiibf6tEmYD3ikA6j1OGpkGUT4E3UsSRh+mx0jRqNHXEgT0iOblPaaP4FPiuimtBWJgqSn0oO9va+hF8GzOtWnEWlBkft/Yri7mY/Z9OhmIrFGTfdzSiAHa5W3gDPpT5SoRMwz2RVcSgpQPBo4uhtSBmVT/AJfWf3U5vYmnOIbjPjFaZ4T5YHHCdoKY9DeFaQBn/w98Qc6eMujFKDkGGNOGMZMQIDAQAB";
        mHelper = new IabHelper(this, base64EncodedPublicKey);

        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
                if (!result.isSuccess())
                {
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
                mHelper.launchPurchaseFlow(IabPurchaseManager.this, "com.tinizine.azoomee.monthly", 10001,
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
        }
        else {
            Log.d("GOOGLEPAY", "onActivityResult handled by IABUtil.");
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (mBroadcastReceiver != null) {
            unregisterReceiver(mBroadcastReceiver);
        }

        if (mHelper != null) {
            mHelper.disposeWhenFinished();
            mHelper = null;
        }
    }

    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.d("GOOGLEPAY", "Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            if (result.isFailure()) {
                Log.d("GOOGLPAY", "Error purchasing: " + result);
                return;
            }

            Log.d("GOOGLEPAY", "Purchase successful.");
        }
    };


}