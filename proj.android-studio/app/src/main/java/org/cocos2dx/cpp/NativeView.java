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

public class NativeView extends Activity {

    private static Context mContext;
    public XWalkView xWalkWebView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_view);

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";
        String myCookies = "";
        if(extras != null)
        {
            myUrl = extras.getString("url");
            myCookies = extras.getString("cookie");
        }

        final View decorView = getWindow().getDecorView();

        if (Build.VERSION.SDK_INT >= 11) {
            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_IMMERSIVE;
            decorView.setSystemUiVisibility(uiOptions);
        }

        mContext = this;

        Map<String, String> headers = new HashMap<String, String>();
        headers.put("Cookie", myCookies);



        xWalkWebView = new XWalkView(this);
        addContentView(xWalkWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        //xWalkWebView.load(myUrl, null, headers);
        xWalkWebView.load("file:///android_asset/res/jwplayer/index.html", null, headers);


        /*

        webview = new WebView(this);
        //setContentView(webview);
        addContentView(webview, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
        {
            CookieManager cookieManager = CookieManager.getInstance();
            cookieManager.setAcceptThirdPartyCookies(webview, true);
        }

        Map<String, String> headers = new HashMap<String, String>();
        headers.put("HeaderCookie", myCookies);

        for(String key : headers.keySet() ) {
            log.d("key in headers", key);
        }

        log.d("HeaderCookie", myCookies);

        webview.setWebViewClient(new WebViewClient());
        webview.getSettings().setJavaScriptEnabled(true);
        webview.getSettings().setAllowFileAccess(true);
        webview.getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
        webview.getSettings().setLoadsImagesAutomatically(true);
        webview.getSettings().setSupportMultipleWindows(true);
        webview.getSettings().setDomStorageEnabled(true);
        //if(Build.VERSION.SDK_INT < 21) webview.setLayerType(View.LAYER_TYPE_SOFTWARE, null);


        //webview.loadUrl("http://www.bonis.me/azoomee/game/index.html");
        webview.loadUrl(myUrl, headers);

        */

        Button extra = new Button(this);
        extra.setText("back");
        extra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String tag = "JniTest";
                String message = "Button was pressed";
                Log.d(tag, "Showing alert dialog: " + message);


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
}
