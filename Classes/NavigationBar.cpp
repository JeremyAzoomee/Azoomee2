//
//  NavigationBar.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/08/2019.
//

#include "NavigationBar.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQConstants.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::map<std::string, HQType> NavigationBar::kHQNameToTypeConv = {
    {HQConsts::kGameHQName,HQType::GAME},
    {HQConsts::kVideoHQName, HQType::VIDEO},
    {HQConsts::kChatHQName, HQType::CHAT},
    {HQConsts::kOomeeHQName, HQType::OOMEE}
};

const std::map<HQType, std::string> NavigationBar::kHQTypeToNameConv = {
    {HQType::GAME, HQConsts::kGameHQName},
    {HQType::VIDEO, HQConsts::kVideoHQName},
    {HQType::CHAT, HQConsts::kChatHQName},
    {HQType::OOMEE, HQConsts::kOomeeHQName}
};

const std::map<std::string, cocos2d::Color3B> NavigationBar::kHQColourMap = {
    {HQConsts::kGameHQName, Color3B(84, 174, 255)},
    {HQConsts::kVideoHQName, Color3B(249, 74, 91)},
    {HQConsts::kChatHQName, Color3B(218, 18, 255)},
    {HQConsts::kOomeeHQName, Color3B(55, 188, 152)}
};

bool NavigationBar::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/navigation/navigation_bg.png");
    setBackGroundImageScale9Enabled(true);
    setContentSize(Size(1240,284));
    
    const int numButtons = 4;
    const float buttonWidth = 144.0f;
    const float buttonSpacing = 60.0f;
    
    ui::Layout* buttonHolder = ui::Layout::create();
    buttonHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    buttonHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    buttonHolder->setContentSize(Size((numButtons * buttonWidth) + ((numButtons - 1) * buttonSpacing),0));
    addChild(buttonHolder);
    
    float pos = buttonWidth / 2.0f;
    
    _gameHQButton = NavigationButton::create();
    _gameHQButton->setPosition(Vec2(pos,0));
    _gameHQButton->setCircleColour(kHQColourMap.at(HQConsts::kGameHQName));
    _gameHQButton->setButtonIcons("res/navigation/menu_game_hq_on.png", "res/navigation/menu_game_hq.png");
    _gameHQButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            bool success = true;
            if(_callback)
            {
                success = _callback(HQType::GAME);
            }
            if(success)
            {
                this->toggleHQSelected(HQType::GAME);
            }
        }
    });
    buttonHolder->addChild(_gameHQButton);
    
    pos += (buttonWidth + buttonSpacing);
    
    _videoHQButton = NavigationButton::create();
    _videoHQButton->setPosition(Vec2(pos,0));
    _videoHQButton->setCircleColour(kHQColourMap.at(HQConsts::kVideoHQName));
    _videoHQButton->setButtonIcons("res/navigation/menu_video_hq_on.png", "res/navigation/menu_video_hq.png");
    _videoHQButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            bool success = true;
            if(_callback)
            {
                success = _callback(HQType::VIDEO);
            }
            if(success)
            {
                this->toggleHQSelected(HQType::VIDEO);
            }
        }
    });
    buttonHolder->addChild(_videoHQButton);
    
    pos += (buttonWidth + buttonSpacing);
    
    _chatHQButton = NavigationButton::create();
    _chatHQButton->setPosition(Vec2(pos,0));
    _chatHQButton->setCircleColour(kHQColourMap.at(HQConsts::kChatHQName));
    _chatHQButton->setButtonIcons("res/navigation/menu_chat_on.png", "res/navigation/menu_chat.png");
    _chatHQButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            bool success = true;
            if(_callback)
            {
                success = _callback(HQType::CHAT);
            }
            if(success)
            {
                this->toggleHQSelected(HQType::CHAT);
            }
        }
    });
    buttonHolder->addChild(_chatHQButton);
    
    pos += (buttonWidth + buttonSpacing);
    
    _oomeeHQButton = NavigationButton::create();
    _oomeeHQButton->setPosition(Vec2(pos,0));
    _oomeeHQButton->setCircleColour(kHQColourMap.at(HQConsts::kOomeeHQName));
    _oomeeHQButton->setButtonIcons("res/navigation/menu_me_hq_on.png", "res/navigation/menu_me_hq.png");
    _oomeeHQButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            bool success = true;
            if(_callback)
            {
                success = _callback(HQType::OOMEE);
            }
            if(success)
            {
                this->toggleHQSelected(HQType::OOMEE);
            }
        }
    });
    buttonHolder->addChild(_oomeeHQButton);
    
    _chatNotificationIcon = Sprite::create("res/navigation/chatAlert.png");
    _chatNotificationIcon->setName("notification");
    _chatNotificationIcon->setNormalizedPosition(Vec2(0.85f, 0.85f));
    _chatNotificationIcon->setVisible(false);
    _chatHQButton->addChild(_chatNotificationIcon);
    
    return true;
}
void NavigationBar::onEnter()
{
    Super::onEnter();
}
void NavigationBar::onExit()
{
    Super::onExit();
}
void NavigationBar::onSizeChanged()
{
    Super::onSizeChanged();
}

void NavigationBar::setHQSelectedCallback(const HQSelectedCallback& callback)
{
    _callback = callback;
}

void NavigationBar::setChatNotificationEnabled(bool enabled)
{
    _chatNotificationIcon->setVisible(enabled);
}

void NavigationBar::toggleHQSelected(const HQType& hq)
{
    _gameHQButton->setActive(hq == HQType::GAME);
    _videoHQButton->setActive(hq == HQType::VIDEO);
    _chatHQButton->setActive(hq == HQType::CHAT);
    _oomeeHQButton->setActive(hq == HQType::OOMEE);
}

void NavigationBar::toggleHQSelected(const std::string &hqName)
{
    const HQType type = (kHQNameToTypeConv.find(hqName) != kHQNameToTypeConv.end()) ? kHQNameToTypeConv.at(hqName) : HQType::GAME;
    toggleHQSelected(type);
}

NS_AZOOMEE_END
