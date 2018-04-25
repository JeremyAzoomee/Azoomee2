package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.MediaController;
import android.widget.VideoView;

import com.tinizine.azoomee.R;

import java.io.UnsupportedEncodingException;

public class NativeMediaPlayer extends Activity {

    private static Context mContext;
    private VideoView videoview;
    private static Activity mActivity;

    private ImageView circle1;
    private ImageView circle2;
    private ImageView loadingSign;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_mediaplayer);

        mContext = this;
        mActivity = this;


        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        if(extras != null)
        {
            myUrl = extras.getString("url");
        }

        Uri uri = Uri.parse(myUrl);

        getVideoPlaylist();

        videoview= (VideoView)findViewById(R.id.videoview_concept);
        Log.i("Video URL", myUrl);
        videoview.setVideoURI(uri);
        videoview.setMediaController(new MediaController(this));
        videoview.requestFocus();
        videoview.start();

        addLoadingScreen();
        addExitButton();

        videoview.setOnInfoListener(new MediaPlayer.OnInfoListener()
        {
            @Override
            public boolean onInfo(MediaPlayer mp, int what, int extra)
            {

                if (what == MediaPlayer.MEDIA_INFO_VIDEO_RENDERING_START)
                {
                    // Here the video starts
                    removeLoadingScreen();
                    return true;
                }

                return false;
            }
        });
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

        closeButton.setX(buttonWidth / 8);
        closeButton.setY(buttonWidth / 8);

        // Add button to screen, with Size and Position
        addContentView(closeButton, buttonLayoutParams);
    }

    //add loading screen functions --------------------------------------------------------------------------------------------------

    void addLoadingScreen()
    {
        circle1 = createAndAddLoadingScreenCircle(0.5, 360.0f);
        circle2 = createAndAddLoadingScreenCircle(0.42, -360.0f);
        loadingSign = createAndAddLoadingSign();
    }

    void removeLoadingScreen()
    {
        if(circle1.getVisibility() != View.GONE && circle2.getVisibility() != View.GONE && loadingSign.getVisibility() != View.GONE)
        {
            circle1.clearAnimation();
            circle2.clearAnimation();

            circle1.setVisibility(View.GONE);
            circle2.setVisibility(View.GONE);
            loadingSign.setVisibility(View.GONE);
        }
    }

    ImageView createAndAddLoadingScreenCircle(double screenHeightMultiplier, float endAngle)
    {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int height = displayMetrics.heightPixels;
        int width = displayMetrics.widthPixels;

        ImageView imageView = new ImageView(this);
        imageView.setImageResource(R.drawable.circle_1);

        android.widget.RelativeLayout.LayoutParams imageLayoutParams = new android.widget.RelativeLayout.LayoutParams(
                android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT, android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT);

        imageLayoutParams.leftMargin = 0;
        imageLayoutParams.topMargin = 0;
        imageLayoutParams.width = (int)(height * screenHeightMultiplier);
        imageLayoutParams.height = (int)(height * screenHeightMultiplier);

        imageView.setX(width / 2 - imageLayoutParams.width / 2);
        imageView.setY(height / 2 - imageLayoutParams.height / 2);

        RotateAnimation anim = new RotateAnimation(0.0f, endAngle, width / 2, height / 2);    //0f, 360f, 15f, 15f);
        anim.setInterpolator(new LinearInterpolator());
        anim.setRepeatCount(Animation.INFINITE);
        anim.setDuration(4000 + (long)(Math.random() * 3000));
        anim.setRepeatCount(1000);

        imageView.startAnimation(anim);

        addContentView(imageView, imageLayoutParams);

        return imageView;
    }

    ImageView createAndAddLoadingSign()
    {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int height = displayMetrics.heightPixels;
        int width = displayMetrics.widthPixels;

        ImageView imageView = new ImageView(this);
        imageView.setImageResource(R.drawable.load);

        android.widget.RelativeLayout.LayoutParams imageLayoutParams = new android.widget.RelativeLayout.LayoutParams(
                android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT, android.widget.RelativeLayout.LayoutParams.WRAP_CONTENT);

        imageLayoutParams.leftMargin = 0;
        imageLayoutParams.topMargin = 0;
        imageLayoutParams.width = (int)(height * 0.2);
        imageLayoutParams.height = (int)(height * 0.2);

        imageView.setX(width / 2 - imageLayoutParams.width / 2);
        imageView.setY(height / 2 - imageLayoutParams.height / 2);

        addContentView(imageView, imageLayoutParams);

        return imageView;
    }

    //Get and create playlist array ---------------------------------------------------------------------------------------------------------
    void getVideoPlaylist()
    {
        byte[] data = Base64.decode(JNICalls.JNIGetVideoPlaylist(), Base64.DEFAULT);
        try {
            String playlistString = new String(data, "UTF-8");
            Log.d("playlist", playlistString);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
    }
}