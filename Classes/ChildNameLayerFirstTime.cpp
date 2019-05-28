//
//  ChildNameLayerFirstTime.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildNameLayerFirstTime.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildNameLayerFirstTime::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void ChildNameLayerFirstTime::onEnter()
{
    const Size& contentSize = this->getContentSize();
    
    bool isPortrait = contentSize.width < contentSize.height;
	
	_continueButton = ui::Button::create("res/buttons/blue_arrow_button.png");
	
    _textInput = TextInputLayer::createWithSize(Size(contentSize.width * (isPortrait ? 0.65f : 0.5f), 160), INPUT_IS_CHILD_NAME);
	if(isPortrait)
	{
    	_textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * 0.65f));
	}
	else
	{
		_textInput->setCenterPosition(Vec2(contentSize.width / 2.0f - _continueButton->getContentSize().width * 0.625f, contentSize.height * 0.6f));
	}
    _textInput->setDelegate(this);
    if(_childCreator && _childCreator->getName() != "")
    {
        _textInput->setText(_childCreator->getName());
    }
	_textInput->setKeyboardAlwaysShown(true);
    this->addChild(_textInput);
    
	
    _continueButton->setTouchEnabled(_textInput->inputIsValid());
    _continueButton->setOpacity(_textInput->inputIsValid() ? 255 : 125);
	if(isPortrait)
	{
		_continueButton->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2.0f, 0));
		_continueButton->setAnchorPoint(Vec2(0.5f,1.5f));
	}
	else
	{
		_continueButton->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width, _textInput->getContentSize().height / 2));
		_continueButton->setAnchorPoint(Vec2(-0.25,0.5));
	}
	
    _continueButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_textInput->inputIsValid())
            {
                if(_childCreator)
                {
                    _childCreator->setChildName(_textInput->getText());
                }
                if(_delegate)
                {
                    _delegate->nextLayer();
                }
            }
        }
    });
    this->addChild(_continueButton);
	
    Super::onEnter();
}

void ChildNameLayerFirstTime::onExit()
{
    if(_textInput)
    {
        _textInput->setDelegate(nullptr);
    }
    Super::onExit();
}


//delegate functions

void ChildNameLayerFirstTime::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _continueButton->setTouchEnabled(isValid);
    _continueButton->setOpacity(isValid ? 255 : 125);
}
void ChildNameLayerFirstTime::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(_textInput->inputIsValid())
    {
        if(_childCreator)
        {
            _childCreator->setChildName(_textInput->getText());
        }
        if(_delegate)
        {
            this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([&](){_delegate->nextLayer();}), NULL));
        }
    }
}
void ChildNameLayerFirstTime::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}
void ChildNameLayerFirstTime::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
	
}


NS_AZOOMEE_END
