//
//  OomeeItemList.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeItemList.h"
#include "OomeeItemButton.h"
#include <AzoomeeCommon/UI/LayoutParams.h>

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
    _itemList = itemList;
    removeAllItems();
    int i = 0;
    while(i < itemList.size())
    {
        ui::Layout* itemRow = ui::Layout::create();
        itemRow->setLayoutType(ui::Layout::Type::ABSOLUTE);
        itemRow->setContentSize(Size(0, 0));
        
        for(int column = 0; column < _columns; column++)
        {
            if(i < itemList.size())
            {
                OomeeItemRef item = itemList.at(i);
                OomeeItemButton* button = OomeeItemButton::create();
                button->setItemSelectedCallback([this](const OomeeItemRef& data) {
                    if(_itemSelectedCallback)
                    {
                        _itemSelectedCallback(data);
                    }
                });
                button->setItemData(item);
                button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                button->setNormalizedPosition(Vec2((column + 0.5) / _columns, 0.5));
                itemRow->setContentSize(Size(this->getContentSize().width * ((column + 1)/_columns), MAX(button->getContentSize().height, itemRow->getContentSize().height)));
                itemRow->addChild(button);
            }
            i++;
        }
        pushBackCustomItem(itemRow);
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
    setTopPadding(100.0f);
    setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    return true;
}

void OomeeItemList::onEnter()
{
    setBackGroundImage("res/oomeeMaker/OomeeMaker_Drawer_Gradient.png");
    setBackGroundImageScale9Enabled(true);
    
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

void OomeeItemList::setColumns(int columns)
{
    _columns = columns;
}

NS_AZOOMEE_OM_END
