package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.MediaController;
import android.widget.VideoView;

import com.tinizine.azoomee.R;

public class NativeMediaPlayer extends Activity {

    private static Context mContext;
    private VideoView videoview;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_mediaplayer);

        mContext = this;

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        if(extras != null)
        {
            myUrl = extras.getString("url");
        }

        Uri uri = Uri.parse(myUrl);

        videoview= (VideoView)findViewById(R.id.videoview_concept);
        Log.i("Video URL", myUrl);
        videoview.setVideoURI(uri);
        videoview.setMediaController(new MediaController(this));
        videoview.requestFocus();
        videoview.start();

        addExitButton();
    }

    void addExitButton()
    {
        ImageButton closeButton = new ImageButton(this);
        closeButton.setImageResource(R.drawable.back_button);

        closeButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        closeButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {

                getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                        WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);


                if(videoview != null && videoview.isPlaying())
                {
                    videoview.stopPlayback();
                }

                finish();
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

        float paddedWindowWidth = metrics.widthPixels - (buttonWidth * 1.25f);
        float paddedWindowHeight = metrics.heightPixels - (buttonWidth * 1.25f);

        closeButton.setX(buttonWidth / 8);
        closeButton.setY(buttonWidth / 8);

        // Add button to screen, with Size and Position
        addContentView(closeButton, buttonLayoutParams);
    }
}