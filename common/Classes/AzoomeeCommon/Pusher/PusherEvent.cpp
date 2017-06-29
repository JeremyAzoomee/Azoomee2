#include "PusherEvent.h"
#include <cocos/cocos2d.h>


NS_AZOOMEE_BEGIN

PusherEventRef PusherEvent::create(const std::string& channelName, const std::string& eventName, const std::string& dataJson)
{
    // Parse the data
    rapidjson::Document response;
    response.Parse(dataJson.c_str());
    if(response.HasParseError())
    {
        // Invalid JSON
        cocos2d::log("Invalid json pusher event data: %s", dataJson.c_str());
        return nullptr;
    }
    
    PusherEventRef event(new PusherEvent());
    event->_channelName = channelName;
    event->_eventName = eventName;
    event->_data = std::move(response);
    return event;
}

PusherEvent::PusherEvent()
{
}

const std::string& PusherEvent::channelName() const
{
    return _channelName;
}

const std::string& PusherEvent::eventName() const
{
    return _eventName;
}

const rapidjson::Value& PusherEvent::data() const
{
    return _data;
}

NS_AZOOMEE_END
