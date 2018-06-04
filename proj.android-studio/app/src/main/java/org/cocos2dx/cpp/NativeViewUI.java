package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
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
    public static String _userid;
    public static ImageButton imageButtonStatic;
    private static final int _portrait = 1;
    private static final int _horizonal = 0;
    private boolean isWebViewReady = false;
    private boolean isActivityExitRequested = false;
    private String _contentURL = "";

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

        _userid = extras.getString("userid");
        _contentURL = extras.getString("url");
        Log.d("userid", _userid);

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

        closeButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);

        closeButton.setX(buttonWidth/8);
        closeButton.setY(buttonWidth/8);

        // Add button to screen, with Size and Position
        addContentView(closeButton, buttonLayoutParams);

        uiWebView.resumeTimers();
        uiWebView.loadUrl("about:blank");
        uiWebView.clearCache(true);
        uiWebView.clearHistory();

        if(extras.getBoolean("remoteDebugWebViewEnabled"))
        {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                Log.d("REMOTEDEBUG", "!!! ATTENTION: REMOTE DEBUGGING IS ON !!!");
                uiWebView.setWebContentsDebuggingEnabled(true);
            }
        }

        imageButtonStatic = closeButton;
        uiWebViewStatic = uiWebView;

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

    private boolean loadingGame()
    {
        return _contentURL.toLowerCase().contains("html");
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
        Log.d("urlToBeLoaded", _contentURL);
        final String urlToBeLoaded;

        if(_contentURL.endsWith("html"))
        {
            if(_contentURL.startsWith("http")) //content is game loaded remotely
            {
                urlToBeLoaded = JNICalls.JNIGetRemoteWebGameAPIPath() + "index_android.html?contentUrl=" + _contentURL;
            }
            else //game is loaded locally
            {
                urlToBeLoaded = "file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + _contentURL;
            }
        }
        else
        {
            urlToBeLoaded = "file:///android_asset/res/jwplayer/index_android.html?contentUrl=" + _contentURL;
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

    @Override
    public boolean dispatchKeyEvent(KeyEvent event)
    {
        Log.d("AzoomeeWebView", "dispatchKeyEvent: " + event.getAction() + ", keyCode=" + event.getKeyCode());
        if(event.getAction() == KeyEvent.ACTION_DOWN)
        {
            onKeyDown(event.getKeyCode(), event);
            return true;
        }
        return false;
    }

    /// Perform a touch event.
    /// x and y are normalized, in 0-1 range.
    private void performTouchEventAt( final float x, final float y )
    {
        performTouchEventAt( x, y, 0 );
    }

    private void performTouchEventAt( final float x, final float y, final int touchDownAfterMs )
    {
        // Convert to screen co-ordinates
        final float width = uiWebView.getWidth();
        final float height = uiWebView.getHeight();
        final float xPos = width * x;
        final float yPos = height * y;
        final int touchUpAfterMs = 100 + touchDownAfterMs;

        // Touch down
        final Handler handlerDown = new Handler();
        handlerDown.postDelayed(new Runnable() {
            @Override
            public void run() {
                Log.d("AzoomeeWebView", "performTouchEventAt: DOWN " + x + ", " + y + ", abs: " + xPos + ", " + yPos);
                dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(),
                                                      MotionEvent.ACTION_DOWN, xPos, yPos, 0));
            }
        }, touchDownAfterMs);

        // Touch up
        // TODO: Touch up should actually be called from onKeyUp instead of automatically
        final Handler handlerUp = new Handler();
        handlerUp.postDelayed(new Runnable() {
            @Override
            public void run() {
                dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(),
                                                      MotionEvent.ACTION_UP, xPos, yPos, 0));
            }
        }, touchUpAfterMs);
    }

    /// Perform a key event.
    /// x and y are normalized, in 0-1 range.
    private void performKeyEventAt( final int keyCode )
    {
        performKeyEventAt( keyCode, 0 );
    }

    private void performKeyEventAt( final int keyCode, final int keyDownAfterMs )
    {
        final int keyUpAfterMs = 100 + keyDownAfterMs;

        // Key down
        final Handler handlerDown = new Handler();
        handlerDown.postDelayed(new Runnable() {
            @Override
            public void run() {
                Log.d("AzoomeeWebView", "performKeyEventAt: DOWN " + keyCode);
                dispatchKeyEvent(new KeyEvent(KeyEvent.ACTION_DOWN, keyCode));
            }
        }, keyDownAfterMs);

        // Key up
        // TODO: Key up should actually be called from onKeyUp instead of automatically
        final Handler handlerUp = new Handler();
        handlerUp.postDelayed(new Runnable() {
            @Override
            public void run() {
                dispatchKeyEvent(new KeyEvent(KeyEvent.ACTION_UP, keyCode));
            }
        }, keyUpAfterMs);
    }

    private static String kGameIDOomeeCake = "e320831d-4e10-489f-a6f3-7f73e156cdf2";
    private static String kGameIDPoloInTheClouds = "989d8152aaea4383b849bff53f39da65";
    private static String kGameIDThePirateKid = "2f149233-b31d-41ef-87b1-5d34d132d645";
    private static String kGameIDExtremeKitten = "39580544-d61b-48ed-a373-0d777c8afe47";
    private static String kGameIDRacingMonsterTrucks = "1fe791cf-b744-4a7a-8e7d-677d30d16a25";
    private static String kGameIDGrandPrixHero = "8eef2d3b-a3c7-459b-9f8a-89e54eb8b312";
    private static String kGameIDIndiaraSkullGold = "e31bee733df24ba78296d1169e666c22";
    private static String kGameIDHighwayRiderExtreme = "11fcb2a17b5c429a952454aec288101a";
    private static String kGameIDNimbleBoxes = "984050c2-55f1-4029-9d54-d84169ac25ac";
    private static String kGameIDOomeeRush = "7e02bdf5-0f5e-4125-8fd3-a01da19fcd20";
    private static String kGameIDYetiSensation = "5c05494a-1d3e-49e4-98fe-fea1560e05f7";
    private static String kGameIDPrincessGoldbladeDangerousWater = "11196265ad8346789f3a91354537c325";


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        boolean handled = false;

        Log.d("AzoomeeWebView", "onKeyDown: " + keyCode);

        final float touchMarginX = 0.052f;
        final float touchMarginY = 0.111f;

        switch(keyCode)
        {
            case KeyEvent.KEYCODE_DPAD_CENTER:
            case KeyEvent.KEYCODE_BUTTON_A:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_CENTER");
                handled = true;

                if(_contentURL.contains(kGameIDPoloInTheClouds))
                {
                    Log.d("AzoomeeWebView", "GameIDPoloInTheClouds");
                    performTouchEventAt( 0.5f, 0.5f );
                }
                else if(_contentURL.contains(kGameIDGrandPrixHero))
                {
                    // performTouchEventAt( 0.5f, 0.5f ); - toggle sound
                    performTouchEventAt( 0.8f, 0.8f );
                    performTouchEventAt( 0.5f, 0.35f, 150 );
                }
                else if(_contentURL.contains(kGameIDRacingMonsterTrucks))
                {
                    performTouchEventAt( 0.5f, 0.6f );
                }
                else if(_contentURL.contains(kGameIDPrincessGoldbladeDangerousWater))
                {
                    // performKeyEventAt(KeyEvent.KEYCODE_C);
                    performTouchEventAt( 0.25f, 0.75f );
                    performTouchEventAt( 0.8f, 0.8f, 150 );
                }
                // else if(_contentURL.contains(kGameIDThePirateKid))
                // {
                //     Log.d("AzoomeeWebView", "GameIDThePirateKid");
                //     performTouchEventAt( 0.9f, 0.8f );
                // }
                // else if(_contentURL.contains(kGameIDExtremeKitten))
                // {
                //     Log.d("AzoomeeWebView", "GameIDExtremeKitten");
                //     performTouchEventAt( 0.5f, 0.75f );
                //     performTouchEventAt( 0.9f, 0.8f );
                // }
                else
                {
                    // Touch just below centre
                    performTouchEventAt( 0.5f, 0.75f );
                }
                break;
            }
            case KeyEvent.KEYCODE_BACK:
            {
                Log.d("AzoomeeWebView", "KEYCODE_BACK");
                handled = true;

                cleanUpAndFinishActivity();
                break;
            }
            case KeyEvent.KEYCODE_DPAD_LEFT:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_LEFT");
                handled = true;

                if(_contentURL.contains(kGameIDRacingMonsterTrucks))
                {
                    // Back
                    performTouchEventAt( 0.1f, 1.0f - touchMarginY );
                }
                else if(_contentURL.contains(kGameIDOomeeCake) || _contentURL.contains(kGameIDOomeeRush))
                {
                    performTouchEventAt( 0.45f, 0.6f );
                }
                else if(_contentURL.contains(kGameIDYetiSensation))
                {
                    performTouchEventAt( 0.42f, 0.75f );
                }
                else if(_contentURL.contains(kGameIDPrincessGoldbladeDangerousWater))
                {
                    performTouchEventAt( 0.1f, 0.75f );
                    // performKeyEventAt(37);
                }
                else
                {
                    // Touch bottom left of screen
                    performTouchEventAt( touchMarginX, 1.0f - touchMarginY );
                }
                break;
            }
            case KeyEvent.KEYCODE_DPAD_RIGHT:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_RIGHT");
                handled = true;

                if(_contentURL.contains(kGameIDThePirateKid) || _contentURL.contains(kGameIDExtremeKitten))
                {
                    performTouchEventAt( 0.9f, 0.8f );
                }
                else if(_contentURL.contains(kGameIDRacingMonsterTrucks) || _contentURL.contains(kGameIDGrandPrixHero))
                {
                    performTouchEventAt( 0.8f, 0.8f );
                }
                else if(_contentURL.contains(kGameIDOomeeCake) || _contentURL.contains(kGameIDOomeeRush))
                {
                    performTouchEventAt( 0.55f, 0.6f );
                }
                else if(_contentURL.contains(kGameIDYetiSensation))
                {
                    performTouchEventAt( 0.55f, 0.75f );
                }
                else if(_contentURL.contains(kGameIDPrincessGoldbladeDangerousWater))
                {
                    performTouchEventAt( 0.4f, 0.75f );
                    // performKeyEventAt(39);
                }
                else
                {
                    // Touch bottom right of screen
                    performTouchEventAt( 1.0f - touchMarginX, 1.0f - touchMarginY );
                }
                break;
            }
            case KeyEvent.KEYCODE_DPAD_UP:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_UP");
                handled = true;

                if(_contentURL.contains(kGameIDGrandPrixHero))
                {
                    // upgrade buttons - just click all at once
                    performTouchEventAt( 0.9f, 0.366f );
                    performTouchEventAt( 0.9f, 0.466f, 150 );
                    performTouchEventAt( 0.9f, 0.566f, 300 );
                    performTouchEventAt( 0.9f, 0.666f, 450 );

                    // pause menu - resume
                    performTouchEventAt( 0.5f, 0.35f, 600 );

                    // 31 height | 30 height
                    // 11 = 0.354 | 0.366
                    // 14 = 0.451 | 0.466
                    // 17 = 0.548 | 0.566
                    // 20 = 0.645 | 0.666
                    // 26.25 = 
                }
                else if(_contentURL.contains(kGameIDRacingMonsterTrucks))
                {
                    // Accelerator pedal
                    performTouchEventAt( 0.8f, 0.5f );
                }
                else if(_contentURL.contains(kGameIDPrincessGoldbladeDangerousWater))
                {
                    performTouchEventAt( 0.5f, 0.2f );
                    // performKeyEventAt(KeyEvent.KEYCODE_X);
                }
                else
                {
                    // Touch top centre of screen
                    performTouchEventAt( 0.5f, touchMarginY );
                }
                break;
            }
            case KeyEvent.KEYCODE_DPAD_DOWN:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_DOWN");
                handled = true;

                if(_contentURL.contains(kGameIDGrandPrixHero))
                {
                    // main menu - how to play
                    performTouchEventAt( 0.5f, 0.8f );
                    // pause menu - how to play
                    performTouchEventAt( 0.5f, 0.66f, 150 );
                }
                else
                {
                    // Touch bottom centre of screen
                    performTouchEventAt( 0.5f, 1.0f - touchMarginY );
                }
                break;
            }
            case KeyEvent.KEYCODE_MENU:
            {
                Log.d("AzoomeeWebView", "KEYCODE_MENU");
                handled = true;

                Log.d("AzoomeeWebView", "Current URL: " + uiWebView.getUrl());

                // Touch top right of screen
                performTouchEventAt( 1.0f - touchMarginX, touchMarginY );
                break;
            }
            case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
            {
                Log.d("AzoomeeWebView", "KEYCODE_MEDIA_PLAY_PAUSE");
                handled = true;

                // Touch just below centre
                performTouchEventAt( 0.5f, 0.75f );
                // And allow restart on Cube Ninja (special case) just for demo purposes, we'll remove this later
                performTouchEventAt( 0.715f, 0.667f );
                break;
            }
            case KeyEvent.KEYCODE_MEDIA_REWIND:
            {
                Log.d("AzoomeeWebView", "KEYCODE_MEDIA_REWIND");
                handled = true;
                break;
            }
            case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
            {
                Log.d("AzoomeeWebView", "KEYCODE_MEDIA_FAST_FORWARD");
                handled = true;
                break;
            }
        }

        return handled || super.onKeyDown(keyCode, event);
    }
}