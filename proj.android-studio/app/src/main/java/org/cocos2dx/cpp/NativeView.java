package org.cocos2dx.cpp;

import android.content.Context;
import android.content.ContextWrapper;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;

import com.tinizine.azoomee.R;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.HashMap;
import java.util.Map;

import org.xwalk.core.XWalkActivity;
import org.xwalk.core.XWalkView;
import org.xwalk.core.XWalkCookieManager;


import static com.loopj.android.http.AsyncHttpClient.log;

public class NativeView extends XWalkActivity {

    private static Context mContext;
    public XWalkView xWalkWebView;
    public static XWalkView xWalkWebViewStatic;
    public static String userid;

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

        Bundle extras = getIntent().getExtras();
        userid = extras.getString("userid");
        log.d("userid", userid);

        xWalkWebView = new XWalkView(this);
        addContentView(xWalkWebView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        //xWAlkWebView.load(myUrl, null, headers);



        ImageButton extra = new ImageButton(this);
        extra.setImageResource(R.drawable.back_new);
        extra.setBackgroundColor(android.graphics.Color.TRANSPARENT);
        extra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                xWalkWebView.removeAllViews();

                if(xWalkWebView != null)
                {
                    xWalkWebView.clearCache(true);
                    xWalkWebView = null;
                }

                finish();
            }
        });

        addContentView(extra, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        xWalkWebViewStatic = xWalkWebView;
    }

    @Override
    protected void onXWalkReady() {
        Bundle extras = getIntent().getExtras();
        String myUrl = "about:blank";
        String myCookieUrl = "";
        String myCookies = "";
        if(extras != null)
        {
            myUrl = extras.getString("url");
            myCookieUrl = extras.getString("cookieurl");
            myCookies = extras.getString("cookie");
            userid = extras.getString("userid");
        }

        //Check if the url received url ends with html, or anything else. If html, then we have to
        //open the html directly, otherwise we have to open the playlist with jw player.

        log.d("urlToBeLoaded", myUrl);


        if(myUrl.substring(myUrl.length() - 4).equals("html"))
        {
            xWalkWebView.load("file:///android_asset/res/webcommApi/index_android.html?contentUrl=" + myUrl, null);
        }
        else
        {
            Map<String, String> headers = new HashMap<String, String>();
            headers.put("Cookie", myCookies);

            XWalkCookieManager mCookieManager = new XWalkCookieManager();
            mCookieManager.flushCookieStore();
            mCookieManager.setAcceptCookie(true);
            mCookieManager.setAcceptFileSchemeCookies(true);

            String[] separatedCookies = myCookies.split("; ");

            for(int i = 0; i < separatedCookies.length; i++)
            {
                log.d("seaparatecookies: ", separatedCookies[i]);
                mCookieManager.setCookie(myCookieUrl, separatedCookies[i]);
            }

            log.d("cookies: ", mCookieManager.getCookie(myCookieUrl));

            xWalkWebView.load("file:///android_asset/res/jwplayer/index.html?contentUrl=" + myUrl, null);
        }

        //xWalkWebView.load("file:////android_asset/res/artapp/index.html", null);
        //xWalkWebView.load("file:////android_asset/res/webcommApi/index_android.html", null);

        xWalkWebView.addJavascriptInterface(new JsInterface(), "NativeInterface");
    }

    static File getUserDirectory()
    {
        ContextWrapper contextWrapper = new ContextWrapper(mContext);

        File scoreCacheDir = new File(contextWrapper.getApplicationInfo().dataDir + "/files/scoreCache");
        if(!scoreCacheDir.exists()) scoreCacheDir.mkdir();

        File userDir = new File(contextWrapper.getApplicationInfo().dataDir + "/files/scoreCache/" + userid);
        if(!userDir.exists()) userDir.mkdir();

        return userDir;
    }

    static File [] getFilesListFromUserDirectory()
    {
        File directory = getUserDirectory();

        log.d("Directory: ", getUserDirectory().getPath());

        return directory.listFiles();
    }

    static int getAmountOfStorageElements()
    {
        File directory = getUserDirectory();
        if(!directory.exists())
        {
            log.d("read", "directory doesn't exist");
            return 0;
        }
        else
        {
            File [] files = directory.listFiles();
            log.d("amount of files", "is " + files.length);
            return files.length;
        }
    }

    static String getKeyForStorageElement(int fileNumber)
    {
        File [] files = getFilesListFromUserDirectory();

        if(!files[fileNumber].isDirectory())
        {
            log.d("filename", files[fileNumber].getName());
            return files[fileNumber].getName().substring(0, files[fileNumber].getName().length() - 5);
        }
        return "DIR";
    }

    static String getValueForStorageElement(int fileNumber)
    {
        File [] files = getFilesListFromUserDirectory();

        if(files[fileNumber].isDirectory())
        {
            return "";
        }

        Context context = mContext;
        StringBuilder stringBuilder = new StringBuilder();
        String line;
        BufferedReader in = null;

        try {
            in = new BufferedReader(new FileReader(files[fileNumber]));
            while ((line = in.readLine()) != null) stringBuilder.append(line);

        } catch (FileNotFoundException e) {
            log.d("exception", e.getMessage());
        } catch (IOException e) {
            log.d("exception", e.getMessage());
        }
        String data = stringBuilder.toString();

        log.d("data", data);

        return data;
    }

    static void saveLocalDataForUser(String title, String data)
    {
        ContextWrapper contextWrapper = new ContextWrapper(mContext);
        String dataDir = contextWrapper.getApplicationInfo().dataDir + "/files/scoreCache";

        File directory = new File(dataDir);
        if(!directory.exists())
        {
            directory.mkdir();
        }

        String currentUserDir = dataDir + "/" + userid;
        File currentUserDirectory = new File(currentUserDir);
        if(!currentUserDirectory.exists()) currentUserDirectory.mkdir();

        String currentWritePathString = currentUserDir + "/" + title + ".json";
        File currentWritePath = new File(currentWritePathString);
        if(currentWritePath.exists()) currentWritePath.delete();

        try
        {
            currentWritePath.createNewFile();
            FileOutputStream fOut = new FileOutputStream(currentWritePath);
            OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);
            myOutWriter.write(data);

            myOutWriter.close();

            fOut.flush();
            fOut.close();
        }
        catch (IOException e)
        {
            Log.e("Exception", "File write failed: " + e.toString());
        }
    }

    static void saveImageForUser(String title, String data)
    {
        ContextWrapper contextWrapper = new ContextWrapper(mContext);
        String dataDir = contextWrapper.getApplicationInfo().dataDir + "/files/artCache";

        File directory = new File(dataDir);
        if(!directory.exists())
        {
            directory.mkdir();
        }

        String currentUserDir = dataDir + "/" + userid;
        File currentUserDirectory = new File(currentUserDir);
        if(!currentUserDirectory.exists()) currentUserDirectory.mkdir();

        String currentWritePathString = currentUserDir + "/" + title;
        File currentWritePath = new File(currentWritePathString);
        if(currentWritePath.exists()) currentWritePath.delete();

        try {
            currentWritePath.createNewFile();
            FileOutputStream fOut = new FileOutputStream(currentWritePath);
            OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);
            myOutWriter.write(data);

            myOutWriter.close();

            fOut.flush();
            fOut.close();
        }
        catch (IOException e)
        {
            Log.e("Exception", "File write failed: " + e.toString());
        }

        Log.d("File was written to", currentWritePathString);
    }
}