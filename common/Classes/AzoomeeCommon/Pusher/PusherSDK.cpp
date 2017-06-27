#include "PusherSDK.h"
#include "../Data/Parent/ParentDataProvider.h"
#include <memory>


NS_AZOOMEE_BEGIN

static std::auto_ptr<PusherSDK> sPusherSDKSharedInstance;

void PusherSDK::initialise(const std::string& appKey)
{
    sPusherSDKSharedInstance.reset(new PusherSDK(appKey));
}

PusherSDK* PusherSDK::getInstance()
{
    return sPusherSDKSharedInstance.get();
}

PusherSDK::PusherSDK(const std::string& appKey) :
    _appKey(appKey)
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

void PusherSDK::closeAllChannels()
{
    for(const std::string& channelName : _subscribedChannels)
    {
        closeChannel(channelName);
    }
}

NS_AZOOMEE_END
