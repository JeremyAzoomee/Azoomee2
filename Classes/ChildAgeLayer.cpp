//
//  ChildAgeLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildAgeLayer.h"
#include <AzoomeeCommon/Strings.h>
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
    
    Label* title = Label::createWithTTF(_("Personalise their experience"), Style::Font::Regular(), 150);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setHorizontalAlignment(TextHAlignment::CENTER);
    title->setPosition(contentSize.width / 2.0f, contentSize.height * 0.9f);
    title->setColor(Color3B::WHITE);
	title->setOverflow(Label::Overflow::SHRINK);
	title->setDimensions(contentSize.width * 0.75f, contentSize.height * 0.15f);
    this->addChild(title);
    
    _textInput = TextInputLayer::createWithSize(Size(400, 160), INPUT_IS_AGE);
    _textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * 0.6f));
    _textInput->setDelegate(this);
    if(_childCreator && _childCreator->getAge() > 0)
    {
        _textInput->setText(StringUtils::format("%d",_childCreator->getAge()));
    }
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF(_("How old are they?"), Style::Font::Regular(), 120);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    textInputTitle->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2, _textInput->getContentSize().height * 1.25f));
    textInputTitle->setColor(Color3B::WHITE);
	textInputTitle->setOverflow(Label::Overflow::SHRINK);
	textInputTitle->setDimensions(contentSize.width * 0.8f, contentSize.height * 0.1f);
    this->addChild(textInputTitle);
    
    Label* textInputSubTitle = Label::createWithTTF(_("We need this to give them age-appropriate content."), Style::Font::Regular(), 100);
    textInputSubTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textInputSubTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    textInputSubTitle->setPosition(_textInput->getPosition() + Vec2(_textInput->getContentSize().width / 2, -_textInput->getContentSize().height / 2));
    textInputSubTitle->setColor(Style::Color::telish);
	textInputSubTitle->setWidth(contentSize.width * 0.8f);
    this->addChild(textInputSubTitle);
    
    _continueButton = ui::Button::create("res/login/next_btnGreen.png");
    _continueButton->setAnchorPoint(Vec2(1.25f,1.25f));
    _continueButton->setPosition(contentSize);
    _continueButton->setTouchEnabled(_textInput->inputIsValid());
    _continueButton->setOpacity(_textInput->inputIsValid() ? 255 : 125);
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
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress2.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}

void ChildAgeLayer::onExit()
{
    if(_textInput)
    {
        _textInput->setDelegate(nullptr);
    }
    Super::onExit();
}

//delegate functions

void ChildAgeLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _continueButton->setTouchEnabled(isValid);
    _continueButton->setOpacity(isValid ? 255 : 125);
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
            this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([&](){_delegate->nextLayer();}), NULL));
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


