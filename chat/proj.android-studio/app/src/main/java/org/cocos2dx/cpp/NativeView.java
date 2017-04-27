package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.ContextWrapper;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;

import com.tinizine.azoomeechat.R;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.HashMap;
import java.util.Map;

import org.xwalk.core.XWalkActivity;
import org.xwalk.core.XWalkView;
import org.xwalk.core.XWalkCookieManager;


import static com.loopj.android.http.AsyncHttpClient.log;

public class NativeView extends XWalkActivity {

    private static Context mContext;
    public static Activity activity = null;
    public XWalkView xWalkWebView;
    public static XWalkView xWalkWebViewStatic;
    public static String userid;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_view);

        final View decorView = getWindow().getDecorView();

        if (Build.VERSION.SDK_INT >= 11) {
            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_IMMERSIVE;
            decorView.setSystemUiVisibility(uiOptions);
        }

        mContext = this;
        activity = this;

        Bundle extras = getIntent().getExtras();
        userid = extras.getString("userid");
        log.d("userid", userid);

        xWalkWebView = new XWalkView(this);
        addContentView(xWalkWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        //xWAlkWebView.load(myUrl, null, headers);



        ImageButton extra = new ImageButton(this);
        extra.setImageResource(R.drawable.back_new);
        extra.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        extra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(xWalkWebView != null)
                {
                    xWalkWebView.evaluateJavascript("javascript:saveLocalDataBeforeExit();", null);
                    xWalkWebView.removeAllViews();
                    xWalkWebView.clearCache(true);
                    xWalkWebView = null;
                }

                finish();
            }
        });

        addContentView(extra, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        xWalkWebViewStatic = xWalkWebView;
    }

    @Override
    protected void onXWalkReady() {
        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";
        String myCookieUrl = "";
        String myCookies = "";
        if(extras != null)
        {
            myUrl = extras.getString("url");
            myCookieUrl = extras.getString("cookieurl");
            myCookies = extras.getString("cookie");
            userid = extras.getString("userid");
        }

        log.d("urlToBeLoaded", myUrl);

        if(myUrl.substring(myUrl.length() - 4).equals("html"))
        {
            xWalkWebView.load("file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl, null);
        }
        else
        {
            Map<String, String> headers = new HashMap<String, String>();
            headers.put("Cookie", myCookies);

            XWalkCookieManager mCookieManager = new XWalkCookieManager();
            mCookieManager.flushCookieStore();
            mCookieManager.setAcceptCookie(true);
            mCookieManager.setAcceptFileSchemeCookies(true);

            String[] separatedCookies = myCookies.split("; ");

            for(int i = 0; i < separatedCookies.length; i++)
            {
                log.d("separatecookies: ", separatedCookies[i]);
                mCookieManager.setCookie(myCookieUrl, separatedCookies[i]);
            }

            log.d("cookies: ", mCookieManager.getCookie(myCookieUrl));

            xWalkWebView.load("file:///android_asset/res/jwplayer/index_android.html?contentUrl=" + myUrl, null);
        }

        xWalkWebView.addJavascriptInterface(new JsInterface(), "NativeInterface");
    }

    static void errorOccurred()
    {
        getBackToLoginScreen();
        activity.finish();
    }

    public static native void getBackToLoginScreen();
    public static native void sendMediaPlayerData(String eventKey, String eventValue);
}
