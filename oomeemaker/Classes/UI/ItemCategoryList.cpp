//
//  ItemCategoryList.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "ItemCategoryList.h"
#include "ItemCategoryButton.h"

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
        button->setItemSelectedCallback([this](const ItemCategoryRef& data) {
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
    
    return true;
}

void ItemCategoryList::onEnter()
{
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

NS_AZOOMEE_OM_END
