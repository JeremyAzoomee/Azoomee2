package org.cocos2dx.cpp;

import android.util.Log;

import org.xwalk.core.JavascriptInterface;

public class JsInterface {
    public JsInterface() {
    }

    @JavascriptInterface
    public String getData()
    {
        return "returnMessage";
        //read all files one by one and call a method to add the data to localstorage
    }
}