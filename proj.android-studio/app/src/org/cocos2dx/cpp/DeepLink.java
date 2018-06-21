package org.cocos2dx.cpp;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.net.Uri;

public class DeepLink extends AppActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        Uri data = intent.getData();
        if(data != null) {
            sendDeepLinkToCPP(data.toString());
        }
    }

    public static native void sendDeepLinkToCPP(String URIString);

}