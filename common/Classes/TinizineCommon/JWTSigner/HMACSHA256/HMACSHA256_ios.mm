#include "HMACSHA256.h"
#import <CommonCrypto/CommonHMAC.h>
#import <Foundation/NSString.h>
#import <Foundation/NSData.h>

using namespace cocos2d;


NS_TZ_BEGIN

std::string HMACSHA256::getHMACSHA256Hash(const std::string& message, const std::string& secret)
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
  
NS_TZ_END
