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
