package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
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

import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.util.Timer;
import java.util.TimerTask;

public class NativeMediaPlayer extends Activity {

    private static Context mContext;
    private VideoView videoview;
    private static Activity mActivity;

    private ImageView circle1;
    private ImageView circle2;
    private ImageView loadingSign;

    private JSONObject playlistObject;
    private String currentlyPlayedUri;

    private float _videoTimeSent;
    private Timer _eventTimer;
    private MediaController _mediaController;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_mediaplayer);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        mContext = this;
        mActivity = this;

        Bundle extras = getIntent().getExtras();
        currentlyPlayedUri = "about:blank";

        if(extras != null)
        {
            currentlyPlayedUri = extras.getString("url");
        }

        Uri uri = Uri.parse(currentlyPlayedUri);

        playlistObject = getVideoPlaylist();

        videoview = findViewById(R.id.videoview_concept);
        Log.i("Video URL", currentlyPlayedUri);
        videoview.setVideoURI(uri);
        _mediaController = new MediaController(this);
        _mediaController.setAnchorView(videoview);
        videoview.setMediaController(_mediaController);
        videoview.requestFocus();
        videoview.start();

        JNICalls.sendMediaPlayerData("video.play", "");

        _eventTimer = new Timer();
        _eventTimer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                handleVideoTimeEvents();
            }
        }, 0, 1000);//put here time 1000 milliseconds=1 second

        addLoadingScreen();
        addExitButton();

        //Adding player listeners ------------------------------------------------------------------

        videoview.setOnInfoListener(new MediaPlayer.OnInfoListener()
        {
            @Override
            public boolean onInfo(MediaPlayer mp, int what, int extra)
            {

                if (what == MediaPlayer.MEDIA_INFO_VIDEO_RENDERING_START)
                {
                    // Here the video starts, send event to mixpanel
                    removeLoadingScreen();
                    _videoTimeSent = -1.0f;
                    return true;
                }

                return false;
            }
        });

        videoview.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mediaPlayer)
            {
                Log.d("playlist event", "100pc passed");

                //current item finished, sent event to mixpanel
                JNICalls.sendMediaPlayerData("video.complete", "");

                String nextItem = getUrlForNextPlaylistItem();

                if(nextItem.equals(""))
                {
                    //playlist finished, send event to mixpanel
                    JNICalls.sendMediaPlayerData("video.playlistcomplete", "");

                    getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                            WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);


                    if(videoview != null && videoview.isPlaying())
                    {
                        videoview.stopPlayback();
                    }

                    finish();
                }
                else
                {
                    Uri uri = Uri.parse(nextItem);
                    videoview.setVideoURI(uri);
                    videoview.start();
                }
            }
        });

        videoview.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mediaPlayer, int what, int extra)
            {
                //error happened, get back to main screen with error, send event to mixpanel

                return true;
            }
        });
    }

    //Handling time based events in a separate method, that is being scheduled in creation method

    void handleVideoTimeEvents()
    {
        if(videoview == null || !videoview.isPlaying())
        {
            return;
        }

        int currentDuration = videoview.getDuration();
        int currentPosition = videoview.getCurrentPosition();

        float playbackRatio = (float)currentPosition / (float)currentDuration;

        if(playbackRatio > 0.0f && _videoTimeSent < 0.0f)
        {
            _videoTimeSent = 0.0f;
            Log.d("playlist event", "0pc passed");
            JNICalls.sendMediaPlayerData("video.time", "0");
        }

        if(playbackRatio > 0.25f && _videoTimeSent < 0.25f)
        {
            _videoTimeSent = 0.25f;
            Log.d("playlist event", "25pc passed");
            JNICalls.sendMediaPlayerData("video.time", "25");
        }

        if(playbackRatio > 0.5f && _videoTimeSent < 0.5f)
        {
            _videoTimeSent = 0.5f;
            Log.d("playlist event", "50pc passed");
            JNICalls.sendMediaPlayerData("video.time", "50");
        }

        if(playbackRatio > 0.75f && _videoTimeSent < 0.75f)
        {
            _videoTimeSent = 0.75f;
            Log.d("playlist event", "75pc passed");
            JNICalls.sendMediaPlayerData("video.time", "75");
        }

    }

    //Adding other elements to the screen

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
                _eventTimer.cancel();

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

    //Get, create and handle playlist array ---------------------------------------------------------------------------------------------------------
    JSONObject getVideoPlaylist()
    {
        String playlistString = "";
        byte[] data = Base64.decode(JNICalls.JNIGetVideoPlaylist(), Base64.DEFAULT);

        try
        {
            playlistString = new String(data, "UTF-8");
        }
        catch (UnsupportedEncodingException e)
        {
            e.printStackTrace();
        }

        JSONObject generatedPlaylistObject;

        try
        {
            generatedPlaylistObject = new JSONObject(playlistString);
        }
        catch (JSONException e)
        {
            return null;
        }

        return generatedPlaylistObject;
    }

    String getUrlForNextPlaylistItem()
    {
        try
        {
            for(int i = 0; i < playlistObject.getJSONArray("Elements").length(); i++)
            {
                String elementUri = playlistObject.getJSONArray("Elements").getJSONObject(i).getString("uri");
                Log.d("playlist element", elementUri);
                if(elementUri.equals(currentlyPlayedUri))
                {
                    if(i < playlistObject.getJSONArray("Elements").length())
                    {
                        String returnString = playlistObject.getJSONArray("Elements").getJSONObject(i + 1).getString("uri");
                        return returnString;
                    }
                    else
                    {
                        return "";
                    }
                }
            }
        }
        catch (JSONException e)
        {
            return "";
        }
        return "";
    }

    //Handling hardware back button

    public void onBackPressed()
    {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
        _eventTimer.cancel();

        if(videoview != null && videoview.isPlaying())
        {
            videoview.stopPlayback();
        }

        finish();

    }
}