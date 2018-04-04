package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.TranslateAnimation;
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
    public static ImageButton burgerButtonStatic;
    public static ImageButton imageButtonStatic;
    public static ImageButton favButtonStatic;
    public static ImageButton shareButtonStatic;
    private static final int _portrait = 1;
    private static final int _horizonal = 0;
    private boolean isWebViewReady = false;
    private boolean isActivityExitRequested = false;
    private boolean _uiExpanded = false;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_view);

        final View decorView = getWindow().getDecorView();

        if (Build.VERSION.SDK_INT >= 11)
        {
            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_IMMERSIVE;
            decorView.setSystemUiVisibility(uiOptions);
        }

        mContext = this;
        activity = this;

        Bundle extras = getIntent().getExtras();

        //---------Set Orientation-----------
        // orientation - 1 = Portrait
        if(extras.getInt("orientation") == _portrait)
        {
            activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        }

        userid = extras.getString("userid");
        Log.d("userid", userid);

        if(uiWebView != null)
        {
            uiWebView.destroy();
        }

        uiWebView = new WebView(this);

        uiWebView.setWebViewClient(new WebViewClient()
        {
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

        uiWebViewStatic = uiWebView;

        addButtons();

        webviewAdditionalSettings();
    }

    private void cleanUpAndFinishActivity()
    {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
        if(uiWebView != null && isWebViewReady)
        {
            isWebViewReady = false;
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

        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                JNICalls.JNIRegisterAndroidSceneChangeEvent();

                finish();
            }
        }, 1500);
    }

    private  boolean loadingGame()
    {
        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        if(extras != null)
        {
            myUrl = extras.getString("url");

            if(myUrl.toLowerCase().contains("html"))
            {
                return true;
            }
        }

        return false;
    }

    public void onBackPressed()
    {
        if(isActivityExitRequested||!isWebViewReady)
        {
            return;
        }

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
        exitView();
    }

    public static void exitView()
    {
        activity.runOnUiThread(new Runnable()
        {
            public void run()
            {
                imageButtonStatic.callOnClick();
            }
        });
    }

    protected void webviewAdditionalSettings()
    {

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        if(extras != null)
        {
            myUrl = extras.getString("url");
            userid = extras.getString("userid");
        }

        Log.d("urlToBeLoaded", myUrl);
        final String urlToBeLoaded;

        if(myUrl.endsWith("html"))
        {
            if(myUrl.startsWith("http")) //content is game loaded remotely
            {
                urlToBeLoaded = JNICalls.JNIGetRemoteWebGameAPIPath() + "index_android.html?contentUrl=" + myUrl;
            }
            else //game is loaded locally
            {
                urlToBeLoaded = "file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl;
            }
        }
        else
        {
            urlToBeLoaded = "file:///android_asset/res/jwplayer/index_android.html?contentUrl=" + myUrl;
        }

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

        if(CookieManager.getInstance().hasCookies()) Log.d("COOKIE", "HAS COOKIES");
        else Log.d("COOKIE", "NO COOKIES SET");

        uiWebView.addJavascriptInterface(new JsInterfaceUI(), "NativeInterface");
        uiWebView.loadUrl(urlToBeLoaded);
        isWebViewReady = true;
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
        if(isWebViewReady && uiWebView != null)
        {
            uiWebView.pauseTimers();
            uiWebView.onPause();
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        JNICalls.JNIRegisterAppCameForegroundEvent();
        if(isWebViewReady && uiWebView != null)
        {
            uiWebView.resumeTimers();
            uiWebView.onResume();
        }
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        if(activity.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE && isActivityExitRequested)
        {
            isActivityExitRequested = false;
            cleanUpAndFinishActivity();
        }
    }

    private void addButtons()
    {
        Bundle extras = getIntent().getExtras();

        //SET Button Size and position
        WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        android.view.Display display = wm.getDefaultDisplay();
        android.util.DisplayMetrics metrics = new android.util.DisplayMetrics();
        display.getMetrics(metrics);

        int buttonWidth = metrics.widthPixels / 12;
        if(metrics.heightPixels > metrics.widthPixels)
        {
            buttonWidth = metrics.heightPixels / 12;
        }

        android.widget.RelativeLayout.LayoutParams buttonLayoutParams = new android.widget.RelativeLayout.LayoutParams(
                android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT, android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT);

        buttonLayoutParams.leftMargin = 0;
        buttonLayoutParams.topMargin = 0;
        buttonLayoutParams.width = buttonWidth;
        buttonLayoutParams.height = buttonWidth;


        float paddedWindowWidth = metrics.widthPixels - (buttonWidth * 1.25f);
        float paddedWindowHeight = metrics.heightPixels - (buttonWidth * 1.25f);
        float xAnchor = extras.getFloat("closeAnchorX");
        float yAnchor = extras.getFloat("closeAnchorY");

        ImageButton closeButton = new ImageButton(this);
        if(loadingGame())
        {
            closeButton.setImageResource(R.drawable.close_button);
        }
        else
        {
            closeButton.setImageResource(R.drawable.back_button);
        }

        closeButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        closeButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                if(isActivityExitRequested||!isWebViewReady)
                {
                    return;
                }

                getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                        WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);

                Bundle extras = getIntent().getExtras();
                if(extras.getInt("orientation") == _portrait)
                {
                    isActivityExitRequested = true;
                    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                    //cleanUpAndFinishActivity() will be called by the screen orientation change callback
                }
                else
                {
                    cleanUpAndFinishActivity();
                }
            }
        });
        closeButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);

        closeButton.setX(buttonWidth/8 + (xAnchor * paddedWindowWidth));
        closeButton.setY(buttonWidth/8 + (yAnchor * paddedWindowHeight));

        // Add button to screen, with Size and Position
        addContentView(closeButton, buttonLayoutParams);

        imageButtonStatic = closeButton;

        ImageButton favButton = new ImageButton(this);
        favButton.setImageResource(R.drawable.confirm);
        favButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        favButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                if(isActivityExitRequested||!isWebViewReady)
                {
                    return;
                }

                JNICalls.JNIAddToFavourites();
            }
        });

        favButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
        favButton.setX(buttonWidth/8 + (xAnchor * paddedWindowWidth));
        favButton.setY(buttonWidth/8 + (yAnchor * paddedWindowHeight));

        addContentView(favButton, buttonLayoutParams);

        favButtonStatic = favButton;

        ImageButton shareButton = new ImageButton(this);
        shareButton.setImageResource(R.drawable.chat);
        shareButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        shareButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                if(isActivityExitRequested||!isWebViewReady)
                {
                    return;
                }

                JNICalls.JNIShareInChat();
                Bundle extras = getIntent().getExtras();
                if(extras.getInt("orientation") == _portrait)
                {
                    isActivityExitRequested = true;
                    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                    //cleanUpAndFinishActivity() will be called by the screen orientation change callback
                }
                else
                {
                    cleanUpAndFinishActivity();
                }

            }
        });

        shareButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
        shareButton.setX(buttonWidth/8 + (xAnchor * paddedWindowWidth));
        shareButton.setY(buttonWidth/8 + (yAnchor * paddedWindowHeight));

        addContentView(shareButton, buttonLayoutParams);

        shareButtonStatic = shareButton;

        ImageButton burgerButton = new ImageButton(this);
        burgerButton.setImageResource(R.drawable.settings);
        burgerButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        burgerButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                animateButtons();
            }
        });

        burgerButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
        burgerButton.setX(buttonWidth/8 + (xAnchor * paddedWindowWidth));
        burgerButton.setY(buttonWidth/8 + (yAnchor * paddedWindowHeight));

        addContentView(burgerButton, buttonLayoutParams);

        burgerButtonStatic = burgerButton;

        _uiExpanded = false;
    }

    void animateButtons()
    {
        Bundle extras = getIntent().getExtras();

        //SET Button Size and position
        WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        android.view.Display display = wm.getDefaultDisplay();
        android.util.DisplayMetrics metrics = new android.util.DisplayMetrics();
        display.getMetrics(metrics);

        int buttonWidth = metrics.widthPixels / 12;
        if(metrics.heightPixels > metrics.widthPixels)
        {
            buttonWidth = metrics.heightPixels / 12;
        }

        float paddedWindowWidth = metrics.widthPixels - (buttonWidth * 1.25f);
        float paddedWindowHeight = metrics.heightPixels - (buttonWidth * 1.25f);
        float xAnchor = extras.getFloat("closeAnchorX");
        float yAnchor = extras.getFloat("closeAnchorY");

        if(_uiExpanded)
        {
            Log.d("anim","close ui");
            TranslateAnimation closeButtonAnim = new TranslateAnimation(0,burgerButtonStatic.getX() - imageButtonStatic.getX(),0,burgerButtonStatic.getY() - imageButtonStatic.getY());
            closeButtonAnim.setDuration(750);
            closeButtonAnim.setFillAfter(true);
            imageButtonStatic.startAnimation(closeButtonAnim);
            TranslateAnimation favButtonAnim = new TranslateAnimation(0,burgerButtonStatic.getX() - favButtonStatic.getX(),0,burgerButtonStatic.getY() - favButtonStatic.getY());
            favButtonAnim.setDuration(750);
            favButtonAnim.setFillAfter(true);
            favButtonStatic.startAnimation(favButtonAnim);
            TranslateAnimation shareButtonAnim = new TranslateAnimation(0,burgerButtonStatic.getX() - shareButtonStatic.getX(),0,burgerButtonStatic.getY() - shareButtonStatic.getY());
            shareButtonAnim.setDuration(750);
            shareButtonAnim.setFillAfter(true);
            shareButtonStatic.startAnimation(shareButtonAnim);
            _uiExpanded = false;
        }
        else
        {
            Log.d("anim","open ui");
            float xMod = 0;
            float yMod = 0;
            if(yAnchor == 0.5)
            {
                if(xAnchor == 1.0)
                {
                    xMod = -1;
                }
                else
                {
                    xMod = 1;
                }
            }
            else if(yAnchor == 0.0)
            {
                yMod = 1;
            }
            else
            {
                yMod = -1;
            }

            TranslateAnimation closeButtonAnim = new TranslateAnimation(0, (xMod * buttonWidth), 0,(yMod * buttonWidth));
            closeButtonAnim.setDuration(750);
            closeButtonAnim.setFillAfter(true);
            imageButtonStatic.startAnimation(closeButtonAnim);
            TranslateAnimation favButtonAnim = new TranslateAnimation(0, 2 * (xMod * buttonWidth), 0, 2 * (yMod * buttonWidth));
            favButtonAnim.setDuration(750);
            closeButtonAnim.setFillAfter(true);
            favButtonStatic.startAnimation(favButtonAnim);
            TranslateAnimation shareButtonAnim = new TranslateAnimation(0,  3 * (xMod * buttonWidth), 0, 3 * (yMod * buttonWidth));
            shareButtonAnim.setDuration(750);
            shareButtonAnim.setFillAfter(true);
            shareButtonStatic.startAnimation(shareButtonAnim);
            _uiExpanded = true;
        }
    }
}