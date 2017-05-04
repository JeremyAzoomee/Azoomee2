#include "Friend.h"



NS_AZOOMEE_CHAT_BEGIN

FriendRef Friend::createFromJson(const rapidjson::Value& json)
{
    // Parse the JSON
    if(!json.IsObject())
    {
        // Invalid JSON
        return FriendRef();
    }
    
    const std::string& friendId = json["friendId"].GetString();
    const std::string& friendName = json["friendName"].GetString();
    const std::string& avatarURL = json["avatar"].GetString();
    
    FriendRef friendData(new Friend());
    friendData->_friendId = friendId;
    friendData->_friendName = friendName;
    friendData->_avatarURL = avatarURL;
    return friendData;
}

Friend::Friend()
{
}

std::string Friend::friendId() const
{
    return _friendId;
}

std::string Friend::friendName() const
{
    return _friendName;
}

std::string Friend::avatarURL() const
{
    return _avatarURL;
}

NS_AZOOMEE_CHAT_END
