package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageButton;

import com.tinizine.azoomee.R;

import org.json.JSONArray;
import org.json.JSONObject;
import org.xwalk.core.XWalkActivity;
import org.xwalk.core.XWalkView;
import org.xwalk.core.XWalkCookieManager;

public class NativeView extends XWalkActivity {

    private static Context mContext;
    public static Activity activity = null;
    public XWalkView xWalkWebView;
    public static XWalkView xWalkWebViewStatic;
    public static String userid;
    public static ImageButton imageButtonStatic;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_view);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);

        final View decorView = getWindow().getDecorView();

        if (Build.VERSION.SDK_INT >= 11) {
            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_IMMERSIVE;
            decorView.setSystemUiVisibility(uiOptions);
        }

        mContext = this;
        activity = this;

        Bundle extras = getIntent().getExtras();
        userid = extras.getString("userid");

        xWalkWebView = new XWalkView(this);
        addContentView(xWalkWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));

        ImageButton extra = new ImageButton(this);
        extra.setImageResource(R.drawable.back_new);
        extra.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        extra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                        WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
                if(xWalkWebView != null)
                {
                    xWalkWebView.evaluateJavascript("javascript:saveLocalDataBeforeExit();", null);
                    xWalkWebView.loadUrl("about:blank");
                    xWalkWebView.removeAllViews();
                    xWalkWebView.clearCache(true);
                    xWalkWebView.pauseTimers();
                    xWalkWebView = null;

                    xWalkWebViewStatic = null;
                }

                JNICalls.JNIRegisterAndroidSceneChangeEvent();

                finish();
            }
        });

        addContentView(extra, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        imageButtonStatic = extra;
        xWalkWebViewStatic = xWalkWebView;
    }

    public static void exitView()
    {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                imageButtonStatic.callOnClick();
            }
        });
    }

    @Override
    protected void onXWalkReady() {
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        if(extras != null)
        {
            myUrl = extras.getString("url");
            userid = extras.getString("userid");
        }

        if(myUrl.substring(myUrl.length() - 4).equals("html"))
        {
            xWalkWebView.loadUrl("file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl);
        }
        else
        {
            XWalkCookieManager mCookieManager = new XWalkCookieManager();
            mCookieManager.removeSessionCookie();
            mCookieManager.removeExpiredCookie();
            mCookieManager.removeAllCookie();
            mCookieManager.flushCookieStore();
            mCookieManager.setAcceptCookie(true);
            mCookieManager.setAcceptFileSchemeCookies(true);

            try
            {
                JSONObject obj = new JSONObject(JNICalls.JNIGetAllCookies());
                JSONArray array = obj.getJSONArray("Elements");

                for(int i = 0; i < array.length(); i++)
                {
                    JSONObject currentObject = array.getJSONObject(i);
                    String url = currentObject.getString("url");
                    String cookie = currentObject.getString("cookie");

                    Log.d("COOKIE URL: ", url);

                    mCookieManager.setCookie(url, cookie);

                    Log.d("COOKIE FOR URL AFTER", mCookieManager.getCookie(url));
                }
            }
            catch (Exception ex)
            {
                JNICalls.getBackToLoginScreen();
            }

            xWalkWebView.loadUrl("file:///android_asset/res/jwplayer/index_android.html?contentUrl=" + myUrl);
        }

        xWalkWebView.addJavascriptInterface(new JsInterface(), "NativeInterface");
    }

    static void errorOccurred()
    {
        JNICalls.JNIRegisterAndroidSceneChangeEvent();
        JNICalls.getBackToLoginScreen();
        activity.finish();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        JNICalls.JNIRegisterAppWentBackgroundEvent();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        JNICalls.JNIRegisterAppCameForegroundEvent();
    }
}
