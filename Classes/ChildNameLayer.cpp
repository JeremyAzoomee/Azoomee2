//
//  ChildNameLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildNameLayer.h"
#include <AzoomeeCommon/Strings.h>
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
    
    Label* title = Label::createWithTTF(_("Add another child"), Style::Font::Regular, 150);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setPosition(contentSize.width / 2.0f, contentSize.height * 0.9f);
    title->setColor(Color3B::WHITE);
    this->addChild(title);
    
    _textInput = TextInputLayer::createWithSize(Size(contentSize.width * 0.75f, 160), INPUT_IS_CHILD_NAME);
    _textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * 0.6f));
    _textInput->setDelegate(this);
    if(_childCreator && _childCreator->getName() != "")
    {
        _textInput->setText(_childCreator->getName());
    }
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF(_("What’s your child’s name?"), Style::Font::Regular, 100);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    textInputTitle->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2, _textInput->getContentSize().height * 1.25f));
    textInputTitle->setColor(Color3B::WHITE);
    this->addChild(textInputTitle);
    
    _continueButton = ui::Button::create("res/login/next_btnGreen.png");
    _continueButton->setAnchorPoint(Vec2(1.25f,1.25f));
    _continueButton->setTouchEnabled(_textInput->inputIsValid());
    _continueButton->setOpacity(_textInput->inputIsValid() ? 255 : 125);
    _continueButton->setPosition(contentSize);
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
    
    ui::Button* backButton = ui::Button::create("res/login/back_btnGreen.png");
    backButton->setAnchorPoint(Vec2(-0.25f,1.25f));
    backButton->setPosition(Vec2(0, contentSize.height));
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
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress1.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}

void ChildNameLayer::onExit()
{
    if(_textInput)
    {
        _textInput->setDelegate(nullptr);
    }
    Super::onExit();
}

//delegate functions

void ChildNameLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _continueButton->setTouchEnabled(isValid);
    _continueButton->setOpacity(isValid ? 255 : 125);
}
void ChildNameLayer::textInputReturnPressed(TextInputLayer* inputLayer)
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
void ChildNameLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}
void ChildNameLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

NS_AZOOMEE_END

