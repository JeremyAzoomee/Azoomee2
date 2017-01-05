/*
 
 HMAC SHA256 hash encoder class for Cocos2d-X.
 HMACSHA256.cpp can be called as a singletone
 HMACSHA256::getInstance()->getHMACSHA256hash(std::string "text", std::string "secret");
 The method returns an std::string, that contains the HMAC SHA256 raw hash base64 encoded.
 
 To include this task, please move the following files to your classes folder (and add them to xcode):
 HMACSHA256.h
 HMACSHA256.cpp
 HMACSHA256_ios.h
 HMACSHA256_ios.mm
 HMACSHA256_javaCaller.h
 HMACSHA256_javaCaller.cpp
 
 Add HMACSHA256.cpp and HMACSHA256_javaCaller.cpp to proj.android-studio/app/jni/Android.mk.
 Add the following method inside proj.android-studio/app/src/org/cocos2dx/cpp/AppActivity.java 's 
 public class AppActivity extends Cocos2dxActivity:
 
     public static String getHMACSHA256(String message, String secret)
     {
         String hash = "";
         try
         {
             Mac sha256_HMAC = Mac.getInstance("HmacSHA256");
             SecretKeySpec secret_key = new SecretKeySpec(secret.getBytes(), "HmacSHA256");
             sha256_HMAC.init(secret_key);
             
             hash = Base64.encodeToString(sha256_HMAC.doFinal(message.getBytes()), Base64.DEFAULT);
         }
         catch (Exception e)
         {
         
         }
         
         return hash.trim();
     
     }
 
 
 */


#include "HMACSHA256.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "HMACSHA256_ios.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "HMACSHA256_javaCaller.h"
#endif


using namespace cocos2d;

static HMACSHA256 *_sharedHMACSHA256 = NULL;

HMACSHA256* HMACSHA256::getInstance()
{
    if (! _sharedHMACSHA256)
    {
        _sharedHMACSHA256 = new HMACSHA256();
        _sharedHMACSHA256->init();
    }
    
    return _sharedHMACSHA256;
}

HMACSHA256::~HMACSHA256(void)
{
}

bool HMACSHA256::init(void)
{
    return true;
}

std::string HMACSHA256::getHMACSHA256Hash(std::string message, std::string secret)
{
    std::string myResult = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    myResult = HMACSHA256_ios::getInstance()->getHMACSHA256Hash(message, secret);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    myResult = HMACSHA256_javaCaller::getInstance()->getHMACSHA256Hash(message, secret);
#endif
    
    
    return myResult;
}
