#ifndef AzoomeeChat_AvatarWidget_h
#define AzoomeeChat_AvatarWidget_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include "../Data/Friend.h"


NS_AZOOMEE_CHAT_BEGIN
    
class AvatarWidget : public cocos2d::ui::Layout, public ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// Image displayed if Avatar doesn't exist
    cocos2d::ui::ImageView* _avatarPlaceholder = nullptr;
    
    /// Outer frame image, sits at the very bottom
    cocos2d::ui::ImageView* _outerFrame = nullptr;
    /// Background sits on top of the frame image
    cocos2d::ui::ImageView* _background = nullptr;
    
    /// Clipping node to mask the avatar contents inside the background
    cocos2d::ClippingNode* _clippingNode = nullptr;
    /// The stencil we use to mask the avatar image
    cocos2d::DrawNode* _stencilMask = nullptr;
    /// The actual avatar image, scaled to fit the background frame
    /// Sits inside the clipping node
    cocos2d::ui::ImageView* _avatarImage = nullptr;
    
    /// Front frame to hide any clipping aliasing
    cocos2d::ui::ImageView* _frameFront = nullptr;
    
    /// Image downloader for the avatar
    ImageDownloaderRef _avatarDownloader = nullptr;
    
    
    // - ImageDownloaderDelegate
    void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    void onImageDownloadFailed() override;

protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    /// Setup to display the avatar for friend
    void setAvatarForFriend(const FriendRef& friendData, bool forceImageReload = false);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(AvatarWidget);
};

NS_AZOOMEE_CHAT_END

#endif
