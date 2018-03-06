package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
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
    public static ImageButton imageButtonStatic;
    private static final int _portrait = 1;
    private static final int _horizonal = 0;
    private boolean isWebViewReady = false;
    private boolean isActivityExitRequested = false;

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

        xWalkWebView = new XWalkView(this);
        addContentView(xWalkWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));

        ImageButton closeButton = new ImageButton(this);
        if(loadingGame())
            closeButton.setImageResource(R.drawable.close_button);
        else
            closeButton.setImageResource(R.drawable.back_button);

        closeButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        closeButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v) {
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

        imageButtonStatic = closeButton;
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

    @Override
    protected void onXWalkReady() {
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

        if(myUrl.substring(myUrl.length() - 4).equals("html"))
        {
            xWalkWebView.loadUrl("file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl);
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
        if (xWalkWebView != null && isWebViewReady) {
            xWalkWebView.pauseTimers();
            xWalkWebView.onHide();
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        JNICalls.JNIRegisterAppCameForegroundEvent();
        if (isWebViewReady && xWalkWebView != null) {
            xWalkWebView.resumeTimers();
            xWalkWebView.onShow();
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
}
