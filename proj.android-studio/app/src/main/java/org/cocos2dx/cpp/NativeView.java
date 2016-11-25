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
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;

import com.tinizine.azoomee.R;

import static com.loopj.android.http.AsyncHttpClient.log;

public class NativeView extends Activity {

    private static Context mContext;
    public WebView webview;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_view);

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";
        if(extras != null)
        {
            myUrl = extras.getString("url");
        }

        final View decorView = getWindow().getDecorView();



        int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_IMMERSIVE;
        decorView.setSystemUiVisibility(uiOptions);

        mContext = this;

        webview = new WebView(this);
        //setContentView(webview);
        addContentView(webview, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));

        webview.setWebViewClient(new WebViewClient());
        webview.getSettings().setJavaScriptEnabled(true);
        webview.getSettings().setAllowFileAccess(true);
        webview.getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
        webview.getSettings().setLoadsImagesAutomatically(true);
        webview.getSettings().setSupportMultipleWindows(true);
        webview.getSettings().setDomStorageEnabled(true);
        //if(Build.VERSION.SDK_INT < 21) webview.setLayerType(View.LAYER_TYPE_SOFTWARE, null);


        //webview.loadUrl("http://www.bonis.me/azoomee/game/index.html");
        webview.loadUrl(myUrl);

        Button extra = new Button(this);
        extra.setText("back");
        extra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String tag = "JniTest";
                String message = "Button was pressed";
                Log.d(tag, "Showing alert dialog: " + message);


                webview.removeAllViews();

                if(webview != null) {
                    webview.clearHistory();
                    webview.clearCache(true);
                    webview.loadUrl("about:blank");
                    //webview.pauseTimers();
                    webview = null;
                }


                //Intent ccs = new Intent(mContext, AppActivity.class);
                //ccs.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
                // mContext.startActivity(ccs);

                finish();
            }
        });

        addContentView(extra, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
    }
}
