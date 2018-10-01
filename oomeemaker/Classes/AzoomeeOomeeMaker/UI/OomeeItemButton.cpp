//
//  OomeeItemButton.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeItemButton.h"
#include "DragAndDropController.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void OomeeItemButton::setItemData(const OomeeItemRef& itemdata)
{
    _itemData = itemdata;
    const std::string& iconFilename = OomeeMakerDataHandler::getInstance()->getAssetDir() + _itemData->getIconFilename();
    loadTextureNormal(iconFilename);
    setMainImage(iconFilename);
    setPlaceholderImage("res/chat/ui/avatar/contact_inknown_icon.png");
    setScale(_itemData->getMenuScale());
    
    addTouchEventListener([this](Ref*, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::BEGAN)
        {
            DragAndDropController::getInstance()->setItemData(_itemData);
        }
        else if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_itemSelectedCallback)
            {
                AnalyticsSingleton::getInstance()->accessorySelectedEvent("TAP");
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
