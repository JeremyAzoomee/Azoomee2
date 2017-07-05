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
        JNICalls.sendMediaPlayerData(key, value);
    }

    @JavascriptInterface
    public void saveLocalDataStorage(String data)
    {
        JNICalls.JNISaveLocalDataStorage(data);
    }

    @JavascriptInterface
    public String getLocalDataStorage()
    {
        return JNICalls.JNIGetLocalDataStorage();
    }

    @JavascriptInterface
    public String apiRequest(String method, String responseID, String score)
    {
        return JNICalls.JNISendAPIRequest(method, responseID, score);
    }

    @JavascriptInterface
    public String getVideoPlaylist()
    {
        return JNICalls.JNIGetVideoPlaylist();
    }

    @JavascriptInterface
    public void exitView()
    {
        NativeViewUI.exitView();
    }
}