//
//  ConfirmCancelMessageBox.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 20/09/2018.
//

#include "ConfirmCancelMessageBox.h"
#include "Style.h"
#include "../Audio/AudioMixer.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

ConfirmCancelMessageBox* ConfirmCancelMessageBox::createWithParams(const std::string &title, const std::string &confirmButtonFilename, const std::string &cancelButtonFilename, const cocos2d::Color3B &backgroundColour, const cocos2d::Color4B &textColour)
{
    ConfirmCancelMessageBox* messageBox = ConfirmCancelMessageBox::create();
    messageBox->setParams(title, confirmButtonFilename, cancelButtonFilename, backgroundColour, textColour);
    return messageBox;
}

bool ConfirmCancelMessageBox::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Director::getInstance()->getVisibleSize());
	
	Layer* touchLayer = Layer::create();
	touchLayer->setContentSize(this->getContentSize() * 2);
	touchLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	touchLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	this->addChild(touchLayer);
    
    EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), touchLayer);
    
    _messageBox = ui::Layout::create();
    _messageBox->setBackGroundImage("res/modal/confirm_cancel_popup.png");
    _messageBox->setBackGroundImageScale9Enabled(true);
    _messageBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBox->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _messageBox->setContentSize(Size(890,570));
    this->addChild(_messageBox);
    
    _titleText = Label::createWithTTF("", Style::Font::Regular(), 109);
    _titleText->setNormalizedPosition(Vec2(0.5,0.9));
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleText->setTextColor(Color4B::BLACK);
    _messageBox->addChild(_titleText);
    
    _confirmButton = ui::Button::create();
    _confirmButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _confirmButton->setNormalizedPosition(Vec2(0.33,0.4));
    _confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			AudioMixer::getInstance()->playEffect(NEXT_BUTTON_AUDIO_EFFECT);
            if(_delegate)
            {
                _delegate->onConfirmPressed(this);
            }
        }
    });
    _messageBox->addChild(_confirmButton);
    
    _cancelButton = ui::Button::create();
    _cancelButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _cancelButton->setNormalizedPosition(Vec2(0.67,0.4));
    _cancelButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
            if(_delegate)
            {
                _delegate->onCancelPressed(this);
            }
        }
    });
    _messageBox->addChild(_cancelButton);
    
    return true;
}
void ConfirmCancelMessageBox::onEnter()
{
    Super::onEnter();
}

void ConfirmCancelMessageBox::setParams(const std::string& title, const std::string& confirmButtonFilename, const std::string& cancelButtonFilename, const cocos2d::Color3B& backgroundColour, const cocos2d::Color4B& textColour)
{
    _confirmButton->loadTextureNormal(confirmButtonFilename);
    _cancelButton->loadTextureNormal(cancelButtonFilename);
    _messageBox->setBackGroundImageColor(backgroundColour);
    _titleText->setString(title);
    _titleText->setTextColor(textColour);
}

void ConfirmCancelMessageBox::setDelegate(Azoomee::ConfirmCancelMessageBoxDelegate *delegate)
{
    _delegate = delegate;
}


NS_AZOOMEE_END
