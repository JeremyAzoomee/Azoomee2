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
    
    bool isPortrait = contentSize.width < contentSize.height;
    
    Label* mainTitle = Label::createWithTTF(StringUtils::format("Welcome to%sAzoomee", isPortrait ? "\n" : " "), Style::Font::Regular, 200);
    mainTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    mainTitle->setPosition(Vec2(contentSize.width / 2, contentSize.height - 100));
    mainTitle->setColor(Style::Color::telish);
    mainTitle->enableGlow(Color4B(Style::Color::telish));
    mainTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(mainTitle);
    
    Label* subTitle = Label::createWithTTF(StringUtils::format("Let's complete%syour setup", isPortrait ? "\n" : " "), Style::Font::Regular, 150);
    subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    subTitle->setPosition(mainTitle->getPosition() - Vec2(0, mainTitle->getContentSize().height + 50));
    subTitle->setColor(Color3B::WHITE);
    subTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(subTitle);
    
    _textInput = TextInputLayer::createWithSize(Size(contentSize.width * (isPortrait ? 0.75 : 0.5), 160), INPUT_IS_CHILD_NAME);
    _textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * (isPortrait ? 0.55f : 0.5f)));
    _textInput->setDelegate(this);
    if(_childCreator && _childCreator->getName() != "")
    {
        _textInput->setText(_childCreator->getName());
    }
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF("What’s your child’s name?", Style::Font::Regular, 100);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    textInputTitle->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2, _textInput->getContentSize().height * 1.25));
    textInputTitle->setColor(Color3B::WHITE);
    this->addChild(textInputTitle);
    
    _continueButton = ui::Button::create("res/buttons/MainButton.png");
    _continueButton->setColor(Style::Color::telish);
    _continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _continueButton->setEnabled(_textInput->inputIsValid());
    _continueButton->setPosition(Vec2(contentSize.width / 2, contentSize.height * 0.3));
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
    
    Label* buttonText = Label::createWithTTF("Continue", Style::Font::Regular, _continueButton->getContentSize().height * 0.4f);
    buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    buttonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    buttonText->setTextColor(Color4B::BLACK);
    _continueButton->addChild(buttonText);
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress1.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}

//delegate functions

void ChildNameLayerFirstTime::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _continueButton->setEnabled(isValid);
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
            _delegate->nextLayer();
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
