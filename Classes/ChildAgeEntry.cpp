//
//  ChildAgeEntry.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 27/11/2019.
//

#include "ChildAgeEntry.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildAgeEntry::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/onboarding/rounded_rect_45px.png");
    setBackGroundImageScale9Enabled(true);
    
    createContentLayout();
    
    return true;
}
void ChildAgeEntry::onEnter()
{
    setContinueButtonEnabled(TextInputChecker::isValidAge(StringUtils::format("%d", _selectedAge)));
    Super::onEnter();
}
void ChildAgeEntry::onExit()
{
    Super::onExit();
}
void ChildAgeEntry::onSizeChanged()
{
    Super::onSizeChanged();
    _inputHolder->setPosition((this->getContentSize() / 2.0f));
}

void ChildAgeEntry::createContentLayout()
{
    const float elementSpacing = 50.0f;
    
    _inputHolder = ui::Layout::create();
    _inputHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _inputHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _inputHolder->setLayoutType(Type::VERTICAL);
    addChild(_inputHolder);
    
    _inputTitle = DynamicText::create(_(""), Style::Font::PoppinsBold(), 55);
    _inputTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _inputTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _inputTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _inputTitle->setTextColor(Color4B(Style::Color::black));
    _inputTitle->setTextAreaSize(Size(500,180));
    _inputTitle->setOverflow(Label::Overflow::SHRINK);
    _inputHolder->addChild(_inputTitle);
    
    _ageNumLayout = ui::Layout::create();
    _ageNumLayout->setLayoutType(Type::VERTICAL);
    _ageNumLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0, elementSpacing, 0, elementSpacing)));
    _inputHolder->addChild(_ageNumLayout);
    createAgeButtons();
    
    _continueButton = CTAButton::create("res/onboarding/rounded_button.png");
    _continueButton->ignoreContentAdaptWithSize(false);
    _continueButton->setContentSize(Size(700,140));
    _continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _continueButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
    _continueButton->setColor(Style::Color::darkIndigo);
    _continueButton->setTextAreaSizePercent(Vec2(0.9f,0.8f));
    _continueButton->setText(_("Continue"));
    _continueButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
    _continueButton->setTextColour(Color4B::WHITE);
    _continueButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_continueCallback)
            {
                _continueCallback(_selectedAge);
            }
        }
    });
    _inputHolder->addChild(_continueButton);
    
    _backButton = DynamicText::create(_("Back"), Style::Font::Bold(), 63);
    _backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _backButton->setContentSize(Size(700,140));
    _backButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
    _backButton->setTextColor(Color4B(Style::Color::brownGrey));
    _backButton->setTouchEnabled(true);
    _backButton->setTouchScaleChangeEnabled(true);
    _backButton->ignoreContentAdaptWithSize(false);
    _backButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _backButton->setTextVerticalAlignment(TextVAlignment::TOP);
    _backButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        switch(eType)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
            {
                _backButton->setScale(1.05f);
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::MOVED:
            {
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::ENDED:
            {
                _backButton->setScale(1.0f);
                if(_backCallback)
                {
                    _backCallback(_selectedAge);
                }
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
            {
                _backButton->setScale(1.0f);
                break;
            }
        }
    });
    _inputHolder->addChild(_backButton);
    
    _inputHolder->setContentSize(Size(0, _inputTitle->getContentSize().height + _ageNumLayout->getContentSize().height + _continueButton->getContentSize().height + _backButton->getContentSize().height + (5 * elementSpacing)));
    
}

void ChildAgeEntry::createAgeButtons()
{
    const Size& buttonSize = Size(210,210);
    const float padding = 50.0f;
    const Size& paddedSize = Size(buttonSize.width + padding, buttonSize.height + padding);
    
    const std::vector<std::pair<std::string, int>> numberStrings = {{"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}, {"10+", 10}};
    const Vec2& gridSize = Vec2(3,3);
    
    const float rowWidth = paddedSize.width * gridSize.x;
    const Size& totalSize = Size(rowWidth, paddedSize.height * gridSize.y);
    
    _ageNumLayout->setContentSize(totalSize);
    
    int i = 0;
    for(int row = 0; row < gridSize.y; row++)
    {
        ui::Layout* rowContainer = ui::Layout::create();
        rowContainer->setContentSize(Size(rowWidth, paddedSize.height));
        rowContainer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        _ageNumLayout->addChild(rowContainer);
        for(int col = 0; col < gridSize.x; col++)
        {
            if(i < numberStrings.size())
            {
                int numVal = numberStrings.at(i).second;
                ui::Button* button = ui::Button::create("res/hqscene/circle.png");
                button->ignoreContentAdaptWithSize(false);
                button->setCascadeColorEnabled(false);
                button->setTag(numVal);
                button->setContentSize(buttonSize);
                button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                button->setNormalizedPosition(Vec2((col + 0.5f) / gridSize.x, 0.5f));
                button->setColor(Style::Color::darkIndigo);
                button->addTouchEventListener([this, numVal](Ref* pSender, ui::Widget::TouchEventType eType){
                    if(eType == TouchEventType::ENDED)
                    {
                        setSelectedAge(numVal);
                    }
                });
                rowContainer->addChild(button);
                _buttons.pushBack(button);
                
                Label* number = Label::createWithTTF(numberStrings.at(i).first, Style::Font::PoppinsBold(), 80);
                number->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                number->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
                number->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                number->setTextColor(Color4B::WHITE);
                button->addChild(number);
            }
            i++;
        }
    }
    
}

void ChildAgeEntry::setContinueCallback(const ButtonCallback& callback)
{
    _continueCallback = callback;
}
void ChildAgeEntry::setBackCallback(const ButtonCallback& callback)
{
    _backCallback = callback;
}

void ChildAgeEntry::setContinueButtonEnabled(bool enabled)
{
    _continueButton->setTouchEnabled(enabled);
    _continueButton->setColor(enabled ? Style::Color::darkIndigo : Style::Color::greyBlue2);
}

void ChildAgeEntry::setChildName(const std::string &name)
{
    _inputTitle->setString(StringUtils::format(_("Hi %s! How old are you?").c_str(), name.c_str()));
    highlightMessageString(name + "!", Color3B(255, 128, 13), Color3B::BLACK);
}

void ChildAgeEntry::setSelectedAge(int age)
{
    ui::Button* targetButton = nullptr;
    for(auto btn : _buttons)
    {
        btn->setColor(Style::Color::darkIndigo);
        if(btn->getTag() == age)
        {
            targetButton = btn;
        }
    }
    if(targetButton)
    {
        targetButton->setColor(Style::Color::strongPink);
    }
    _selectedAge = age;
    setContinueButtonEnabled(TextInputChecker::isValidAge(StringUtils::format("%d", _selectedAge)));
}

void ChildAgeEntry::highlightMessageString(const std::string& targetStr, const cocos2d::Color3B& highlightColour, const cocos2d::Color3B& baseColour)
{
    const std::string& messageString = _inputTitle->getString();
    auto pos = messageString.find(targetStr);
    if( pos != std::string::npos )
    {
        _inputTitle->setTextColor(Color4B::WHITE);
        for(int i = 0; i < messageString.size(); i++)
        {
            auto letter = _inputTitle->getLetter(i);
            if(letter)
            {
                if(i >= pos && i < (pos + targetStr.size()))
                {
                    letter->setColor(highlightColour);
                }
                else
                {
                    letter->setColor(Color3B(baseColour));
                }
            }
        }
    }
}

NS_AZOOMEE_END
