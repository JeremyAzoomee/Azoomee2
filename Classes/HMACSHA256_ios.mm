#include "HMACSHA256_ios.h"
#import <CommonCrypto/CommonHMAC.h>


using namespace cocos2d;

static HMACSHA256_ios *_sharedHMACSHA256_ios = NULL;

HMACSHA256_ios* HMACSHA256_ios::getInstance()
{
    if (! _sharedHMACSHA256_ios)
    {
        _sharedHMACSHA256_ios = new HMACSHA256_ios();
        _sharedHMACSHA256_ios->init();
    }
    
    return _sharedHMACSHA256_ios;
}

HMACSHA256_ios::~HMACSHA256_ios(void)
{
}

bool HMACSHA256_ios::init(void)
{
    return true;
}

//This method is being used on iOS only. In HelloWorldScene it is being selected depending on platform.

std::string HMACSHA256_ios::getHMACSHA256Hash(std::string message, std::string secret)
{
    
    NSString *nsMessage = [NSString stringWithCString:message.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *nsSecret = [NSString stringWithCString:secret.c_str() encoding:[NSString defaultCStringEncoding]];
    NSData *nsSecretData = [nsSecret dataUsingEncoding:NSUTF8StringEncoding];
    NSData *nsMessageData = [nsMessage dataUsingEncoding:NSUTF8StringEncoding];
    NSMutableData* hash = [NSMutableData dataWithLength:CC_SHA256_DIGEST_LENGTH ];
    CCHmac(kCCHmacAlgSHA256, nsSecretData.bytes, nsSecretData.length, nsMessageData.bytes, nsMessageData.length, hash.mutableBytes);
    NSString *base64Hash = [hash base64EncodedStringWithOptions:0];
    
    std::string result = std::string([base64Hash UTF8String]);
    
    return result;

}
