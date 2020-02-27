#ifndef Azoomee_ChatDelegate_h
#define Azoomee_ChatDelegate_h

#include <TinizineCommon/Tinizine.h>
#include "AzoomeeChat/AzoomeeChat.h"
#include <TinizineCommon/ImageDownloader/ImageDownloader.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

/**
 * Global chat delegate to handle chat events
 */
class ChatDelegate : public AZ::Chat::Delegate, ImageDownloaderDelegate
{
public:
    
    /// Singleton instance
    static ChatDelegate* getInstance();
    
    void shareContentInChat();
    
    virtual void onChatNavigationBack() override;
    virtual void onChatAddFriend() override;
    virtual void onChatAuthorizationError(const std::string& requestTag, long errorCode) override;
    virtual void onChatNavigateToContent(const std::string& contentId) override;
	virtual void onChatOfflineError(const std::string& requestTag) override;
    
    // delegate functions
    void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    void onImageDownloadFailed() override;
};

NS_AZ_END

#endif
