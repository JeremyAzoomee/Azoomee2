//
//  ChildNameLayerFirstTime.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildNameLayerFirstTime.h"
#include <AzoomeeCommon/UI/Style.h>

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
    
    Label* mainTitle = Label::createWithTTF("Welcome to Azoomee", Style::Font::Bold, 120);
    mainTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    mainTitle->setPosition(Vec2(contentSize.width / 2, contentSize.height - mainTitle->getContentSize().height));
    mainTitle->setColor(Style::Color::telish);
    mainTitle->enableGlow(Color4B(Style::Color::telish));
    this->addChild(mainTitle);
    
    Label* subTitle = Label::createWithTTF("Let's complete your setup", Style::Font::Bold, 90);
    subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    subTitle->setPosition(mainTitle->getPosition() - Vec2(0, mainTitle->getContentSize().height * 1.5f));
    subTitle->setColor(Color3B::WHITE);
    this->addChild(subTitle);
    
    _textInput = TextInputLayer::createWithSize(Size(contentSize.width / 2, 200), INPUT_IS_CHILD_NAME);
    _textInput->setCenterPosition(contentSize / 2.0f);
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF("What’s your child’s name?", Style::Font::Regular, 60);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    textInputTitle->setPosition(_textInput->getPosition() + Vec2(0, _textInput->getContentSize().height));
    textInputTitle->setColor(Color3B::WHITE);
    this->addChild(textInputTitle);
    
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
                    _childCreator->setChildName(_textInput->getText());
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
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress1.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}


NS_AZOOMEE_END
