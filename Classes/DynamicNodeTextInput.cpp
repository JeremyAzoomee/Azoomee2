//
//  DynamicNodeTextInput.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/11/2017.
//

#include "DynamicNodeTextInput.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool DynamicNodeTextInput::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void DynamicNodeTextInput::initWithParams(const rapidjson::Value &params, const Size& dynamicNodeSize)
{
    Vec2 pos;
    Vec2 size;
    
    pos = getVec2FromJson("position",params);
    
    if(pos.x != 0 && pos.y != 0)
    {
        pos = pos/100.0f;
    }
    else
    {
        return;
    }
    
    size = getVec2FromJson("size",params);
    
    if(size.x != 0 && size.y != 0)
    {
        size = size/100.0f;
    }
    else
    {
        return;
    }
    
    _valueKey = getStringFromJson("valueKey", params);
    
    int inputType = getIntFromJson("inputType", params);
    
    _textInput = TextInputLayer::createWithSize(Size(size.x * dynamicNodeSize.width, size.y * dynamicNodeSize.height), inputType);
    _textInput->setDelegate(this);
    _textInput->setCenterPosition(Vec2(pos.x * dynamicNodeSize.width, pos.y * dynamicNodeSize.height));
    _textInput->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_textInput);
    
    _errorText = createLabelBodyCentred(getStringFromJson("errorText", params), Style::Color::watermelon);
    _errorText->setLineSpacing(20);
    _errorText->setPosition(Vec2(pos.x * dynamicNodeSize.width, pos.y * dynamicNodeSize.height - _textInput->getContentSize().height/2 - _errorText->getContentSize().height/2));
    _errorText->setVisible(false);
    
    this->addChild(_errorText);
    
}

bool DynamicNodeTextInput::isInputValid()
{
    return _inputIsValid;
}

void DynamicNodeTextInput::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _inputIsValid = isValid;
    _errorText->setVisible(!isValid);
    if(_inputIsValid)
    {
        DynamicNodeDataInputStorage::getInstance()->addElementToStorage(_valueKey, inputLayer->getText());
    }
}

void DynamicNodeTextInput::textInputReturnPressed(TextInputLayer* inputLayer)
{
    _inputIsValid = inputLayer->inputIsValid();
    _errorText->setVisible(!_inputIsValid);
    if(_inputIsValid)
    {
        DynamicNodeDataInputStorage::getInstance()->addElementToStorage(_valueKey, inputLayer->getText());
    }
    
}

void DynamicNodeTextInput::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    _inputIsValid = inputLayer->inputIsValid();
    _errorText->setVisible(!_inputIsValid);
}

void DynamicNodeTextInput::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    _inputIsValid = inputLayer->inputIsValid();
    _errorText->setVisible(!_inputIsValid);
    if(!_inputIsValid)
    {
        DynamicNodeDataInputStorage::getInstance()->addElementToStorage(_valueKey, inputLayer->getText());
    }
}

NS_AZOOMEE_END
