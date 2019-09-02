//
//  HQPage.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#include "HQPage.h"
#include <AzoomeeCommon/UI/LayoutParams.h>

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
    setClippingEnabled(true);
    setBackGroundColor(Color3B::GRAY);
    setBackGroundColorType(BackGroundColorType::SOLID);
    
    _structureUIHolder = ui::Layout::create();
    _structureUIHolder->setSizeType(SizeType::PERCENT);
    _structureUIHolder->setSizePercent(Vec2(1.0f,1.0f));
    //_structureUIHolder->setClippingEnabled(true);
    _structureUIHolder->setLayoutType(Layout::Type::HORIZONTAL);
    addChild(_structureUIHolder);
    
    _staticContentLayout = ui::Layout::create();
    _staticContentLayout->setSizeType(SizeType::PERCENT);
    _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
    _staticContentLayout->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    _staticContentLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _staticContentLayout->setBackGroundColor(Color3B::ORANGE);
    _structureUIHolder->addChild(_staticContentLayout);
    
    _contentListView = ui::ListView::create();
    _contentListView->setSizeType(SizeType::PERCENT);
    _contentListView->setSizePercent(Vec2(1.0f,1.0f));
    _contentListView->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    _contentListView->setBackGroundColorType(BackGroundColorType::SOLID);
    _contentListView->setBackGroundColor(Color3B::RED);
    _contentListView->setItemsMargin(70);
    _contentListView->setBounceEnabled(true);
    _contentListView->setTopPadding(70);
    _contentListView->setBottomPadding(70);
    _structureUIHolder->addChild(_contentListView);
    
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
}

void HQPage::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _contentSceletedCallback = callback;
}

NS_AZOOMEE_END
