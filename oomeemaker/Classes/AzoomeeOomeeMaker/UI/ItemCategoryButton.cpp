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
    setScale(_categoryData->getScaleUnselected());
    loadTextureNormal(OomeeMakerDataHandler::getInstance()->getAssetDir() + _categoryData->getIconFilenameUnselected());
    setMainImage(OomeeMakerDataHandler::getInstance()->getAssetDir() + _categoryData->getIconFilenameUnselected());
    setPlaceholderImage("res/chat/ui/avatar/avatar_background.png");
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

void ItemCategoryButton::setSelected(bool selected)
{
    if(selected)
    {
        loadTextureNormal(OomeeMakerDataHandler::getInstance()->getAssetDir() + _categoryData->getIconFilenameSelected());
        setMainImage(OomeeMakerDataHandler::getInstance()->getAssetDir() + _categoryData->getIconFilenameSelected());
        setScale(_categoryData->getScaleSelected());
    }
    else
    {
        loadTextureNormal(OomeeMakerDataHandler::getInstance()->getAssetDir() + _categoryData->getIconFilenameUnselected());
        setMainImage(OomeeMakerDataHandler::getInstance()->getAssetDir() + _categoryData->getIconFilenameUnselected());
        setScale(_categoryData->getScaleUnselected());
    }
}

NS_AZOOMEE_OM_END
