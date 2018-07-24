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
    
    bool isPortrait = contentSize.width < contentSize.height;
    
    Label* title = Label::createWithTTF(StringUtils::format("Personalise their%sexperience",isPortrait ? "\n" : " "), Style::Font::Regular, 150);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setHorizontalAlignment(TextHAlignment::CENTER);
    title->setPosition(contentSize.width / 2.0f, contentSize.height * 0.9f);
    title->setColor(Color3B::WHITE);
    this->addChild(title);
    
    _textInput = TextInputLayer::createWithSize(Size(400, 160), INPUT_IS_AGE);
    _textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * 0.6f));
    _textInput->setDelegate(this);
    if(_childCreator && _childCreator->getAge() > 0)
    {
        _textInput->setText(StringUtils::format("%d",_childCreator->getAge()));
    }
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF("How old are they?", Style::Font::Regular, 120);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    textInputTitle->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2, _textInput->getContentSize().height * 1.25f));
    textInputTitle->setColor(Color3B::WHITE);
    this->addChild(textInputTitle);
    
    Label* textInputSubTitle = Label::createWithTTF(StringUtils::format("We need this to give them%sage-appropriate content.",isPortrait ? "\n" : " "), Style::Font::Regular, 100);
    textInputSubTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textInputSubTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    textInputSubTitle->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2, -_textInput->getContentSize().height / 2));
    textInputSubTitle->setColor(Style::Color::telish);
    this->addChild(textInputSubTitle);
    
    _continueButton = ui::Button::create("res/buttons/MainButton.png");
    _continueButton->setColor(Style::Color::telish);
    _continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _continueButton->setPosition(Vec2(contentSize.width * (isPortrait ? 0.5 : 0.66), contentSize.height * 0.3));
    _continueButton->setEnabled(_textInput->inputIsValid());
    _continueButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
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
    this->addChild(_continueButton);
    
    Label* buttonText = Label::createWithTTF("Continue", Style::Font::Regular, _continueButton->getContentSize().height * 0.4f);
    buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    buttonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    buttonText->setTextColor(Color4B::BLACK);
    _continueButton->addChild(buttonText);
    
    ui::Button* backButton = ui::Button::create("res/buttons/MainButton.png");
    backButton->setColor(Style::Color::telish);
    backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    backButton->setPosition(Vec2(contentSize.width * (isPortrait ? 0.5 : 0.33), contentSize.height * (isPortrait ? 0.2 : 0.3)));
    backButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_delegate)
            {
                _delegate->prevLayer();
            }
        }
    });
    this->addChild(backButton);
    
    Label* backButtonText = Label::createWithTTF("Cancel", Style::Font::Regular, _continueButton->getContentSize().height * 0.4f);
    backButtonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    backButtonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    backButtonText->setTextColor(Color4B::BLACK);
    backButton->addChild(backButtonText);
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress2.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}

//delegate functions

void ChildAgeLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _continueButton->setEnabled(isValid);
}
void ChildAgeLayer::textInputReturnPressed(TextInputLayer* inputLayer)
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
void ChildAgeLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}
void ChildAgeLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

NS_AZOOMEE_END


