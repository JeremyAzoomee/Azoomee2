//
//  HQPage.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#include "HQPage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool HQPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f,1.0f));
    
    _contentListView = ui::ListView::create();
    _contentListView->setSizeType(SizeType::PERCENT);
    _contentListView->setSizePercent(Vec2(1.0f,1.0f));
    _contentListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentListView->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_contentListView);
    
    _staticContentLayout = ui::Layout::create();
    _staticContentLayout->setSizeType(SizeType::PERCENT);
    _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
    _staticContentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _staticContentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    addChild(_staticContentLayout);
    
    return true;
}
void HQPage::onEnter()
{
    Super::onEnter();
}
void HQPage::onExit()
{
    Super::onExit();
}
void HQPage::onSizeChanged()
{
    Super::onSizeChanged();
    const Size& contentSize = getContentSize();
    _isPortrait = contentSize.width < contentSize.height;
    if(_isPortrait)
    {
        _contentListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _contentListView->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        
        _staticContentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _staticContentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
    }
    else
    {
        _contentListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _contentListView->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        
        _staticContentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _staticContentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
    }
}

void HQPage::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _contentSceletedCallback = callback;
}

NS_AZOOMEE_END
