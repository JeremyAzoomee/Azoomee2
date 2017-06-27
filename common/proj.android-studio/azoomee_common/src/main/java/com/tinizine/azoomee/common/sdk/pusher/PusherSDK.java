package com.tinizine.azoomee.common.sdk.pusher;

import android.util.Log;

import com.pusher.client.Pusher;
import com.pusher.client.PusherOptions;
import com.pusher.client.channel.PrivateChannelEventListener;
import com.pusher.client.channel.PrivateChannel;
import com.pusher.client.connection.*;
import com.tinizine.azoomee.common.AzoomeeActivity;


public class PusherSDK
{
    private final static String TAG = "PusherSDK";

    /// Static Instance of Pusher
    private static Pusher sPusherClient = null;
    /// True while we should auto reconnect Pusher
    private static boolean sKeepAlive = true;

    /// Initialise with Pusher App Key
    public static void initialise(final String appKey)
    {
        PusherOptions options = new PusherOptions();
        options.setAuthorizer(new PusherSDKAuth());
        options.setCluster("eu");
        options.setEncrypted(true);
        sPusherClient = new Pusher(appKey, options);

        AzoomeeActivity.RunOnUIThread(new Runnable() {
            @Override
            public void run() {
                sPusherClient.connect(new ConnectionEventListener() {
                    @Override
                    public void onConnectionStateChange(ConnectionStateChange change) {
                        Log.d(TAG, "State changed to " + change.getCurrentState() + " from " + change.getPreviousState());

                        // Keep connection alive
                        if(change.getCurrentState().equals(ConnectionState.DISCONNECTED) && sKeepAlive)
                        {
                            if(sPusherClient != null)
                            {
                                sPusherClient.connect();
                            }
                        }
                    }

                    @Override
                    public void onError(String message, String code, Exception e) {
                        Log.d(TAG, String.format("Pusher connection error due to [%s], exception was [%s]", message, e));
                    }
                }, ConnectionState.ALL);
            }
        });
    }

    /// Subscribe to a channel
    public static void subscribeToChannel(final String channelName)
    {
        if(sPusherClient == null)
        {
            Log.e(TAG, "Java PusherSDK not initialised!");
            return;
        }

        Log.d(TAG, "subscribeToChannel: " + channelName);

        AzoomeeActivity.RunOnUIThread(new Runnable() {
            @Override
            public void run() {
                PrivateChannel channel = sPusherClient.subscribePrivate(channelName, new PrivateChannelEventListener() {
                    @Override
                    public void onSubscriptionSucceeded(String channelName) {
                        Log.d(TAG, "Subscribed to channel: " + channelName);
                    }

                    @Override
                    public void onEvent(String channelName, String eventName, String data) {
                        Log.d(TAG, "onEvent: " + eventName + ", data=" + data);
                    }

                    @Override
                    public void onAuthenticationFailure(String message, Exception e) {
                        Log.d(TAG, String.format("Authentication failure due to [%s], exception was [%s]", message, e));
                    }
                });
            }
        });
    }

    /// Close a channel subscription
    public static void closeChannel(final String channelName)
    {
        if(sPusherClient == null)
        {
            return;
        }

        AzoomeeActivity.RunOnUIThread(new Runnable() {
            @Override
            public void run() {
                sPusherClient.unsubscribe(channelName);
            }
        });
    }
}
