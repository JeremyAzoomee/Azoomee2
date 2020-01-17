//
//  HQPage.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#include "HQPage.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const cocos2d::Size HQPage::kCircleTileSizeLandscape = Size(256,256);
const cocos2d::Size HQPage::kCircleTileSizePortrait = Size(300,300);
const float HQPage::kFeaturedContentHeightLandscape = 640.0f;
const float HQPage::kFeaturedContentHeightPortrait = 1110.0f;
const float HQPage::kListViewSidePadding = 64.0f;
const cocos2d::Vec2 HQPage::kFocusDropDownAnchor = Vec2(0.0f, 1.0f); // top left

bool HQPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f,1.0f));
    setClippingEnabled(true);
    
    _structureUIHolder = ui::Layout::create();
    _structureUIHolder->setSizeType(SizeType::PERCENT);
    _structureUIHolder->setSizePercent(Vec2(1.0f,1.0f));
    _structureUIHolder->setLayoutType(Layout::Type::HORIZONTAL);
    addChild(_structureUIHolder);
    
    _staticContentLayout = ui::Layout::create();
    _staticContentLayout->setSizeType(SizeType::PERCENT);
    _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
    _staticContentLayout->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    _structureUIHolder->addChild(_staticContentLayout);
    
    _contentListView = ui::ListView::create();
    _contentListView->setSizeType(SizeType::PERCENT);
    _contentListView->setSizePercent(Vec2(1.0f,1.0f));
    _contentListView->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    _contentListView->setItemsMargin(32);
    _contentListView->setBounceEnabled(true);
    _contentListView->setBottomPadding(160);
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
    _contentSelectedCallback = callback;
}

void HQPage::listviewDropdownResizeCallback()
{
    if(_focusedDropdown == nullptr)
    {
        return;
    }

    // Force layout to happen immediately
    _contentListView->forceDoLayout();
    
    // Calculate new scroll position to match _focusedDropdown's _resizingPositionInView
    const Size& itemSize = _focusedDropdown->getContentSize();
    const Vec2 itemOrigin(_focusedDropdown->getLeftBoundary(), _focusedDropdown->getBottomBoundary());
    const Vec2 itemPosition = itemOrigin + Vec2(itemSize.width * kFocusDropDownAnchor.x, itemSize.height * kFocusDropDownAnchor.y);
    const Size& scrollContentSize = _contentListView->getInnerContainerSize();
    const Size& viewportContentSize = _contentListView->getContentSize();
    
    Vec2 newScrollPosition = -(itemPosition - _resizingPositionInView);
    
    // Check bounds of the scroll position
    newScrollPosition.y = MIN(newScrollPosition.y, 0);
    newScrollPosition.y = MAX(newScrollPosition.y, -(scrollContentSize.height - viewportContentSize.height));
    
    _contentListView->setInnerContainerPosition(newScrollPosition);
}
void HQPage::dropdownAutoOpenCloseLogic(DropdownContentHolder* pressedDropdown, const cocos2d::Vector<DropdownContentHolder*>& dropdownHoldersInListview)
{
    _focusedDropdown = pressedDropdown;
    
    // Stop the scrollview
    _contentListView->stopOverallScroll();
    
    // Get the position of the focused item in viewport
    const Vec2& scrollPosition = _contentListView->getInnerContainerPosition();
    const Size& itemSize = _focusedDropdown->getContentSize();
    const Vec2 itemOrigin(_focusedDropdown->getLeftBoundary(), _focusedDropdown->getBottomBoundary());
    _resizingPositionInView = itemOrigin + scrollPosition + Vec2(itemSize.width * kFocusDropDownAnchor.x, itemSize.height * kFocusDropDownAnchor.y);
    
    // Now toggle the opening/closing
    pressedDropdown->toggleOpened(!pressedDropdown->isOpen());
    AnalyticsSingleton::getInstance()->categoryOpenEvent(pressedDropdown->isOpen(), pressedDropdown->getContentItemData()->getTitle());
    
    for(auto dd : dropdownHoldersInListview)
    {
        if(dd != _focusedDropdown && dd->isOpen())
        {
            dd->toggleOpened(false);
        }
    }
}

float HQPage::getScrolledPos() const
{
    return _contentListView->getInnerContainerPosition().y;
}

void HQPage::setScrolledPos(float pos)
{
    runAction(Sequence::createWithTwoActions(DelayTime::create(0.1), CallFunc::create([this, pos](){
        _contentListView->setInnerContainerPosition(Vec2(_contentListView->getInnerContainerPosition().x, pos));
    })));
}

NS_AZOOMEE_END
