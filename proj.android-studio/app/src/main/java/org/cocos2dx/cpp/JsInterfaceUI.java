package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.webkit.JavascriptInterface;


import static com.loopj.android.http.AsyncHttpClient.log;

public class JsInterfaceUI extends Activity {

    private static Context mContext;

    public JsInterfaceUI()
    {
        mContext = this;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mContext = this;
    }

    @JavascriptInterface
    public void errorOccurred()
    {
        log.d("error", "errorOccurred called!");
        NativeViewUI.errorOccurred();
    }

    @JavascriptInterface
    public void sendMediaPlayerData(String key, String value)
    {
        log.d("eventhandler", "eventhandler called!");
        NativeViewUI.sendMediaPlayerData(key, value);
    }

    @JavascriptInterface
    public void saveLocalDataStorage(String data)
    {
        JNISaveLocalDataStorage(data);
    }

    @JavascriptInterface
    public String getLocalDataStorage()
    {
        return JNIGetLocalDataStorage();
    }

    @JavascriptInterface
    public String apiRequest(String method, String responseID, String score)
    {
        return JNISendAPIRequest(method, responseID, score);
    }

    @JavascriptInterface
    public String getVideoPlaylist()
    {
        return JNIGetVideoPlaylist();
    }

    @JavascriptInterface
    public void exitView()
    {
        NativeViewUI.exitView();
    }

    public static native void JNISaveLocalDataStorage(String data);
    public static native String JNIGetLocalDataStorage();
    public static native String JNISendAPIRequest(String method, String responseID, String score);
    public static native String JNIGetVideoPlaylist();
}