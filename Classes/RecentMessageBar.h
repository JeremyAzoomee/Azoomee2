//
//  RecentMessageBar.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 28/10/2019.
//

#ifndef RecentMessageBar_h
#define RecentMessageBar_h

#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include "AzoomeeChat/Data/Friend.h"
#include "AzoomeeChat/Data/Message.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

typedef std::pair<Chat::FriendRef, Chat::MessageRef> RecentMessage;

class RecentMessageBar : public cocos2d::ui::Layout, ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(Chat::FriendRef)> MessageSelectedCallback;
private:
    
    static const cocos2d::Vec2 kDropShadowPadding;
    static const float kTextPadding;
    static const cocos2d::Vec2 kAvatarTileAspectRatio;
    
    cocos2d::ui::ImageView* _dropShadow = nullptr;
    cocos2d::ui::Layout* _avatarClip = nullptr;
    cocos2d::ui::ImageView* _avatar = nullptr;
    RoundedRectSprite* _pattern = nullptr;
    RoundedRectSprite* _avatarBG = nullptr;
    cocos2d::ui::Layout* _textLayout = nullptr;
    DynamicText* _senderName = nullptr;
    DynamicText* _messageText = nullptr;
    
    RecentMessage _message;
    
    ImageDownloaderRef _imageDownloader = nullptr;
    
    MessageSelectedCallback _callback = nullptr;
    
    void resizeImageAndText();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setMessageData(const RecentMessage& message);
    
    void setMessageSelectedCallback(const MessageSelectedCallback& callback);
    
    void setBackgroundColour(const cocos2d::Color3B& colour);
    
    CREATE_FUNC(RecentMessageBar);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZ_END
#endif /* RecentMessageBar_h */
