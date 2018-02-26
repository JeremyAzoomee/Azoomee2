//
//  ItemCategoryButton.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "ItemCategoryButton.h"
#include "../DataObjects/OomeeMakerDataHandler.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void ItemCategoryButton::setCategoryData(const ItemCategoryRef& categoryData)
{
    _categoryData = categoryData;
    loadTextureNormal(OomeeMakerDataHandler::getInstance()->getAssetDir() + _categoryData->getIconFilename());
    addTouchEventListener([this](Ref*, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_itemSelectedCallback)
            {
                _itemSelectedCallback(_categoryData);
            }
        }
    });
}

ItemCategoryRef ItemCategoryButton::getCategoryData() const
{
    return _categoryData;
}

void ItemCategoryButton::setItemSelectedCallback(const ItemSelectedCallback &callback)
{
    _itemSelectedCallback = callback;
}

NS_AZOOMEE_OM_END
