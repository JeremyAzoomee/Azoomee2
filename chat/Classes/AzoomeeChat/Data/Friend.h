#ifndef AzoomeeChat_Friend_h
#define AzoomeeChat_Friend_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/Data/Json.h>
#include <string>
#include <memory>


NS_AZOOMEE_CHAT_BEGIN

// forward decleration
class Friend;
typedef std::shared_ptr<Friend> FriendRef;
typedef std::vector<FriendRef> FriendList;

/**
 * A friend is a contact in the chat list.
 */
class Friend
{
private:
    
    std::string _friendId;
    std::string _friendName;
    std::string _avatarURL;
    
    // no direct construction
    Friend();
    
public:
    
    static FriendRef createFromJson(const rapidjson::Value& json);
    
    std::string friendId() const;
    std::string friendName() const;
    std::string avatarURL() const;
};

NS_AZOOMEE_CHAT_END

#endif