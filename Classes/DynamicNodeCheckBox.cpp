//
//  DynamicNodeCheckBox.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 19/04/2018.
//

#include "DynamicNodeCheckBox.h"
#include "DynamicNodeDataInputStorage.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool DynamicNodeCheckBox::init()
{
    return true;
}

bool DynamicNodeCheckBox::initWithParams(const rapidjson::Value &params, const cocos2d::Size &dynamicNodeSize, bool usingExtrnParams)
{
    this->setContentSize(dynamicNodeSize);
    
    Vec2 pos = getVec2FromJson("position",params);
    
    // make sure we have a position, normalise if we do, return if not
    if(pos.x != 0 && pos.y != 0)
    {
        pos = pos/100.0f;
    }
    else
    {
        return false;
    }
    
    float size = getDoubleFromJson("size", params, 0.0) / 100.0f;
    
    _valueKey = getStringFromJson("valueKey", params);
    
    bool isSelected = getBoolFromJson("selected", params);
    
    addCheckboxWithParams(pos, size, isSelected);
    
    return true;
}


void DynamicNodeCheckBox::addCheckboxWithParams(const cocos2d::Vec2& pos, float size, bool selected)
{
    _checkbox = ui::CheckBox::create("res/cta_assets/check-box-empty.png", "res/cta_assets/correct-symbol.png");
    _checkbox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _checkbox->setNormalizedPosition(pos);
    _checkbox->ignoreContentAdaptWithSize(false);
    _checkbox->setContentSize(Size(this->getContentSize().width, this->getContentSize().width) * size);
    _checkbox->addEventListener([&](Ref* pSender, ui::CheckBox::EventType eType){
        switch (eType) {
            case ui::CheckBox::EventType::SELECTED:
                DynamicNodeDataInputStorage::getInstance()->addElementToStorage(_valueKey, "true");
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                DynamicNodeDataInputStorage::getInstance()->addElementToStorage(_valueKey, "false");
                break;
        }
    });
    _checkbox->setSelected(selected);
    this->addChild(_checkbox);
}

NS_AZOOMEE_END
