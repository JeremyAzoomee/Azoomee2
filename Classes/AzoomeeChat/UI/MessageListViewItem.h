#ifndef AzoomeeChat_MessageListViewItem_h
#define AzoomeeChat_MessageListViewItem_h

#include "../AzoomeeChat.h"
#include "../Data/Message.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/ImageDownloader/RemoteImageSprite.h>

USING_NS_TZ

NS_AZ_CHAT_BEGIN
    
class MessageListViewItem : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
public:
    
    enum Alignment {
        Left,
        Right
    };
    
private:
    
    /// The frienddata this item is displayed
    MessageRef _messageData;
    /// Alignment to use for the message
    Alignment _alignment = Alignment::Left;
    /// Margin from the edge
    float _margin = 0.0f;
    
    /// Where we place the content inside the menu item
    cocos2d::ui::Layout* _contentLayout = nullptr;
    
    /// Bubble container
    cocos2d::ui::Layout* _bubbleLayout = nullptr;
    /// Message text
    cocos2d::ui::Text* _textLabel = nullptr;
    
    /// Sticker container
    cocos2d::ui::Layout* _stickerLayout = nullptr;
    /// Sticker image
    cocos2d::ui::ImageView* _stickerImage = nullptr;
    
    /// Art container
    cocos2d::ui::Layout* _artLayout = nullptr;
    /// Art image
    RemoteImageSprite* _artImage = nullptr;
    cocos2d::ClippingNode* _imageMask = nullptr;
    cocos2d::ui::Scale9Sprite* _imageStencil = nullptr;
    
    
    bool _userIsParent = false;
    
    /// Resize the item height to fit the contents
    void resizeItemContents();
    
protected:
    
    virtual void doLayout() override;
    
public:
    
    /// Set this item to display data for friend
    void setData(const MessageRef& message);
    /// Get the data this item is displaying
    MessageRef getData() const;
    
    /// Which side should the message align to?
    void setAlignment(const Alignment& alignment);
    /// Set the margin from the edge
    void setEdgeMargin(float margin);
    
    void setUserIsParent(bool isParent);
    bool getUserIsParent() const;
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(MessageListViewItem);
};

NS_AZ_CHAT_END

#endif
