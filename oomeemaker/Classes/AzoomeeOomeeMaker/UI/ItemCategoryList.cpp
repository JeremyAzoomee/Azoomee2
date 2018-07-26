//
//  ItemCategoryList.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "ItemCategoryList.h"
#include "ItemCategoryButton.h"
#include <AzoomeeCommon/Utils/ActionBuilder.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void ItemCategoryList::doLayout()
{
    Super::doLayout();
}

void ItemCategoryList::setCategories(const std::vector<ItemCategoryRef>& categoryList)
{
    removeAllItems();
    
    for(const ItemCategoryRef& category : categoryList)
    {
        ItemCategoryButton* button = ItemCategoryButton::create();
        button->setCategoryData(category);
        button->setItemSelectedCallback([this, button](const ItemCategoryRef& data) {
            if(_itemSelectedCallback)
            {
                _itemSelectedCallback(data);
            }
        });
        pushBackCustomItem(button);
    }
    
    forceDoLayout();
}

bool ItemCategoryList::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setDirection(ui::ScrollView::Direction::VERTICAL);
    setBounceEnabled(true);
    setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    setItemsMargin(50.0f);
    setTopPadding(50.0f);
    setBottomPadding(50.0f);
    
    return true;
}

void ItemCategoryList::onEnter()
{
    auto items = getItems();
    for(auto item : items)
    {
        item->setOpacity(0);
        float randomDelay = RandomHelper::random_real(0.2, 0.7) + 0.5;
        item->runAction(createBlinkEffect(randomDelay, 0.1));
    }
    
    Super::onEnter();
}

void ItemCategoryList::onExit()
{
    Super::onExit();
}

void ItemCategoryList::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

void ItemCategoryList::setItemSelectedCallback(const ItemSelectedCallback& callback)
{
    _itemSelectedCallback = callback;
}

void ItemCategoryList::setSelectedButton(const ItemCategoryRef& data)
{
    auto itemList = getItems();
    for(const auto& item : itemList)
    {
        ItemCategoryButton* button = dynamic_cast<ItemCategoryButton*>(item);
        if(button)
        {
            if(button->getCategoryData() == data)
            {
                button->setSelected(true);
            }
            else
            {
                button->setSelected(false);
            }
        }
    }
}

NS_AZOOMEE_OM_END
