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
    public static String userid;
    public static ImageButton imageButtonStatic;
    private static final int _portrait = 1;
    private static final int _horizonal = 0;
    private boolean isWebViewReady = false;
    private boolean isActivityExitRequested = false;

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
        // Convert to screen co-ordinates
        final float width = uiWebView.getWidth();
        final float height = uiWebView.getHeight();
        final float xPos = width * x;
        final float yPos = height * y;
        final int touchUpAfterMs = 100;

        // Touch down
        dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(),
                                              MotionEvent.ACTION_DOWN, xPos, yPos, 0));

        // Touch up
        // TODO: Touch up should actually be called from onKeyUp instead of automatically
        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(),
                                                      MotionEvent.ACTION_UP, xPos, yPos, 0));
            }
        }, touchUpAfterMs);
    }

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

                // Touch just below centre
                performTouchEventAt( 0.5f, 0.75f );
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

                // Touch bottom left of screen
                performTouchEventAt( touchMarginX, 1.0f - touchMarginY );
                break;
            }
            case KeyEvent.KEYCODE_DPAD_RIGHT:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_RIGHT");
                handled = true;

                // Touch bottom right of screen
                performTouchEventAt( 1.0f - touchMarginX, 1.0f - touchMarginY );
                break;
            }
            case KeyEvent.KEYCODE_DPAD_UP:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_UP");
                handled = true;

                // Touch top centre of screen
                performTouchEventAt( 0.5f, touchMarginY );
                break;
            }
            case KeyEvent.KEYCODE_DPAD_DOWN:
            {
                Log.d("AzoomeeWebView", "KEYCODE_DPAD_DOWN");
                handled = true;

                // Touch bottom centre of screen
                performTouchEventAt( 0.5f, 1.0f - touchMarginY );
                break;
            }
            case KeyEvent.KEYCODE_MENU:
            {
                Log.d("AzoomeeWebView", "KEYCODE_MENU");
                handled = true;

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