#ifndef AzoomeeChat_AzoomeeChat_h
#define AzoomeeChat_AzoomeeChat_h

#include <string>
#include <AzoomeeCommon/Tinizine.h>

// Some convenience macros for namespace
#define NS_AZ_CHAT_BEGIN namespace AZ { namespace Chat {
#define NS_AZ_CHAT_END }}

USING_NS_TZ

NS_AZ_CHAT_BEGIN

// Burnt in version number for the AzoomeeChat static library
const char* const Version = "0.1.5";


// Delegate is used to deal with navigating outside the chat app
struct Delegate {
    virtual void onChatNavigationBack() = 0;
    virtual void onChatAddFriend() = 0;
    virtual void onChatAuthorizationError(const std::string& requestTag, long errorCode) = 0;
	virtual void onChatOfflineError(const std::string& requestTag) = 0;
    virtual void onChatNavigateToContent(const std::string & contentId) = 0;
    
    std::string _imageFileName = "";
    std::string _sharedContentId = "";
    bool _sharingOomee = false;
};

// Delegate instance in namespace
extern Delegate* delegate;

NS_AZ_CHAT_END

#endif
