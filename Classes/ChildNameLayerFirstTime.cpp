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
    bool is18x9 = ConfigStorage::getInstance()->isDevice18x9();
    
    Label* mainTitle = Label::createWithTTF(_("Welcome to Azoomee"), Style::Font::Regular(), 110);
    mainTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    mainTitle->setPosition(Vec2(contentSize.width / 2, contentSize.height - (is18x9 ? (isPortrait ? 225 : 50) : 100)));
    mainTitle->setColor(Style::Color::telish);
    mainTitle->enableGlow(Color4B(Style::Color::telish));
    mainTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	mainTitle->setWidth(contentSize.width * 0.75f);
    this->addChild(mainTitle);
    
    Label* subTitle = Label::createWithTTF(_("Let's complete your setup"), Style::Font::Regular(), 96);
    subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    subTitle->setPosition(mainTitle->getPosition() - Vec2(0, mainTitle->getContentSize().height + ((is18x9 && !isPortrait) ? 25 : 50)));
    subTitle->setColor(Color3B::WHITE);
    subTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(subTitle);
    
    _textInput = TextInputLayer::createWithSize(Size(contentSize.width * (isPortrait ? 0.75f : 0.5f), 160), INPUT_IS_CHILD_NAME);
    _textInput->setCenterPosition(Vec2(contentSize.width / 2.0f, contentSize.height * (isPortrait ? 0.55f : 0.5f)));
    _textInput->setDelegate(this);
    if(_childCreator && _childCreator->getName() != "")
    {
        _textInput->setText(_childCreator->getName());
    }
    this->addChild(_textInput);
    
    Label* textInputTitle = Label::createWithTTF(_("What’s your child’s name?"), Style::Font::Regular(), 64);
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
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress1.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
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
