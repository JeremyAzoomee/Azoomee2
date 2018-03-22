package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.pm.ApplicationInfo;
import android.os.Bundle;
import android.util.Log;

import org.xwalk.core.JavascriptInterface;

import java.io.File;
import java.io.FileOutputStream;

import static com.loopj.android.http.AsyncHttpClient.log;

public class JsInterface extends Activity {

    private static Context mContext;

    public JsInterface() {
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
        NativeView.errorOccurred();
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
    public String apiRequest(String method, String responseID, String sendData)
    {
        return JNICalls.JNISendAPIRequest(method, responseID, sendData);
    }

    @JavascriptInterface
    public String getVideoPlaylist()
    {
        return JNICalls.JNIGetVideoPlaylist();
    }

    @JavascriptInterface
    public void exitView()
    {
        NativeView.exitView();
    }
}