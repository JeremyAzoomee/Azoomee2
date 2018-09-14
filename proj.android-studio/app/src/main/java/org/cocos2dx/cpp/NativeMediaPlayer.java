package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.view.animation.TranslateAnimation;
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
    public static ImageButton burgerButtonStatic;
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
    private boolean _isAnimating = false;


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
                favButton.setImageResource(R.drawable.favourite_selected);
            } else {
                favButton.setImageResource(R.drawable.favourite_unelected);
            }
            favButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
            favButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (JNICalls.JNIIsInFavourites()) {
                        JNICalls.JNIRemoveFromFavourites();
                        favButton.setImageResource(R.drawable.favourite_unelected);
                    } else {
                        JNICalls.JNIAddToFavourites();
                        favButton.setImageResource(R.drawable.favourite_selected);
                    }
                }
            });

            favButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
            favButton.setX(buttonPadding);
            favButton.setY(buttonPadding + 2 * _buttonWidth);

            addContentView(favButton, buttonLayoutParams);

            favButtonStatic = favButton;

            if(JNICalls.JNIIsChatEntitled()) {
                final ImageButton shareButton = new ImageButton(this);
                shareButton.setImageResource(R.drawable.share_unelected);
                shareButton.setTag(R.drawable.share_unelected);
                shareButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
                shareButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {

                        Integer resource = (Integer) shareButton.getTag();
                        if (resource == R.drawable.share_selected) {
                            shareButton.setImageResource(R.drawable.share_unelected);
                            shareButton.setTag(R.drawable.share_unelected);
                        } else {
                            shareButton.setImageResource(R.drawable.share_selected);
                            shareButton.setTag(R.drawable.share_selected);
                        }
                        JNICalls.JNIShareInChat();

                        exitMediaplayer();

                    }
                });

                shareButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
                shareButton.setX(buttonPadding);
                shareButton.setY(buttonPadding + 3 * _buttonWidth);

                addContentView(shareButton, buttonLayoutParams);

                shareButtonStatic = shareButton;
            }
            else
            {
                shareButtonStatic = null;
            }

            final ImageButton burgerButton = new ImageButton(this);
            burgerButton.setImageResource(R.drawable.menu_unselected);
            burgerButton.setTag(R.drawable.menu_unselected);
            burgerButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
            burgerButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Integer resource = (Integer) burgerButton.getTag();
                    if (resource == R.drawable.menu_selected) {
                        burgerButton.setImageResource(R.drawable.menu_unselected);
                        burgerButton.setTag(R.drawable.menu_unselected);
                    } else {
                        burgerButton.setImageResource(R.drawable.menu_selected);
                        burgerButton.setTag(R.drawable.menu_selected);
                    }
                    animateButtons();
                }
            });

            burgerButton.setScaleType(android.widget.ImageView.ScaleType.FIT_START);
            burgerButton.setX(buttonPadding);
            burgerButton.setY(buttonPadding + _buttonWidth);

            addContentView(burgerButton, buttonLayoutParams);

            burgerButtonStatic = burgerButton;

            _uiExpanded = true;
            animateButtons();
        }
        final ImageButton closeButton = new ImageButton(this);
        closeButton.setImageResource(R.drawable.close_unelected);
        closeButton.setTag(R.drawable.close_unelected);

        closeButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        closeButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {

                Integer resource = (Integer)closeButton.getTag();
                if(resource == R.drawable.close_selected)
                {
                    closeButton.setImageResource(R.drawable.close_unelected);
                    closeButton.setTag(R.drawable.close_unelected);
                }
                else
                {
                    closeButton.setImageResource(R.drawable.close_selected);
                    closeButton.setTag(R.drawable.close_selected);
                }

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

    void animateButtons()
    {

        if(_isAnimating)
        {
            return;
        }

        //Ok, so android animations dont actually "move" the button, so the button is always in its expanded position,
        //and the animations are done relative to that.  when buttons are in the "closed" state, they are dissabled.
        if(_uiExpanded)
        {
            TranslateAnimation favButtonAnim = new TranslateAnimation(0,burgerButtonStatic.getX() - favButtonStatic.getX(),0,burgerButtonStatic.getY() - favButtonStatic.getY());
            favButtonAnim.setDuration(500);
            favButtonAnim.setFillAfter(true);
            favButtonAnim.setAnimationListener(new TranslateAnimation.AnimationListener() {

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
                    //imageButtonStatic.setClickable(false);
                    if(shareButtonStatic != null) {
                        shareButtonStatic.setClickable(false);
                    }
                    favButtonStatic.setClickable(false);
                }
            });
            favButtonStatic.startAnimation(favButtonAnim);
            if(shareButtonStatic != null) {
                TranslateAnimation shareButtonAnim = new TranslateAnimation(0, burgerButtonStatic.getX() - shareButtonStatic.getX(), 0, burgerButtonStatic.getY() - shareButtonStatic.getY());
                shareButtonAnim.setDuration(500);
                shareButtonAnim.setFillAfter(true);
                shareButtonStatic.startAnimation(shareButtonAnim);
            }
            _uiExpanded = false;
        }
        else
        {
            TranslateAnimation favButtonAnim = new TranslateAnimation(burgerButtonStatic.getX() - favButtonStatic.getX(), 0 , burgerButtonStatic.getY() - favButtonStatic.getY(), 0);
            favButtonAnim.setDuration(500);
            favButtonAnim.setFillAfter(true);
            favButtonAnim.setAnimationListener(new TranslateAnimation.AnimationListener() {

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
                    //imageButtonStatic.setClickable(true);
                    if(shareButtonStatic != null) {
                        shareButtonStatic.setClickable(true);
                    }
                    favButtonStatic.setClickable(true);
                }
            });
            favButtonStatic.startAnimation(favButtonAnim);
            if(shareButtonStatic != null) {
                TranslateAnimation shareButtonAnim = new TranslateAnimation(burgerButtonStatic.getX() - shareButtonStatic.getX(), 0, burgerButtonStatic.getY() - shareButtonStatic.getY(), 0);
                shareButtonAnim.setDuration(500);
                shareButtonAnim.setFillAfter(true);
                shareButtonStatic.startAnimation(shareButtonAnim);
            }
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
    }


    public void exitMediaplayer()
    {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
        _eventTimer.cancel();

        if(videoview != null && videoview.isPlaying())
        {
            videoview.stopPlayback();
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
    //Handling hardware back button

    public void onBackPressed()
    {
        exitMediaplayer();
    }
}