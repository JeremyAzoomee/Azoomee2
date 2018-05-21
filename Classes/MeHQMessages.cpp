//
//  MeHQMessages.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 21/05/2018.
//

#include "MeHQMessages.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQMessages::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 0));
    setBackGroundColor(Color3B::GREEN);
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    
    return true;
}

void MeHQMessages::onEnter()
{
    Super::onEnter();
    Chat::ChatAPI::getInstance()->registerObserver(this);
}

void MeHQMessages::onExit()
{
    Super::onExit();
    Chat::ChatAPI::getInstance()->removeObserver(this);
}

void MeHQMessages::onSizeChanged()
{
    
}

/// Friend List success response
void onChatAPIGetFriendList(const Chat::FriendList& friendList, int amountOfNewMessages)
{
    
}
/// Get message list success response
void onChatAPIGetChatMessages(const Chat::MessageList& messageList)
{
    
}
/// API error from Chat request
void onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    
}

NS_AZOOMEE_END
