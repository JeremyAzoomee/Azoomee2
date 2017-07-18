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
    // Unsubscribe from the previous account channel
    closeParentAccountChannel();
    
    ParentDataProvider* parentData = ParentDataProvider::getInstance();
    _subscribedAccountChannel = "private-" + parentData->getLoggedInParentId();
    subscribeToChannel(_subscribedAccountChannel);
}

void PusherSDK::closeParentAccountChannel()
{
    if(!_subscribedAccountChannel.empty())
    {
        closeChannel(_subscribedAccountChannel);
        _subscribedAccountChannel = "";
    }
}

void PusherSDK::closeAllChannels()
{
    for(const std::string& channelName : _subscribedChannels)
    {
        closeChannel(channelName);
    }
}

#pragma mark - Observers

void PusherSDK::registerObserver(PusherEventObserver* observer)
{
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if(it == _observers.end())
    {
        _observers.push_back(observer);
    }
}

void PusherSDK::removeObserver(PusherEventObserver* observer)
{
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if(it != _observers.end())
    {
        _observers.erase(it);
    }
}

void PusherSDK::notifyObservers(const PusherEventRef& event)
{
    for(PusherEventObserver* observer : _observers)
    {
        observer->onPusherEventRecieved(event);
    }
}

NS_AZOOMEE_END
