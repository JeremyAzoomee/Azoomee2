#ifndef AzoomeeChat_AzoomeeChat_h
#define AzoomeeChat_AzoomeeChat_h

#include <string>

// Some convenience macros for namespace
#define NS_AZOOMEE_CHAT_BEGIN namespace Azoomee { namespace Chat {
#define NS_AZOOMEE_CHAT_END }}


NS_AZOOMEE_CHAT_BEGIN

// Burnt in version number for the AzoomeeChat static library
const char* const Version = "0.1.5";


// Delegate is used to deal with navigating outside the chat app
struct Delegate {
    virtual void onChatNavigationBack() = 0;
    virtual void onChatAddFriend() = 0;
    virtual void onChatAuthorizationError(const std::string& requestTag, long errorCode) = 0;
};

// Delegate instance in namespace
extern Delegate* delegate;

NS_AZOOMEE_CHAT_END

#endif
