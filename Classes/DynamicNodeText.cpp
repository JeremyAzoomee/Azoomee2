//
//  DynamicNodeText.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/12/2017.
//

#include "DynamicNodeText.h"
#include "DynamicNodeCreator.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const int DynamicNodeText::kDefaultTitleFontSize = 94;
const int DynamicNodeText::kDefaultBodyFontSize = 72;
const int DynamicNodeText::kDefaultFooterFontSize = 48;

bool DynamicNodeText::init()
{
    return true;
}

bool DynamicNodeText::initWithParams(int fontSize, Color4B fontColour, const rapidjson::Value &params, const cocos2d::Size &dynamicNodeSize, bool usingExternParams)
{
    this->setContentSize(dynamicNodeSize);
    
    const Vec2& pos = getVec2FromJson("position", params)/100.0f;
    std::string text = getStringFromJson("text", params);
    
    if(usingExternParams)
    {
        text = DynamicNodeCreator::addExternalParamsToString(text);
    }
    
    const std::string& alignment = getStringFromJson("alignment", params);
    int newFontSize = getIntFromJson("fontSize", params);
    if(newFontSize != INT_MAX)
    {
        fontSize = newFontSize;
    }
    
    fontSize *= DynamicNodeCreator::getInstance()->_sizeMod;
    
    int lineSpacing = getIntFromJson("lineSpacing", params);
    if(lineSpacing == INT_MAX)
    {
        lineSpacing = 20;
    }
    
    fontColour = getColor4BFromJson("colour", params);
    
    const std::string& fontType = getBoolFromJson("bold", params) ?  Style::Font::Bold : Style::Font::Regular;
    
    _text = Label::createWithTTF(text, fontType, fontSize);
    _text->setNormalizedPosition(pos);
    _text->setTextColor(fontColour);
    _text->setLineSpacing(lineSpacing);
    
    if(alignment == "left")
    {
        _text->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _text->setHorizontalAlignment(TextHAlignment::LEFT);
    }
    else if(alignment == "right")
    {
        _text->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _text->setHorizontalAlignment(TextHAlignment::RIGHT);
    }
    else
    {
        _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _text->setHorizontalAlignment(TextHAlignment::CENTER);
    }
    
    if(_text->getContentSize().width > dynamicNodeSize.width - kBorderSafeZone)
    {
        _text->setScale((dynamicNodeSize.width - kBorderSafeZone) / _text->getContentSize().width);
    }
    
    this->addChild(_text);
    
    return true;
}

NS_AZOOMEE_END
