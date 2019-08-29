//
//  OomeeHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 29/08/2019.
//

#include "OomeeHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool OomeeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createOomeeLayout();
    createScrollViewContent();
    
    return true;
}
void OomeeHQ::onEnter()
{
    scheduleUpdate();
    Super::onEnter();
}
void OomeeHQ::onExit()
{
    unscheduleUpdate();
    Super::onExit();
}
void OomeeHQ::onSizeChanged()
{
    Super::onSizeChanged();
    
    if(_isPortrait)
    {
        _structureUIHolder->setLayoutType(Type::VERTICAL);
        _staticContentLayout->setSizePercent(Vec2(1.0f, 0.5f));
        _contentListView->setSizePercent(Vec2(1.0f, 0.5f));
    }
    else
    {
        _structureUIHolder->setLayoutType(Type::HORIZONTAL);
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
    }
    
    _artStudioLayout->setContentSize(Size(_contentListView->getContentSize().width, 1412));
    _shopButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _oomeeMakerButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->forceDoLayout();
}

void OomeeHQ::update(float deltaT)
{
    Super::update(deltaT);
    
    
}

void OomeeHQ::createOomeeLayout()
{
    _oomeeLayout = ui::Layout::create();
    _oomeeLayout->setSizeType(SizeType::PERCENT);
    _oomeeLayout->setSizePercent(Vec2(1.0f, 1.0f));
    _oomeeLayout->setPositionType(PositionType::PERCENT);
    _oomeeLayout->setPositionPercent(Vec2(0.5f, 0.5f));
    _oomeeLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _oomeeLayout->setBackGroundColor(Color3B::GRAY);
    _oomeeLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _staticContentLayout->addChild(_oomeeLayout, 1);
}
void OomeeHQ::createScrollViewContent()
{
    
    _oomeeMakerButton = ui::Layout::create();
    _oomeeMakerButton->setBackGroundColorType(BackGroundColorType::SOLID);
    _oomeeMakerButton->setBackGroundColor(Color3B::BLUE);
    _oomeeMakerButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->pushBackCustomItem(_oomeeMakerButton);
    
    _shopButton = ui::Layout::create();
    _shopButton->setBackGroundColorType(BackGroundColorType::SOLID);
    _shopButton->setBackGroundColor(Color3B::YELLOW);
    _shopButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->pushBackCustomItem(_shopButton);
    
    _artStudioLayout = ui::Layout::create();
    _artStudioLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _artStudioLayout->setBackGroundColor(Color3B::GREEN);
    _artStudioLayout->setContentSize(Size(_contentListView->getContentSize().width, 1412));
    _contentListView->pushBackCustomItem(_artStudioLayout);
    
}

NS_AZOOMEE_END
