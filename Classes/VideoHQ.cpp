//
//  VideoHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/08/2019.
//

#include "VideoHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VideoHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createFeaturedTiles();
    createRecentlyPlayedTiles();
    createEpisodePlayer();
    
    return true;
}
void VideoHQ::onEnter()
{
    Super::onEnter();
}
void VideoHQ::onExit()
{
    Super::onExit();
}
void VideoHQ::onSizeChanged()
{
    Super::onSizeChanged();
    
    _episodePlayer->stopAllActions();
    _episodePlayer->setTouchEnabled(_isPortrait);
    if(_isPortrait)
    {
        //if(_episodePlayer->getParent() == _staticContentLayout)
        //{
        //    _episodePlayer->retain();
        //    _episodePlayer->removeFromParent();
        //    addChild(_episodePlayer, 1);
        //    _episodePlayer->release();
        //}
        _episodePlayer->setSizePercent(Vec2(1.0,1.0f));
        if(_episodePlayerMoving)
        {
            if(_episodePlayerOpen)
            {
                _episodePlayer->setPosition(Vec2(0, -_episodePlayer->getContentSize().height + 50));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodePlayer->setPosition(Vec2(0,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
        }
        else
        {
            if(_episodePlayerOpen)
            {
                _episodePlayer->setPosition(Vec2(0,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodePlayer->setPosition(Vec2(0, -_episodePlayer->getContentSize().height + 100));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
        }
    }
    else
    {
        //if(_episodePlayer->getParent() == this)
        //{
        //    _episodePlayer->retain();
        //    _episodePlayer->removeFromParent();
        //    _staticContentLayout->addChild(_episodePlayer, 1);
        //    _episodePlayer->release();
        //}
        //_episodePlayer->setPositionType(PositionType::PERCENT);
        _episodePlayer->setPosition(Vec2(0,0));
        _episodePlayer->setSizePercent(Vec2(0.5,1.0f));
        _episodePlayerOpen = false;
        _episodePlayerMoving = false;
    }
    
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getContentSize().width, 400));
    _featuredLayout->setContentSize(Size(_contentListView->getContentSize().width, 1000));
    _contentListView->forceDoLayout();
}

void VideoHQ::createFeaturedTiles()
{
    _featuredLayout = ui::Layout::create();
    _featuredLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _featuredLayout->setBackGroundColor(Color3B::YELLOW);
    _featuredLayout->setSizeType(SizeType::ABSOLUTE);
    _featuredLayout->setPositionType(PositionType::ABSOLUTE);
    _featuredLayout->setContentSize(Size(_contentListView->getContentSize().width, 1000));
    _contentListView->pushBackCustomItem(_featuredLayout);
    
}
void VideoHQ::createRecentlyPlayedTiles()
{
    _recentlyPlayedLayout = ui::Layout::create();
    _recentlyPlayedLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _recentlyPlayedLayout->setBackGroundColor(Color3B::BLUE);
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getContentSize().width, 400));
    _contentListView->pushBackCustomItem(_recentlyPlayedLayout);
}
void VideoHQ::createDropdowns()
{
    
}

void VideoHQ::createEpisodePlayer()
{
    _episodePlayer = ui::Layout::create();
    _episodePlayer->setSizeType(SizeType::PERCENT);
    _episodePlayer->setSizePercent(Vec2(1.0f,1.0f));
    _episodePlayer->setBackGroundColorType(BackGroundColorType::SOLID);
    _episodePlayer->setBackGroundColor(Color3B::GREEN);
    _episodePlayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _episodePlayer->setPosition(Vec2(0,0));
    _episodePlayer->setTouchEnabled(true);
    _episodePlayer->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_episodePlayerOpen)
            {
                if(!_episodePlayerMoving)
                {
                    _episodePlayerMoving = true;
                    _episodePlayer->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0f, Vec2(0,-_episodePlayer->getContentSize().height + 100)), CallFunc::create([this](){
                        _episodePlayerOpen = false;
                        _episodePlayerMoving = false;
                    })));
                }
            }
            else
            {
                if(!_episodePlayerMoving)
                {
                    _episodePlayerMoving = true;
                    _episodePlayer->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0f, Vec2(0,0)), CallFunc::create([this](){
                        _episodePlayerOpen = true;
                        _episodePlayerMoving = false;
                    })));
                }
            }
        }
    });
    addChild(_episodePlayer, 1);
    
}

NS_AZOOMEE_END
