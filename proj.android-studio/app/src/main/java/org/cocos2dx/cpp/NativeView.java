package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.Transformation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageButton;

import com.tinizine.azoomee.R;

import org.json.JSONArray;
import org.json.JSONObject;
import org.xwalk.core.XWalkActivity;
import org.xwalk.core.XWalkSettings;
import org.xwalk.core.XWalkView;
import org.xwalk.core.XWalkCookieManager;

public class NativeView extends XWalkActivity {

    private static Context mContext;
    public static Activity activity = null;
    public XWalkView xWalkWebView;
    public static XWalkView xWalkWebViewStatic;
    public static String userid;
    public static ImageButton burgerButtonStatic;
    public static ImageButton imageButtonStatic;
    public static ImageButton favButtonStatic;
    public static ImageButton shareButtonStatic;
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
            activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        userid = extras.getString("userid");

        xWalkWebView = new XWalkView(this);
        addContentView(xWalkWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));

        addButtons();

        xWalkWebViewStatic = xWalkWebView;
    }

    private void cleanUpAndFinishActivity()
    {
        if(xWalkWebView != null && isWebViewReady)
        {
            isWebViewReady = false;
            xWalkWebView.evaluateJavascript("javascript:saveLocalDataBeforeExit();", null);
            xWalkWebView.loadUrl("about:blank");
            xWalkWebView.removeAllViews();
            xWalkWebView.clearCache(true);
            xWalkWebView.pauseTimers();
            xWalkWebView.onDestroy();
        }

        xWalkWebView = null;
        xWalkWebViewStatic = null;

        final Handler handler = new Handler();
        handler.postDelayed(new Runnable()
        {
            @Override
            public void run()
            {
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

    @Override
    protected void onXWalkReady()
    {
        isWebViewReady = true;
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        XWalkSettings webSettings = xWalkWebView.getSettings();

        webSettings.setAllowUniversalAccessFromFileURLs(true);
        webSettings.setAllowContentAccess(true);
        webSettings.setAllowFileAccess(true);
        webSettings.setAllowFileAccessFromFileURLs(true);
        webSettings.setBuiltInZoomControls(false);
        webSettings.setMediaPlaybackRequiresUserGesture(false);
        webSettings.setSupportZoom(false);


        if(extras != null)
        {
            myUrl = extras.getString("url");
            userid = extras.getString("userid");
        }

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

                mCookieManager.setCookie(url, cookie);
            }
        }
        catch (Exception ex)
        {
            JNICalls.getBackToLoginScreen();
        }

        if(myUrl.endsWith("html")) //content is game
        {
            if(myUrl.startsWith("http")) //content is game loaded remotely
            {
                xWalkWebView.loadUrl(JNICalls.JNIGetRemoteWebGameAPIPath() + "index_android.html?contentUrl=" + myUrl);
            }
            else //game is loaded locally
            {
                xWalkWebView.loadUrl("file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl);
            }
        }
        else
        {
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
        if (xWalkWebView != null && isWebViewReady)
        {
            xWalkWebView.pauseTimers();
            xWalkWebView.onHide();
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        JNICalls.JNIRegisterAppCameForegroundEvent();
        if (isWebViewReady && xWalkWebView != null)
        {
            xWalkWebView.resumeTimers();
            xWalkWebView.onShow();
        }

    }

    @Override
    public void onConfigurationChanged(Configuration newConfig)
    {
        super.onConfigurationChanged(newConfig);

        if(activity.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE && isActivityExitRequested)
        {
            isActivityExitRequested = false;
            cleanUpAndFinishActivity();
        }
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {

        try
        {
            return super.dispatchTouchEvent(event);
        }
        catch(java.lang.RuntimeException e) {

            if ( e.getMessage().compareTo("Crosswalk's APIs are not ready yet") == 0 )
            {
                //don't throw exception if api is not ready.
            }
            else
            {
                throw e;
            }
        }

        return false;
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

        closeButton.setX(_buttonWidth/8 + (_xAnchor * _paddedWindowWidth) + (_xMod * _buttonWidth));
        closeButton.setY(_buttonWidth/8 + (_yAnchor * _paddedWindowHeight) + (_yMod * _buttonWidth));

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
        favButton.setX(_buttonWidth/8 + (_xAnchor * _paddedWindowWidth) + 2 * (_xMod * _buttonWidth));
        favButton.setY(_buttonWidth/8 + (_yAnchor * _paddedWindowHeight) + 2 * (_yMod * _buttonWidth));

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
        shareButton.setX(_buttonWidth/8 + (_xAnchor * _paddedWindowWidth) + 3 * (_xMod * _buttonWidth));
        shareButton.setY(_buttonWidth/8 + (_yAnchor * _paddedWindowHeight) + 3 * (_yMod * _buttonWidth));

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
        burgerButton.setX(_buttonWidth/8 + (_xAnchor * _paddedWindowWidth));
        burgerButton.setY(_buttonWidth/8 + (_yAnchor * _paddedWindowHeight));

        addContentView(burgerButton, buttonLayoutParams);

        burgerButtonStatic = burgerButton;

        _uiExpanded = true;
        animateButtons();
    }

    void animateButtons()
    {

        if(_isAnimating)
        {
            return;
        }

        //Ok, so android animations dont actually "move" the button, so the button is always in its expanded position,
        //and the animations are sone relative to that.  when buttons are in the "closed" state, they are dissabled.
        if(_uiExpanded)
        {
            TranslateAnimation closeButtonAnim = new TranslateAnimation(0,burgerButtonStatic.getX() - imageButtonStatic.getX(),0,burgerButtonStatic.getY() - imageButtonStatic.getY());
            closeButtonAnim.setDuration(750);
            closeButtonAnim.setFillAfter(true);
            closeButtonAnim.setAnimationListener(new TranslateAnimation.AnimationListener() {

                @Override
                public void onAnimationStart(Animation animation) {
                    _isAnimating = true;
                }

                @Override
                public void onAnimationRepeat(Animation animation) {
                }

                @Override
                public void onAnimationEnd(Animation animation) {
                    _isAnimating = false;
                    imageButtonStatic.setClickable(false);
                    shareButtonStatic.setClickable(false);
                    favButtonStatic.setClickable(false);
                }
            });
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
            TranslateAnimation closeButtonAnim = new TranslateAnimation(burgerButtonStatic.getX() - imageButtonStatic.getX(),  0, burgerButtonStatic.getY() - imageButtonStatic.getY(), 0);
            closeButtonAnim.setDuration(750);
            closeButtonAnim.setFillAfter(true);
            closeButtonAnim.setAnimationListener(new TranslateAnimation.AnimationListener() {

                @Override
                public void onAnimationStart(Animation animation) {
                    _isAnimating = true;
                }

                @Override
                public void onAnimationRepeat(Animation animation) {
                }

                @Override
                public void onAnimationEnd(Animation animation) {
                    _isAnimating = false;
                    imageButtonStatic.setClickable(true);
                    shareButtonStatic.setClickable(true);
                    favButtonStatic.setClickable(true);
                }
            });
            imageButtonStatic.startAnimation(closeButtonAnim);
            TranslateAnimation favButtonAnim = new TranslateAnimation(burgerButtonStatic.getX() - favButtonStatic.getX(), 0 , burgerButtonStatic.getY() - favButtonStatic.getY(), 0);
            favButtonAnim.setDuration(750);
            favButtonAnim.setFillAfter(true);
            favButtonStatic.startAnimation(favButtonAnim);
            TranslateAnimation shareButtonAnim = new TranslateAnimation(burgerButtonStatic.getX() - shareButtonStatic.getX(), 0 , burgerButtonStatic.getY() - shareButtonStatic.getY(), 0);
            shareButtonAnim.setDuration(750);
            shareButtonAnim.setFillAfter(true);
            shareButtonStatic.startAnimation(shareButtonAnim);
            _uiExpanded = true;
        }
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
