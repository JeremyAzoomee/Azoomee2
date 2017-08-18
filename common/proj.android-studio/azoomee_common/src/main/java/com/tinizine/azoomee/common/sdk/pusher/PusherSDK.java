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
    private Pusher _pusherClient = null;
    /// True while we should auto reconnect Pusher
    private boolean _keepAlive = true;
    /// Current connection status
    private ConnectionState _connectionState = ConnectionState.DISCONNECTED;


    // - Static

    private static class SingletonHolder
    {
        private static final PusherSDK Instance = new PusherSDK();
    }

    public static PusherSDK getInstance()
    {
        return SingletonHolder.Instance;
    }

    /// Initialise with Pusher App Key
    public static void Initialise(final String appKey)
    {
        getInstance().initialise(appKey);
    }

    /// Destroy the Pusher SDK instance - it must be initialised to be used again
    public static void Destroy()
    {
        getInstance().close();
    }

    /// Subscribe to a channel
    public static void SubscribeToChannel(final String channelName)
    {
        getInstance().subscribeToChannel(channelName);
    }

    /// Close a channel subscription
    public static void CloseChannel(final String channelName)
    {
        getInstance().closeChannel(channelName);
    }


    // - Private

    // No direct construction - use getInstance
    private PusherSDK()
    {
    }

    /// Close all connections
    private void close()
    {
        _keepAlive = false;
        if(_pusherClient != null)
        {
            _pusherClient.disconnect();
            _pusherClient = null;
        }
    }

    /// Initialise with Pusher App Key
    private void initialise(final String appKey)
    {
        PusherOptions options = new PusherOptions();
        options.setAuthorizer(new PusherSDKAuth());
        options.setCluster("eu");
        options.setEncrypted(true);
        _pusherClient = new Pusher(appKey, options);
        _keepAlive = true;

        AzoomeeActivity.RunOnUIThread(new Runnable() {
            @Override
            public void run() {
                _pusherClient.connect(new ConnectionEventListener() {
                    @Override
                    public void onConnectionStateChange(ConnectionStateChange change) {
                        _connectionState = change.getCurrentState();
                        Log.d(TAG, "State changed from " + change.getPreviousState() + " to " + change.getCurrentState());

                        // Keep connection alive
                        if(_connectionState.equals(ConnectionState.DISCONNECTED) && _keepAlive)
                        {
                            if(_pusherClient != null)
                            {
                                _pusherClient.connect();
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
    private void subscribeToChannel(final String channelName)
    {
        if(_pusherClient == null)
        {
            Log.e(TAG, "Java PusherSDK not initialised!");
            return;
        }

        AzoomeeActivity.RunOnUIThread(new Runnable() {
            @Override
            public void run() {
                if(_pusherClient != null)
                {
                    Log.d(TAG, "subscribeToChannel: " + channelName);

                    try
                    {
                        PrivateChannel channel = _pusherClient.subscribePrivate(channelName, new PrivateChannelEventListener() {
                            @Override
                            public void onSubscriptionSucceeded(String channelName) {
                                Log.d(TAG, "Subscribed to channel: " + channelName);
                            }

                            @Override
                            public void onEvent(final String channelName, final String eventName, final String data) {
                                Log.d(TAG, "onEvent: channelName=" + channelName + ", eventName=" + eventName + ", data=" + data);

                                AzoomeeActivity.RunOnGLThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        onPusherEventRecieved(channelName, eventName, data);
                                    }
                                });
                            }

                            @Override
                            public void onAuthenticationFailure(String message, Exception e) {
                                Log.d(TAG, String.format("Authentication failure due to [%s], exception was [%s]", message, e));
                            }
                        }, "SEND_MESSAGE", "MODERATION"); // TODO: In a future update of Pusher, see if it's possible to avoid registering for specific events
                    }
                    catch(IllegalArgumentException e)
                    {
                        // Channel is already subscribed, we can silently continue
                    }
                }
            }
        });
    }

    /// Close a channel subscription
    private void closeChannel(final String channelName)
    {
        if(_pusherClient == null)
        {
            return;
        }

        AzoomeeActivity.RunOnUIThread(new Runnable() {
            @Override
            public void run() {
                if(_pusherClient != null)
                {
                    _pusherClient.unsubscribe(channelName);
                }
            }
        });
    }

    /// Recieved a Pusher event
    private native void onPusherEventRecieved(String channelName, String eventName, String data);
}
