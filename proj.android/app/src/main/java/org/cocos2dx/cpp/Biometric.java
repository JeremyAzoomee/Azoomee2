package org.cocos2dx.cpp;

import android.Manifest;
import android.app.KeyguardManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.fingerprint.FingerprintManager;
import android.os.Build;
import android.support.v4.app.ActivityCompat;


import static android.content.Context.FINGERPRINT_SERVICE;

public class Biometric {

    private static Context mContext;
    private BiometricHelper biometricHelper;

    public Biometric(Context context) {
        this.mContext = context;
    }

    public boolean fingerprintAuthenticationPossible()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
        {
            FingerprintManager fingerprintManager = (FingerprintManager) mContext.getSystemService(FINGERPRINT_SERVICE);
            if(fingerprintManager == null)
            {
                return false;
            }
            
            if (ActivityCompat.checkSelfPermission(mContext, Manifest.permission.USE_FINGERPRINT) == PackageManager.PERMISSION_GRANTED)
            {
                if (fingerprintManager.isHardwareDetected())
                {
                    if (fingerprintManager.hasEnrolledFingerprints())
                    {

                        return true;
                    }
                }
            }
        }

        return false;
    }

    public void startAuth()
    {
        if(!fingerprintAuthenticationPossible())
        {
            return;
        }

        FingerprintManager fingerprintManager = (FingerprintManager) mContext.getSystemService(FINGERPRINT_SERVICE);
        if(biometricHelper == null)
        {
            biometricHelper = new BiometricHelper(mContext, this);
        }

        biometricHelper.startAuth(fingerprintManager);
    }

    public void stopAuth()
    {
        if(biometricHelper == null)
        {
            return;
        }

        biometricHelper.stopAuth();
    }

    public static native void biometricAuthenticationSuccess();
    public static native void biometricAuthenticationFailure();
    public static native void biometricAuthenticationError();
}