#ifndef Azoomee_ChatDelegate_h
#define Azoomee_ChatDelegate_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeChat/AzoomeeChat.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>


NS_AZOOMEE_BEGIN

/**
 * Global chat delegate to handle chat events
 */
class ChatDelegate : public Azoomee::Chat::Delegate, ImageDownloaderDelegate
{
public:
    
    /// Singleton instance
    static ChatDelegate* getInstance();
    
    void shareContentInChat();
    
    virtual void onChatNavigationBack() override;
    virtual void onChatAddFriend() override;
    virtual void onChatAuthorizationError(const std::string& requestTag, long errorCode) override;
    virtual void onChatNavigateToContent(const std::string& contentId) override;
    
    // delegate functions
    void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
};

NS_AZOOMEE_END

#endif
