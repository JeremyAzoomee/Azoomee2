#include "Friend.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>


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
    
    // Can't create a friend if name or id is null
    // It's ok if avatar is null
    if(friendId.empty() || friendName.empty())
    {
        return FriendRef();
    }
    
    
    // Get the avatar
    const auto& avatarURLObj = json["avatar"];
    std::string avatarURL = (avatarURLObj.IsString()) ? avatarURLObj.GetString() : "";
    
    // If the avatar is empty, see if we have one already we have use
    if(avatarURL.empty())
    {
        avatarURL = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildrenById(friendId);
    }
    
    return Friend::create(friendId, friendName, avatarURL);
}

FriendRef Friend::create(const std::string& friendId, const std::string& friendName, const std::string& avatarURL)
{
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
