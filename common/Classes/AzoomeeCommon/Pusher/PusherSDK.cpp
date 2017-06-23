#include "PusherSDK.h"
#include <memory>


NS_AZOOMEE_BEGIN

static std::auto_ptr<PusherSDK> sPusherSDKSharedInstance;

PusherSDK* PusherSDK::getInstance()
{
    if(!sPusherSDKSharedInstance.get())
    {
        sPusherSDKSharedInstance.reset(new PusherSDK());
    }
    return sPusherSDKSharedInstance.get();
}

PusherSDK::PusherSDK()
{
    ;
}

NS_AZOOMEE_END
