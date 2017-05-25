#ifndef AzoomeeChat_AvatarWidget_h
#define AzoomeeChat_AvatarWidget_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class AvatarWidget : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
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

protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the URL to use for the avatar
    void setAvatarURL(const std::string& avatarURL);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(AvatarWidget);
};

NS_AZOOMEE_CHAT_END

#endif
