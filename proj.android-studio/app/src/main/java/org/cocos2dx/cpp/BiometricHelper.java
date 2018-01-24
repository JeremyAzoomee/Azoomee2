package org.cocos2dx.cpp;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.KeyguardManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.fingerprint.FingerprintManager;
import android.os.Build;
import android.support.v4.app.ActivityCompat;
import android.os.CancellationSignal;
import android.util.Log;
import android.widget.Toast;


import static android.content.Context.FINGERPRINT_SERVICE;
import static android.content.Context.KEYGUARD_SERVICE;

@TargetApi(Build.VERSION_CODES.M)
public class BiometricHelper extends FingerprintManager.AuthenticationCallback {

    private static Context mContext;
    private static Biometric mSender;
    private CancellationSignal cancellationSignal;

    public BiometricHelper(Context context, Biometric sender) {
        this.mContext = context;
        this.mSender = sender;
    }

    public void startAuth(FingerprintManager manager) {
        if(cancellationSignal != null)
        {
            cancellationSignal.isCanceled();
            cancellationSignal = null;
        }

        cancellationSignal = new CancellationSignal();

        if (ActivityCompat.checkSelfPermission(mContext, Manifest.permission.USE_FINGERPRINT) != PackageManager.PERMISSION_GRANTED) {
            return;
        }

        manager.authenticate(null, cancellationSignal, 0, this, null);
    }

    public void stopAuth() {
            if (cancellationSignal != null) {
                cancellationSignal.cancel();
                cancellationSignal = null;
            }
        }


        @Override
    //onAuthenticationError is called when a fatal error has occurred. It provides the error code and error message as its parameters//
    public void onAuthenticationError(int errMsgId, CharSequence errString) {
            stopAuth();

            if(errMsgId != FingerprintManager.FINGERPRINT_ERROR_CANCELED)
            {
                Toast.makeText(mContext, "Authentication error: " + errString, Toast.LENGTH_LONG).show();
                mSender.biometricAuthenticationError();
            }
    }

    @Override
    public void onAuthenticationFailed() {
        stopAuth();
        Toast.makeText(mContext, "Authentication failed", Toast.LENGTH_LONG).show();
        mSender.biometricAuthenticationFailure();
    }

    @Override
    public void onAuthenticationHelp(int helpMsgId, CharSequence helpString) {
        stopAuth();
        Toast.makeText(mContext, "Authentication help\n" + helpString, Toast.LENGTH_LONG).show();
    }@Override

    public void onAuthenticationSucceeded(
            FingerprintManager.AuthenticationResult result) {
                stopAuth();
                Toast.makeText(mContext, "Success!", Toast.LENGTH_LONG).show();
                mSender.biometricAuthenticationSuccess();
    }
}