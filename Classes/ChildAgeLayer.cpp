//
//  ChildAgeLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildAgeLayer.h"

#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildAgeLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void ChildAgeLayer::onEnter()
{
    const Size& contentSize = this->getContentSize();
    
    Label* title = Label::createWithTTF("Personalise their experience", Style::Font::Bold, 90);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setPosition(contentSize.width / 2.0f, contentSize.height * 0.9f);
    title->setColor(Color3B::WHITE);
    this->addChild(title);
    
    _textInput = TextInputLayer::createWithSize(Size(contentSize.width / 4, 200), INPUT_IS_AGE);
    _textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * 0.6f));
    if(_childCreator && _childCreator->getAge() > 0)
    {
        _textInput->setText(StringUtils::format("%d",_childCreator->getAge()));
    }
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF("How old are they?", Style::Font::Regular, 60);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    textInputTitle->setPosition(_textInput->getPosition() + Vec2(0, _textInput->getContentSize().height));
    textInputTitle->setColor(Color3B::WHITE);
    this->addChild(textInputTitle);
    
    Label* textInputSubTitle = Label::createWithTTF("We need this to give them age-appropriate content.", Style::Font::Regular, 50);
    textInputSubTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    textInputSubTitle->setPosition(_textInput->getPosition() + Vec2(0, -_textInput->getContentSize().height));
    textInputSubTitle->setColor(Style::Color::telish);
    this->addChild(textInputSubTitle);
    
    ui::Button* continueButton = ui::Button::create("res/buttons/MainButton.png");
    continueButton->setColor(Style::Color::greenishTeal);
    continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    continueButton->setPosition(Vec2(contentSize.width / 2, contentSize.height * 0.3));
    continueButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_textInput->inputIsValid())
            {
                if(_childCreator)
                {
                    _childCreator->setAge(std::atoi(_textInput->getText().c_str()));
                }
                if(_delegate)
                {
                    _delegate->nextLayer();
                }
            }
        }
    });
    this->addChild(continueButton);
    
    Label* buttonText = Label::createWithTTF("Continue", Style::Font::Regular, continueButton->getContentSize().height * 0.4f);
    buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    buttonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    buttonText->setTextColor(Color4B::BLACK);
    continueButton->addChild(buttonText);
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress2.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}


NS_AZOOMEE_END


