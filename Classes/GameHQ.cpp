//
//  GameHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#include "GameHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool GameHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createFeaturedTiles();
    createRecentlyPlayedTiles();
    
    return true;
}
void GameHQ::onEnter()
{
    Super::onEnter();
}
void GameHQ::onExit()
{
    Super::onExit();
}
void GameHQ::onSizeChanged()
{
    Super::onSizeChanged();

    if(_isPortrait)
    {
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _featuredLayout->setSizeType(SizeType::ABSOLUTE);
        _featuredLayout->setPositionType(PositionType::ABSOLUTE);
        _featuredLayout->setContentSize(Size(_contentListView->getContentSize().width, 1000));
        if(_featuredLayout->getParent() == _staticContentLayout)
        {
            _featuredLayout->retain();
            _featuredLayout->removeFromParent();
            _contentListView->insertCustomItem(_featuredLayout, 0);
            _featuredLayout->release();
        }
    }
    else
    {
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        
        _featuredLayout->setSizeType(SizeType::PERCENT);
        _featuredLayout->setSizePercent(Vec2(1.0f,1.0f));
        _featuredLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _featuredLayout->setPositionType(PositionType::PERCENT);
        _featuredLayout->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        if(_contentListView->getItem(0) == _featuredLayout)
        {
            _featuredLayout->retain();
            _contentListView->removeItem(0);
            _staticContentLayout->addChild(_featuredLayout);
            _featuredLayout->release();
        }
    }
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getContentSize().width, 400));
    _contentListView->forceDoLayout();
}

void GameHQ::createFeaturedTiles()
{
    _featuredLayout = ui::Layout::create();
    _featuredLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _featuredLayout->setBackGroundColor(Color3B::YELLOW);
    _featuredLayout->setSizeType(SizeType::PERCENT);
    _featuredLayout->setSizePercent(Vec2(1.0f,1.0f));
    _staticContentLayout->addChild(_featuredLayout);
    
}
void GameHQ::createRecentlyPlayedTiles()
{
    _recentlyPlayedLayout = ui::Layout::create();
    _recentlyPlayedLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _recentlyPlayedLayout->setBackGroundColor(Color3B::BLUE);
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getContentSize().width, 400));
    _contentListView->pushBackCustomItem(_recentlyPlayedLayout);
}
void GameHQ::createDropdowns()
{
    
}

NS_AZOOMEE_END
