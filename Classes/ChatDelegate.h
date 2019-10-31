#ifndef Azoomee_ChatDelegate_h
#define Azoomee_ChatDelegate_h

#include <AzoomeeCommon/Azoomee.h>
#include "AzoomeeChat/AzoomeeChat.h"
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/UI/MessageBox.h>


NS_AZOOMEE_BEGIN

/**
 * Global chat delegate to handle chat events
 */
class ChatDelegate : public Azoomee::Chat::Delegate, ImageDownloaderDelegate, MessageBoxDelegate
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
	
	void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) override;
};

NS_AZOOMEE_END

#endif
