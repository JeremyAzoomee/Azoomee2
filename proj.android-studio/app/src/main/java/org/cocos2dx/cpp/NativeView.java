package org.cocos2dx.cpp;

import android.app.ActionBar;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.webkit.CookieManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;

import com.tinizine.azoomee.R;

import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import org.xwalk.core.XWalkActivity;
import org.xwalk.core.XWalkPreferences;
import org.xwalk.core.XWalkView;
import org.xwalk.core.XWalkCookieManager;


import static com.loopj.android.http.AsyncHttpClient.log;

public class NativeView extends XWalkActivity {

    private static Context mContext;
    public XWalkView xWalkWebView;

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

        xWalkWebView = new XWalkView(this);
        addContentView(xWalkWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        //xWalkWebView.load(myUrl, null, headers);



        Button extra = new Button(this);
        extra.setText("back");
        extra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                xWalkWebView.removeAllViews();

                if(xWalkWebView != null)
                {
                    xWalkWebView.clearCache(true);
                    xWalkWebView = null;
                }

                finish();
            }
        });

        addContentView(extra, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
    }

    @Override
    protected void onXWalkReady() {
        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";
        String myCookies = "";
        if(extras != null)
        {
            myUrl = extras.getString("url");
            myCookies = extras.getString("cookie");
        }

        Map<String, String> headers = new HashMap<String, String>();
        headers.put("Cookie", myCookies);




        // Do anything with embedding API

        XWalkCookieManager mCookieManager = new XWalkCookieManager();
        mCookieManager.setAcceptCookie(true);
        mCookieManager.setAcceptFileSchemeCookies(true);

        String[] separatedCookies = myCookies.split("; ");

        for(int i = 0; i < separatedCookies.length; i++)
        {
            log.d("seaparatecookies: ", separatedCookies[i]);
            mCookieManager.setCookie("https://media.azoomee.ninja", separatedCookies[i]);
        }

        log.d("cookies: ", mCookieManager.getCookie("https://media.azoomee.ninja"));

        //Check if the url received url ends with html, or anything else. If html, then we have to
        //open the html directly, otherwise we have to open the playlist with jw player.

        log.d("url", myUrl);

        if(myUrl.substring(myUrl.length() - 4).equals("html"))
        {
            xWalkWebView.load("file://" + myUrl, null);
        }
        else
        {
            xWalkWebView.load("file:///android_asset/res/jwplayer/index.html?contentUrl=" + myUrl, null);
        }
    }
}
