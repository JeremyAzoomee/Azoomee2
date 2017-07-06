#ifndef Azoomee_ChatDelegate_h
#define Azoomee_ChatDelegate_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeChat/AzoomeeChat.h>


NS_AZOOMEE_BEGIN

/**
 * Global chat delegate to handle chat events
 */
class ChatDelegate : public Azoomee::Chat::Delegate
{
public:
    
    /// Singleton instance
    static ChatDelegate* getInstance();
    
    
    virtual void onChatNavigationBack() override;
    virtual void onChatAddFriend() override;
};

NS_AZOOMEE_END

#endif
