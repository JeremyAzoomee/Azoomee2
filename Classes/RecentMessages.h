//
//  RecentMessages.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 25/10/2019.
//

#ifndef RecentMessages_h
#define RecentMessages_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include "RecentMessageBar.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class RecentMessages : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(Chat::FriendRef)> MessageSelectedCallback;
private:
    
    static const float kListViewPadding;
    static const float kHeaderHeight;
    static const float kMessageBarSpacing;
    
    RoundedRectSprite* _background = nullptr;
    
    cocos2d::ui::Layout* _contentLayout = nullptr;
    cocos2d::ui::Layout* _headerLayout = nullptr;
    DynamicText* _headerText = nullptr;
    RoundedRectSprite* _bannerGradient = nullptr;
    cocos2d::LayerGradient* _bannerShadow = nullptr;
    cocos2d::ui::Layout* _divider = nullptr;
    cocos2d::ui::ListView* _messageListView = nullptr;
    cocos2d::Vector<RecentMessageBar*> _messageBars;
    RoundedRectSprite* _bottomGradient = nullptr;
    
    std::vector<RecentMessage> _messageData;
    
    MessageSelectedCallback _callback = nullptr;
    
    float _messageBarHeight;
    
    void setupMessageBars();
    
public:
    
    static const float kMessageBarHeightPortrait;
    static const float kMessageBarHeightLandscape;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setMessageData(const std::vector<RecentMessage>& messageData);
    void setMessageSelectedCallback(const MessageSelectedCallback& callback);
    
    void toggleBottomGradient(bool enabled);
    
    void setMessageBarHeight(float height);
    
    CREATE_FUNC(RecentMessages);
    
};

NS_AZ_END

#endif /* RecentMessages_h */
