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
    public int getAmountOfStorageElements()
    {
        return NativeView.getAmountOfStorageElements();
    }

    @JavascriptInterface
    public String getKeyForStorageElement(int fileNumber)
    {
        return NativeView.getKeyForStorageElement(fileNumber);
    }

    @JavascriptInterface
    public String getValueForStorageElement(int fileNumber)
    {
        return NativeView.getValueForStorageElement(fileNumber);
    }

    @JavascriptInterface
    public void saveData(String title, String data)
    {
        NativeView.saveLocalDataForUser(title, data);
    }

    @JavascriptInterface
    public void saveImage(String title, String data)
    {
        NativeView.saveImageForUser(title, data);
    }
}