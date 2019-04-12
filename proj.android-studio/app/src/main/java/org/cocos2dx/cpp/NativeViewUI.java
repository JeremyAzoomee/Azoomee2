package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.widget.TextViewCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.webkit.CookieManager;
import android.webkit.WebResourceError;
import android.webkit.WebResourceRequest;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

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
    public static ImageButton favButtonStatic;
    public static ImageButton shareButtonStatic;

    private FrameLayout _favBanner;

    private static final int _portrait = 1;
    private static final int _horizonal = 0;
    private int _buttonWidth;
    private float _xAnchor;
    private float _yAnchor;
    private float _paddedWindowWidth;
    private float _paddedWindowHeight;
    private float _xMod;
    private float _yMod;
    private boolean isWebViewReady = false;
    private boolean isActivityExitRequested = false;
    private boolean _uiExpanded = false;
    private boolean _isAnimating = false;

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


        uiWebView.setWebViewClient(new WebViewClient(){

            @Override public void onReceivedError(WebView view, WebResourceRequest request,
                                                  WebResourceError error) {
                super.onReceivedError(view, request, error);
                Log.d("NativeViewUI", "onReceivedError: " + error.toString());
            }
        });

        uiWebViewStatic = uiWebView;

        addButtons();
        addFavBanner();

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
            JNICalls.JNISendProgressMetaDataGame();
        }
        
        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                JNICalls.JNIRegisterAndroidSceneChangeEvent();

                finish();
            }
        }, 500);
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
        final String urlToBeLoaded = "file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl;


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
        calcUIButtonParams();

        android.widget.RelativeLayout.LayoutParams buttonLayoutParams = new android.widget.RelativeLayout.LayoutParams(
                android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT, android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT);

        buttonLayoutParams.leftMargin = 0;
        buttonLayoutParams.topMargin = 0;
        buttonLayoutParams.width = _buttonWidth;
        buttonLayoutParams.height = _buttonWidth;

        float buttonPadding = _buttonWidth / 8.0f;

        if(!JNICalls.JNIIsAnonUser()) {
            final ImageButton favButton = new ImageButton(this);
            if (JNICalls.JNIIsInFavourites()) {
                favButton.setImageResource(R.drawable.favourite_selected_v2);
            } else {
                favButton.setImageResource(R.drawable.favourite_unelected_v2);
            }
            favButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
            favButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (isActivityExitRequested || !isWebViewReady) {
                        return;
                    }
                    if (JNICalls.JNIIsInFavourites()) {
                        JNICalls.JNIRemoveFromFavourites();
                        favButton.setImageResource(R.drawable.favourite_unelected_v2);
                    } else {
                        JNICalls.JNIAddToFavourites();
                        animateFavBanner();
                        favButton.setImageResource(R.drawable.favourite_selected_v2);
                    }
                }
            });

            favButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
            favButton.setX(buttonPadding + (_xAnchor * _paddedWindowWidth) + (_xMod * _buttonWidth));
            favButton.setY(buttonPadding + (_yAnchor * _paddedWindowHeight) + (_yMod * _buttonWidth));

            addContentView(favButton, buttonLayoutParams);

            favButtonStatic = favButton;

            if(JNICalls.JNIIsChatEntitled()) {
                final ImageButton shareButton = new ImageButton(this);
                shareButton.setImageResource(R.drawable.share_unelected_v2);
                shareButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
                shareButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (isActivityExitRequested || !isWebViewReady) {
                            return;
                        }
                        JNICalls.JNIShareInChat();
                        Bundle extras = getIntent().getExtras();
                        if (extras.getInt("orientation") == _portrait) {
                            isActivityExitRequested = true;
                            activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                            //cleanUpAndFinishActivity() will be called by the screen orientation change callback
                        } else {
                            cleanUpAndFinishActivity();
                        }

                    }
                });

                shareButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
                shareButton.setX(buttonPadding + (_xAnchor * _paddedWindowWidth) + 2 * (_xMod * _buttonWidth));
                shareButton.setY(buttonPadding + (_yAnchor * _paddedWindowHeight) + 2 * (_yMod * _buttonWidth));

                addContentView(shareButton, buttonLayoutParams);

                shareButtonStatic = shareButton;
            }
            else
            {
                shareButtonStatic = null;
            }

        }
        final ImageButton closeButton = new ImageButton(this);
        closeButton.setImageResource(R.drawable.close_unelected_v2);

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

                cleanUpAndFinishActivity();
            }
        });
        closeButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);

        closeButton.setX(buttonPadding + (_xAnchor * _paddedWindowWidth));
        closeButton.setY(buttonPadding + (_yAnchor * _paddedWindowHeight));

        // Add button to screen, with Size and Position
        addContentView(closeButton, buttonLayoutParams);

        imageButtonStatic = closeButton;
    }

    // Create fav notification banner
    void addFavBanner()
    {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int height = displayMetrics.heightPixels;
        int width = displayMetrics.widthPixels;

        android.widget.RelativeLayout.LayoutParams bgLayoutParams = new android.widget.RelativeLayout.LayoutParams(
                android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT, android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT);

        bgLayoutParams.leftMargin = 0;
        bgLayoutParams.topMargin = 0;
        bgLayoutParams.width = (int)(Math.max(width, height) * 0.334f);
        bgLayoutParams.height = (int)(Math.min(width,height) * 0.105f);

        _favBanner = new FrameLayout(this);
        _favBanner.setX(width / 2 - bgLayoutParams.width / 2);
        _favBanner.setY(0);
        _favBanner.setAlpha(0.0f);

        addContentView(_favBanner,bgLayoutParams);

        ImageView background = new ImageView(this);
        background.setImageResource(R.drawable.fav_banner);

        background.setX(0);
        background.setY(0);

        _favBanner.addView(background, bgLayoutParams);

        ImageView heart = new ImageView(this);
        heart.setImageResource(R.drawable.heart);

        android.widget.RelativeLayout.LayoutParams heartLayoutParams = new android.widget.RelativeLayout.LayoutParams(
                android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT, android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT);

        heartLayoutParams.leftMargin = 0;
        heartLayoutParams.topMargin = 0;
        heartLayoutParams.width = (int)(bgLayoutParams.width * 0.107f);
        heartLayoutParams.height = (int)(bgLayoutParams.height * 0.44f);

        heart.setX(bgLayoutParams.width * 0.07f);
        heart.setY(bgLayoutParams.height * 0.28f);

        _favBanner.addView(heart, heartLayoutParams);

        TextView text = new TextView(this);
        text.setText(JNICalls.JNIGetStringForKey("Added to favourites"));
        text.setWidth((int)(bgLayoutParams.width * 0.7f));
        text.setHeight((int)(bgLayoutParams.height * 0.6f));
        text.setX(bgLayoutParams.width * 0.25f);
        text.setY(bgLayoutParams.height * 0.2f);
        Typeface face = Typeface.createFromAsset(getAssets(),
                "fonts/azoomee.ttf");
        text.setTypeface(face);

        TextViewCompat.setAutoSizeTextTypeUniformWithConfiguration(text,(int)(bgLayoutParams.height * 0.1f),(int)(bgLayoutParams.height),2, TypedValue.COMPLEX_UNIT_DIP);

        _favBanner.addView(text);
    }

    void animateFavBanner()
    {

        final TranslateAnimation returnAnim = new TranslateAnimation(0,0,0,-_favBanner.getHeight());
        returnAnim.setDuration(500);
        returnAnim.setAnimationListener(new TranslateAnimation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation)
            {

            }

            @Override
            public void onAnimationRepeat(Animation animation)
            {

            }
            @Override
            public void onAnimationEnd(Animation animation)
            {
                _favBanner.setAlpha(0.0f);
            }
        });

        final TranslateAnimation waitAnim = new TranslateAnimation(0,0,0,0);
        waitAnim.setDuration(1000);
        waitAnim.setAnimationListener(new TranslateAnimation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation)
            {

            }

            @Override
            public void onAnimationRepeat(Animation animation)
            {

            }
            @Override
            public void onAnimationEnd(Animation animation)
            {
                _favBanner.startAnimation(returnAnim);
            }
        });
        waitAnim.setFillAfter(true);

        TranslateAnimation animation = new TranslateAnimation(0,0,-_favBanner.getHeight(),0);
        animation.setDuration(500);
        animation.setAnimationListener(new TranslateAnimation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation)
            {
                _favBanner.setAlpha(1.0f);
            }

            @Override
            public void onAnimationRepeat(Animation animation)
            {

            }
            @Override
            public void onAnimationEnd(Animation animation)
            {
                _favBanner.startAnimation(waitAnim);
            }
        });
        animation.setFillAfter(true);
        _favBanner.startAnimation(animation);
    }

    void calcUIButtonParams()
    {
        Bundle extras = getIntent().getExtras();

        //SET Button Size and position
        WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        android.view.Display display = wm.getDefaultDisplay();
        android.util.DisplayMetrics metrics = new android.util.DisplayMetrics();
        display.getMetrics(metrics);

        _buttonWidth = metrics.widthPixels / 12;
        if(metrics.heightPixels > metrics.widthPixels)
        {
            _buttonWidth = metrics.heightPixels / 12;
        }

        _paddedWindowWidth = metrics.widthPixels - (_buttonWidth * 1.25f);
        _paddedWindowHeight = metrics.heightPixels - (_buttonWidth * 1.25f);
        _xAnchor = extras.getFloat("closeAnchorX");
        _yAnchor = extras.getFloat("closeAnchorY");

        _xMod = 0;
        _yMod = 0;
        if(_yAnchor == 0.5)
        {
            if(_xAnchor == 1.0)
            {
                _xMod = -1;
            }
            else
            {
                _xMod = 1;
            }
        }
        else if(_yAnchor == 0.0)
        {
            _yMod = 1;
        }
        else
        {
            _yMod = -1;
        }
    }

}