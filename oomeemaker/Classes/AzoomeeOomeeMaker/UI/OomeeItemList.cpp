//
//  OomeeItemList.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeItemList.h"
#include "OomeeItemButton.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void OomeeItemList::doLayout()
{
    Super::doLayout();
}

void OomeeItemList::interceptTouchEvent(cocos2d::ui::Widget::TouchEventType event, cocos2d::ui::Widget *sender, cocos2d::Touch* touch)
{
    // use this to handle touch on buttons if we have mutiple columns
    forceDoLayout();
}

void OomeeItemList::setItems(const std::vector<OomeeItemRef>& itemList)
{
    removeAllItems();
    
    for(const OomeeItemRef& item : itemList)
    {
        OomeeItemButton* button = OomeeItemButton::create();
        button->setItemSelectedCallback([this](const OomeeItemRef& data) {
            if(_itemSelectedCallback)
            {
                _itemSelectedCallback(data);
            }
        });
        button->setItemData(item);
        pushBackCustomItem(button);
    }
    
    forceDoLayout();
}

bool OomeeItemList::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setDirection(ui::ScrollView::Direction::VERTICAL);
    setBounceEnabled(true);
    setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    setItemsMargin(50.0f);
    
    return true;
}

void OomeeItemList::onEnter()
{
    Super::onEnter();
}

void OomeeItemList::onExit()
{
    Super::onExit();
}

void OomeeItemList::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

void OomeeItemList::setItemSelectedCallback(const ItemSelectedCallback &callback)
{
    _itemSelectedCallback = callback;
}

NS_AZOOMEE_OM_END
