//
//  ChildNameLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildNameLayer.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildNameLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void ChildNameLayer::onEnter()
{
    const Size& contentSize = this->getContentSize();
    
    Label* title = Label::createWithTTF("Add another child", Style::Font::Regular, 150);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setPosition(contentSize.width / 2.0f, contentSize.height * 0.9f);
    title->setColor(Color3B::WHITE);
    this->addChild(title);
    
    _textInput = TextInputLayer::createWithSize(Size(contentSize.width * ((contentSize.width > contentSize.height) ? 0.5 : 0.75), 160), INPUT_IS_CHILD_NAME);
    _textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * 0.6f));
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF("What’s your child’s name?", Style::Font::Regular, 100);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    textInputTitle->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2, _textInput->getContentSize().height * 1.25));
    textInputTitle->setColor(Color3B::WHITE);
    this->addChild(textInputTitle);
    
    ui::Button* continueButton = ui::Button::create("res/buttons/MainButton.png");
    continueButton->setColor(Style::Color::telish);
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

