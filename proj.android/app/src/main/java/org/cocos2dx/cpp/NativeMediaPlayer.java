package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Typeface;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.widget.TextViewCompat;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.TypedValue;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.MediaController;
import android.widget.TextView;
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
    public static ImageButton imageButtonStatic;
    public static ImageButton favButtonStatic;
    public static ImageButton shareButtonStatic;

    private ImageView circle1;
    private ImageView circle2;
    private ImageView loadingSign;

    private JSONObject playlistObject;
    private String currentlyPlayedUri;

    private float _videoTimeSent;
    private Timer _eventTimer;
    private MediaController _mediaController;

    private int _buttonWidth;
    private float _paddedWindowWidth;
    private float _paddedWindowHeight;
    private boolean _uiExpanded = false;


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
        currentlyPlayedUri = (extras != null) ? extras.getString("url") : "about:blank";

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

        int vidProgress = extras.getInt("videoProgressSeconds") * 1000;
        if(vidProgress > 0)
        {
            videoview.seekTo(vidProgress);
        }
        JNICalls.sendMediaPlayerData("video.play", "");

        _eventTimer = new Timer();
        _eventTimer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                handleVideoTimeEvents();
            }
        }, 0, 5000);//put here time 1000 milliseconds=1 second

        addLoadingScreen();
        addButtons();

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
                        JNICalls.JNISendProgressMetaDataVideo(0,videoview.getDuration() / 1000);
                        videoview.stopPlayback();
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
                else
                {
                    JNICalls.JNISendProgressMetaDataVideo(0,videoview.getDuration() / 1000);
                    currentlyPlayedUri = nextItem;
                    Uri uri = Uri.parse(nextItem);
                    videoview.setVideoURI(uri);
                    videoview.start();
                    JNICalls.JNINewVideoOpened(getCurrentItemPlaylistIndex());
                    if(!JNICalls.JNIIsAnonUser()) {
                        if (JNICalls.JNIIsInFavourites()) {
                            favButtonStatic.setImageResource(R.drawable.favourite_selected_v2);
                        } else {
                            favButtonStatic.setImageResource(R.drawable.favourite_unelected_v2);
                        }
                    }
                }
            }
        });

        videoview.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mediaPlayer, int what, int extra)
            {
                //error happened, get back to main screen with error, send event to mixpanel
                JNICalls.getBackToLoginScreen();
                onBackPressed();
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
                if(elementUri.equals(currentlyPlayedUri))
                {
                    //JNICalls.JNISendVideoProgress(i, 0);
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
                    if (JNICalls.JNIIsInFavourites()) {
                        JNICalls.JNIRemoveFromFavourites();
                        favButton.setImageResource(R.drawable.favourite_unelected_v2);
                    } else {
                        JNICalls.JNIAddToFavourites();
                        favButton.setImageResource(R.drawable.favourite_selected_v2);
                    }
                }
            });

            favButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
            favButton.setX(buttonPadding);
            favButton.setY(buttonPadding + _buttonWidth);

            addContentView(favButton, buttonLayoutParams);

            favButtonStatic = favButton;

            if(JNICalls.JNIIsChatEntitled()) {
                final ImageButton shareButton = new ImageButton(this);
                shareButton.setImageResource(R.drawable.share_unelected_v2);
                shareButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
                shareButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {

                        JNICalls.JNIShareInChat();

                        exitMediaplayer();

                    }
                });

                shareButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
                shareButton.setX(buttonPadding);
                shareButton.setY(buttonPadding + 2 * _buttonWidth);

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
                exitMediaplayer();
            }
        });
        closeButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);

        closeButton.setX(buttonPadding);
        closeButton.setY(buttonPadding);

        // Add button to screen, with Size and Position
        addContentView(closeButton, buttonLayoutParams);

        imageButtonStatic = closeButton;
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
    }

    int getCurrentItemPlaylistIndex()
    {
        try
        {
            for(int i = 0; i < playlistObject.getJSONArray("Elements").length(); i++)
            {
                String elementUri = playlistObject.getJSONArray("Elements").getJSONObject(i).getString("uri");
                if(elementUri.equals(currentlyPlayedUri))
                {
                    return i;
                }
            }
        }
        catch (JSONException e)
        {
            return 0;
        }
        return 0;
    }

    public void exitMediaplayer()
    {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
        _eventTimer.cancel();

        if(videoview != null && videoview.isPlaying())
        {
            //JNICalls.JNISendVideoProgress(getCurrentItemPlaylistIndex(), videoview.getCurrentPosition() / 1000);
            JNICalls.JNISendProgressMetaDataVideo(videoview.getCurrentPosition() / 1000,videoview.getDuration() / 1000);
            videoview.stopPlayback();
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
    //Handling hardware back button

    public void onBackPressed()
    {
        exitMediaplayer();
    }
}