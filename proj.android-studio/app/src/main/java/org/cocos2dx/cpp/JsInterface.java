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
        NativeView.sendMediaPlayerData(key, value);
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

    public static native void JNISaveLocalDataStorage(String data);
    public static native String JNIGetLocalDataStorage();
    public static native String JNISendAPIRequest(String method, String responseID, String score);
    public static native String JNIGetVideoPlaylist();
}