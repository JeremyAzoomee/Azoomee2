package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.webkit.CookieManager;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageButton;

import com.tinizine.azoomee.R;

import org.json.JSONArray;
import org.json.JSONObject;

public class NativeViewUI extends Activity {

    private static Context mContext;
    public static Activity activity = null;
    public WebView uiWebView;
    public static WebView uiWebViewStatic;
    public static String userid;
    public static ImageButton imageButtonStatic;
    private static final int _portrait = 1;
    private static final int _horizonal = 0;

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

        //---------Set Orientation-----------
        // orientation - 1 = Portrait
        if(extras.getInt("orientation") == _portrait)
            activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        userid = extras.getString("userid");
        Log.d("userid", userid);

        if(uiWebView != null) uiWebView.destroy();

        uiWebView = new WebView(this);

        uiWebView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                return false;
            }
        });

        WebSettings webSettings = uiWebView.getSettings();

        webSettings.setJavaScriptEnabled(true);
        webSettings.setJavaScriptCanOpenWindowsAutomatically(false);
        webSettings.setSupportMultipleWindows(false);
        webSettings.setSaveFormData(false);
        webSettings.setDomStorageEnabled(true);

        webSettings.setAllowFileAccess(true);
        webSettings.setBuiltInZoomControls(false);
        webSettings.setAllowUniversalAccessFromFileURLs(true);
        webSettings.setAllowFileAccessFromFileURLs(true);
        webSettings.setAllowContentAccess(true);
        webSettings.setMediaPlaybackRequiresUserGesture(false);
        webSettings.setSupportZoom(false);

        addContentView(uiWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));

        ImageButton extra = new ImageButton(this);
        extra.setImageResource(R.drawable.back_new);
        extra.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        extra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                        WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
                if(uiWebView != null)
                {
                    uiWebView.evaluateJavascript("javascript:saveLocalDataBeforeExit();", null);

                    uiWebView.loadUrl("about:blank");
                    uiWebView.removeAllViews();
                    uiWebView.clearCache(true);
                    uiWebView.pauseTimers();
                    uiWebView.onPause();
                    uiWebView.removeJavascriptInterface("NativeInterface");

                    ViewGroup vg = (ViewGroup)(uiWebView.getParent());
                    vg.removeView(uiWebView);

                    uiWebView.destroy();
                }

                JNICalls.JNIRegisterAndroidSceneChangeEvent();

                finish();
            }
        });

        addContentView(extra, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        uiWebView.resumeTimers();
        uiWebView.loadUrl("about:blank");
        uiWebView.clearCache(true);
        uiWebView.clearHistory();

        imageButtonStatic = extra;
        uiWebViewStatic = uiWebView;

        webviewAdditionalSettings();
    }

    public void onBackPressed(){
        exitView();
    }

    public static void exitView()
    {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                imageButtonStatic.callOnClick();
            }
        });
    }

    protected void webviewAdditionalSettings() {

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        if(extras != null)
        {
            myUrl = extras.getString("url");
            userid = extras.getString("userid");
        }

        Log.d("urlToBeLoaded", myUrl);
        final String urlToBeLoaded;

        if(myUrl.substring(myUrl.length() - 4).equals("html"))
        {
            urlToBeLoaded = "file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl;
        }
        else
        {
            CookieManager uiWebviewCookieManager = CookieManager.getInstance();
            if (Build.VERSION.SDK_INT >= 21)
            {
                uiWebviewCookieManager.removeAllCookies(null);
                uiWebviewCookieManager.flush();
                uiWebviewCookieManager.setAcceptThirdPartyCookies(uiWebView, true);
            }
            uiWebviewCookieManager.setAcceptCookie(true);

            try
            {
                JSONObject obj = new JSONObject(JNICalls.JNIGetAllCookies());
                JSONArray array = obj.getJSONArray("Elements");

                for(int i = 0; i < array.length(); i++)
                {
                    JSONObject currentObject = array.getJSONObject(i);
                    String url = currentObject.getString("url");
                    String cookie = currentObject.getString("cookie");

                    Log.d("COOKIE URL", url);
                    Log.d("COOKIE", cookie);

                    uiWebviewCookieManager.setCookie(url, cookie);
                }
            }
            catch (Exception ex)
            {
                JNICalls.getBackToLoginScreen();
            }
            urlToBeLoaded = "file:///android_asset/res/jwplayer/index_android.html?contentUrl=" + myUrl;
        }

        if(CookieManager.getInstance().hasCookies()) Log.d("COOKIE", "HAS COOKIES");
        else Log.d("COOKIE", "NO COOKIES SET");

        uiWebView.addJavascriptInterface(new JsInterfaceUI(), "NativeInterface");
        uiWebView.loadUrl(urlToBeLoaded);
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