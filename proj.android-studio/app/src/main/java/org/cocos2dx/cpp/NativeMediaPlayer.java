package org.cocos2dx.cpp;

import android.app.Activity;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.widget.MediaController;
import android.widget.VideoView;

import com.tinizine.azoomee.R;

public class NativeMediaPlayer extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_mediaplayer);

        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";

        if(extras != null)
        {
            myUrl = extras.getString("url");
        }

        Uri uri = Uri.parse(myUrl);

        VideoView videoview= (VideoView)findViewById(R.id.videoview_concept);
        Log.i("Video URL", myUrl);
        videoview.setVideoURI(uri);
        videoview.setMediaController(new MediaController(this));
        videoview.requestFocus();
        videoview.start();
    }
}