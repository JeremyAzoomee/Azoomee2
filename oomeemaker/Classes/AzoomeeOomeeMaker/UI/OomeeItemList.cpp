//
//  OomeeItemList.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeItemList.h"
#include "OomeeItemButton.h"
#include "../DataObjects/OomeeMakerDataStorage.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/CCSpriteWithHue.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void OomeeItemList::doLayout()
{
    Super::doLayout();
}

void OomeeItemList::interceptTouchEvent(cocos2d::ui::Widget::TouchEventType event, cocos2d::ui::Widget *sender, cocos2d::Touch* touch)
{
    // use this to handle touch on buttons if we have mutiple columns
    //forceDoLayout();
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
                button->setSwallowTouches(false);
                itemRow->setContentSize(Size(this->getContentSize().width, MAX(button->getContentSize().height, itemRow->getContentSize().height)));
                itemRow->addChild(button);
            }
            i++;
        }
        pushBackCustomItem(itemRow);
    }
    
    forceDoLayout();
}

void OomeeItemList::setColourItems()
{
    removeAllItems();
    auto colours = OomeeMakerDataStorage::getInstance()->getColourList();
    int i = 0;
    while(i < colours.size())
    {
        ui::Layout* itemRow = ui::Layout::create();
        itemRow->setLayoutType(ui::Layout::Type::ABSOLUTE);
        itemRow->setContentSize(Size(0, 0));
        
        for(int column = 0; column < _columns; column++)
        {
            if(i < colours.size())
            {
                ui::Button* colourButton = ui::Button::create("res/oomeeMaker/colour.png");
                colourButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                colourButton->ignoreContentAdaptWithSize(false);
                colourButton->setContentSize(Size(this->getContentSize().width * 0.3f, this->getContentSize().width * 0.3f));
                colourButton->setNormalizedPosition(Vec2((column + 0.5) / _columns, 0.5));
                colourButton->setColor(Color3B(colours.at(i)->getColours().at("base")));
                colourButton->addTouchEventListener([this, i, colours](Ref* pSender, ui::Widget::TouchEventType eType){
                    if(eType == ui::Widget::TouchEventType::ENDED)
                    {
                        if(_colourSelectedCallback)
                        {
                            _colourSelectedCallback(colours.at(i));
                        }
                    }
                });
                itemRow->setContentSize(Size(this->getContentSize().width, MAX(colourButton->getContentSize().height, itemRow->getContentSize().height)));
                itemRow->addChild(colourButton);
            }
            i++;
        }
        pushBackCustomItem(itemRow);
    }
}

bool OomeeItemList::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Color3B::WHITE);
    
    setDirection(ui::ScrollView::Direction::VERTICAL);
    setBounceEnabled(true);
    setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    setItemsMargin(100.0f);
    setTopPadding(100.0f);
    setBottomPadding(100.0f);
    setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
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

void OomeeItemList::setColourSelectedCallback(const ColourSelectedCallback &callback)
{
    _colourSelectedCallback = callback;
}

void OomeeItemList::setColumns(int columns)
{
    _columns = columns;
}

NS_AZOOMEE_OM_END
