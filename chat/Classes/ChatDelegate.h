#ifndef AzoomeeChat_ChatDelegate_h
#define AzoomeeChat_ChatDelegate_h

#include <AzoomeeChat/AzoomeeChat.h>


NS_AZOOMEE_CHAT_BEGIN

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

NS_AZOOMEE_CHAT_END

#endif
