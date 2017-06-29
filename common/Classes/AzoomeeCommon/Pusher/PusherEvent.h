#ifndef AzoomeeCommon_PusherEvent_h
#define AzoomeeCommon_PusherEvent_h

#include "../Azoomee.h"
#include "../Data/Json.h"
#include <string>
#include <memory>


NS_AZOOMEE_BEGIN

// forward decleration
class PusherEvent;
typedef std::shared_ptr<PusherEvent> PusherEventRef;

/**
 * A friend is a contact in the chat list.
 */
class PusherEvent
{
private:
    
    std::string _channelName;
    std::string _eventName;
    rapidjson::Document _data;
    
    // no direct construction
    PusherEvent();
    
public:
    
    /// Create a new Pusher event
    /// dataJson must be a valid json string, otherwise create will fail and return a null PusherEventRef
    static PusherEventRef create(const std::string& channelName, const std::string& eventName, const std::string& dataJson);
    
    const std::string& channelName() const;
    const std::string& eventName() const;
    const rapidjson::Value& data() const;
};

NS_AZOOMEE_END

#endif
