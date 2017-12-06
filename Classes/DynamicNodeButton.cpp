//
//  DynamicNodeButton.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/12/2017.
//

#include "DynamicNodeButton.h"
#include "DynamicNodeCreator.h"
#include "DynamicNodeButtonListener.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool DynamicNodeButton::init()
{
    return true;
}

bool DynamicNodeButton::initWithParams(const rapidjson::Value &params, const cocos2d::Size &dynamicNodeSize)
{
    this->setContentSize(dynamicNodeSize);
    
    Vec2 pos;
    Vec2 size;
    ButtonActionDataRef actionData;
    
    pos = getVec2FromJson("position",params);
    
    if(pos.x != 0 && pos.y != 0)
    {
        pos = pos/100.0f;
    }
    else
    {
        return false;
    }
    
    size = getVec2FromJson("size",params);
    
    if(size.x != 0 && size.y != 0)
    {
        size = size/100.0f;
    }
    else
    {
        return false;
    }
    
    const std::string& btnSprite = getStringFromJson("sprite", params);
    
    const std::string& btnString = getStringFromJson("text", params);
    
    if(params.HasMember("action"))
    {
        const rapidjson::Value& actionParams = params["action"];
        actionData = ButtonActionData::createWithJson(actionParams);
    }
    
    bool underlined = getBoolFromJson("underlined", params);
    
    addButtonWithParams(Size(size.x * dynamicNodeSize.width, size.y * dynamicNodeSize.height), pos, btnString, actionData, btnSprite, underlined);
    
    return true;
}

bool DynamicNodeButton::initWithParams(const rapidjson::Value &params, const cocos2d::Size &dynamicNodeSize, const rapidjson::Document& externParams)
{
    this->setContentSize(dynamicNodeSize);
    
    Vec2 pos;
    Vec2 size;
    ButtonActionDataRef actionData;
    
    pos = getVec2FromJson("position",params);
    
    if(pos.x != 0 && pos.y != 0)
    {
        pos = pos/100.0f;
    }
    else
    {
        return false;
    }
    
    size = getVec2FromJson("size",params);
    
    if(size.x != 0 && size.y != 0)
    {
        size = size/100.0f;
    }
    else
    {
        return false;
    }
    
    const std::string& btnSprite = getStringFromJson("sprite", params);
    
    const std::string& btnString = getStringFromJson("text", params);
    
    if(params.HasMember("action"))
    {
        const rapidjson::Value& actionParams = params["action"];
        actionData = ButtonActionData::createWithJson(actionParams);

        auto actionParamsMap = actionData->getParams();
        for(auto& param : actionParamsMap)
        {
            param.second = DynamicNodeCreator::addExternalParamsToString(param.second);
        }
        actionData->setParams(actionParamsMap);
        
    }
    
    bool underlined = getBoolFromJson("underlined", params);
    
    addButtonWithParams(Size(size.x * dynamicNodeSize.width, size.y * dynamicNodeSize.height), pos, btnString, actionData, btnSprite, underlined);
    
    return true;
}


void DynamicNodeButton::addButtonWithParams(const cocos2d::Size& size, const cocos2d::Vec2& pos, const std::string& buttonText, ButtonActionDataRef buttonActionData, const std::string& btnSpriteStr, bool underlined)
{
    ui::Button* button = ui::Button::create();
    const std::string& btnSpriteFile = FileUtils::getInstance()->getWritablePath() + DynamicNodeCreator::kCTADeviceImageCacheLoc + btnSpriteStr;
    if(btnSpriteStr == "" || !FileUtils::getInstance()->isFileExist(btnSpriteFile))
    {
        button->loadTextures(DynamicNodeCreator::kCTAAssetLoc + "rectangle_copy_3.png", DynamicNodeCreator::kCTAAssetLoc + "rectangle_copy_3.png");
    }
    else
    {
        button->loadTextures(btnSpriteFile,btnSpriteFile);
    }
    
    button->setContentSize(size);
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setNormalizedPosition(pos);
    button->setScale9Enabled(true);
    button->setSwallowTouches(true);
    button->addTouchEventListener(CC_CALLBACK_2(DynamicNodeButtonListener::onButtonPressedCallFunc, DynamicNodeButtonListener::getInstance(),buttonActionData));
    
    Label* label = Label::createWithTTF(buttonText, Style::Font::Regular, button->getContentSize().height*0.4);
    label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setColor(Color3B::BLACK);
    if(underlined)
    {
        DrawNode* newDrawNode = DrawNode::create();
        newDrawNode->drawRect(Vec2(0, -7), Vec2(label->getContentSize().width, -6), Color4F::BLACK);
        label->addChild(newDrawNode);
    }
    button->addChild(label);
    this->addChild(button);
}

NS_AZOOMEE_END
