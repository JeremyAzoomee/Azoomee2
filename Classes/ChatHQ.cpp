//
//  ChatHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/10/2019.
//

#include "ChatHQ.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChatHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createRecentMessages();
    createFriendsList();
    
    const Color3B& gradColour = Style::Color::darkIndigo;
    _topScrollGradient = LayerGradient::create(Color4B(gradColour), Color4B(gradColour.r, gradColour.g, gradColour.b, 0));
    _topScrollGradient->setIgnoreAnchorPointForPosition(false);
    _topScrollGradient->setContentSize(Size(_contentListView->getContentSize().width, 0));
    _topScrollGradient->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _topScrollGradient->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    addChild(_topScrollGradient, 1);
    
    _contentListView->addEventListener([this](Ref* pSender, ui::ScrollView::EventType eType){
        if(eType == ui::ScrollView::EventType::CONTAINER_MOVED)
        {
            const float minY = _contentListView->getContentSize().height - _contentListView->getInnerContainerSize().height;
            float scrollDist = MAX(_contentListView->getInnerContainerPosition().y - minY, 0);
            
            _topScrollGradient->setContentSize(Size(_contentListView->getContentSize().width, MIN(scrollDist,160)));
            
        }
    });
    
    return true;
}

void ChatHQ::onEnter()
{
    Super::onEnter();
}

void ChatHQ::onExit()
{
    Super::onExit();
}

void ChatHQ::onSizeChanged()
{
    Super::onSizeChanged();
    
    if(_isPortrait)
    {
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _recentMessagesLayout->setSizeType(SizeType::ABSOLUTE);
        _recentMessagesLayout->setPositionType(PositionType::ABSOLUTE);
        _recentMessagesLayout->setContentSize(Size(_contentListView->getContentSize().width - (kListViewSidePadding / 4.0f), 1000));
        if(_recentMessagesLayout->getParent() == _staticContentLayout)
        {
            _recentMessagesLayout->retain();
            _recentMessagesLayout->removeFromParent();
            _contentListView->insertCustomItem(_recentMessagesLayout, 0);
            _recentMessagesLayout->release();
        }
    }
    else
    {
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        
        _recentMessagesLayout->setSizeType(SizeType::PERCENT);
        _recentMessagesLayout->setSizePercent(Vec2(0.975f,0.95f));
        _recentMessagesLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _recentMessagesLayout->setPositionType(PositionType::PERCENT);
        _recentMessagesLayout->setPositionPercent(Vec2::ANCHOR_TOP_LEFT);
        if(_contentListView->getItem(0) == _recentMessagesLayout)
        {
            _recentMessagesLayout->retain();
            _contentListView->removeItem(0);
            _staticContentLayout->addChild(_recentMessagesLayout);
            _recentMessagesLayout->release();
        }
    }
    
    const float contentListViewWidth = _contentListView->getSizePercent().x * getContentSize().width;
    
    _friendsListLayout->setContentSize(Size(contentListViewWidth, 1000));
    
    _topScrollGradient->setContentSize(Size(contentListViewWidth, _topScrollGradient->getContentSize().height));
    
    _contentListView->forceDoLayout();
    
}

void ChatHQ::createRecentMessages()
{
    _recentMessagesLayout = ui::Layout::create();
    _recentMessagesLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _recentMessagesLayout->setBackGroundColor(Color3B::RED);
    _recentMessagesLayout->setSizeType(SizeType::PERCENT);
    _recentMessagesLayout->setSizePercent(Vec2(0.975f,0.95f));
    _recentMessagesLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _recentMessagesLayout->setPositionType(PositionType::PERCENT);
    _recentMessagesLayout->setPositionPercent(Vec2::ANCHOR_TOP_LEFT);
    _staticContentLayout->addChild(_recentMessagesLayout);
}

void ChatHQ::createFriendsList()
{
    _friendsListLayout = ui::Layout::create();
    _friendsListLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _friendsListLayout->setBackGroundColor(Color3B::GREEN);
    _friendsListLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentListView->addChild(_friendsListLayout);
}

NS_AZOOMEE_END
