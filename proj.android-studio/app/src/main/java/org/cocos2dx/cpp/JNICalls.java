package org.cocos2dx.cpp;

public class JNICalls {
    public static native void getBackToLoginScreen();
    public static native void sendMediaPlayerData(String eventKey, String eventValue);
    public static native void JNIRegisterAppWentBackgroundEvent();
    public static native void JNIRegisterAppCameForegroundEvent();
    public static native void JNIRegisterAndroidSceneChangeEvent();
    public static native String JNIGetAllCookies();

    public static native void JNISaveLocalDataStorage(String data);
    public static native String JNIGetLocalDataStorage();
    public static native String JNISendAPIRequest(String method, String responseID, String sendData);
    public static native String JNIGetVideoPlaylist();
    public static native String JNIGetRemoteWebGameAPIPath();
    public static native String JNIGetStringForKey(String key);

    public static native void JNIAddToFavourites();
    public static native void JNIRemoveFromFavourites();
    public static native boolean JNIIsInFavourites();
    public static native void JNIShareInChat();
    public static native boolean JNIIsChatEntitled();
    public static native boolean JNIIsAnonUser();
}