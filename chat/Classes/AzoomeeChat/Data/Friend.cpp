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
    
    // Example response:
    //{
    //    "friendId":"42cefe6d-89c5-49a5-a7c9-661c5f1ca72b",
    //    "friendName":"Yoshi",
    //    "avatar":"https://media.azoomee.com/static/thumbs/oomee_05.png",
    //    "unreadMessages":0
    //}
    
    const auto& friendIdObj = json["friendId"];
    const std::string& friendId = (friendIdObj.IsString()) ? friendIdObj.GetString() : "";
    
    const auto& friendNameObj = json["friendName"];
    const std::string& friendName = (friendNameObj.IsString()) ? friendNameObj.GetString() : "";
    
    const auto& avatarURLObj = json["avatar"];
    const std::string& avatarURL = (avatarURLObj.IsString()) ? avatarURLObj.GetString() : "";
    
    // Can't create a friend if name or id is null
    // It's ok if avatar is null
    if(friendId.empty() || friendName.empty())
    {
        return FriendRef();
    }
    
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
