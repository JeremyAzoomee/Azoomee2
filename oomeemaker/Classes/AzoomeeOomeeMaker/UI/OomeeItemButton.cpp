//
//  OomeeItemButton.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeItemButton.h"
#include "DragAndDropController.h"
#include "../DataObjects/OomeeMakerDataHandler.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void OomeeItemButton::setItemData(const OomeeItemRef& itemdata)
{
    _itemData = itemdata;
    loadTextureNormal(OomeeMakerDataHandler::getInstance()->getAssetDir() + _itemData->getAssetName());
    setMainImage(OomeeMakerDataHandler::getInstance()->getAssetDir() + _itemData->getAssetName());
    setPlaceholderImage("res/chat/ui/avatar/contact_inknown_icon.png");
    setScale(_itemData->getMenuScale());
    
    addTouchEventListener([this](Ref*, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::BEGAN)
        {
            DragAndDropController::getInstance()->setItemData(_itemData);
        }
        
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_itemSelectedCallback)
            {
                _itemSelectedCallback(_itemData);
            }
        }
    });
}

OomeeItemRef OomeeItemButton::getData() const
{
    return _itemData;
}

void OomeeItemButton::setItemSelectedCallback(const ItemSelectedCallback &callback)
{
    _itemSelectedCallback = callback;
}

NS_AZOOMEE_OM_END
