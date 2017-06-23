#include "PusherSDK.h"
#include "../Data/Parent/ParentDataProvider.h"
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

void PusherSDK::openParentAccountChannel()
{
    ParentDataProvider* parentData = ParentDataProvider::getInstance();
    subscribeToChannel("private-" + parentData->getLoggedInParentId());
}

void PusherSDK::closeParentAccountChannel()
{
    ParentDataProvider* parentData = ParentDataProvider::getInstance();
    closeChannel("private-" + parentData->getLoggedInParentId());
}

NS_AZOOMEE_END
