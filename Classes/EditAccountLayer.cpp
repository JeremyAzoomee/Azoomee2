//
//  EditAccountLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/08/2018.
//

#include "EditAccountLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool EditAccountLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/settings/rounded_rect.png");
    setBackGroundImageScale9Enabled(true);
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void EditAccountLayer::onEnter()
{
    int lowestY = this->getContentSize().height;
    
    _nameLayout = ui::Layout::create();
    _nameLayout->setContentSize(Size(this->getContentSize().width * 0.6f, 157));
    _nameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,80,0,0)));
    
    this->addChild(_nameLayout);
    
    _editNameLayout = ui::Layout::create();
    _editNameLayout->setContentSize(_nameLayout->getContentSize());
    _editNameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _editNameLayout->setVisible(false);
    _nameLayout->addChild(_editNameLayout);
    
    _editNameInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_CHILD_NAME);
    _editNameInput->setCenterPosition(_editNameLayout->getContentSize() / 2);
    _editNameInput->setText(ParentDataProvider::getInstance()->getParentDisplayName());
    _editNameLayout->addChild(_editNameInput);
    
    ui::Button* confirmNameEditButton = ui::Button::create("res/settings/tick_button.png");
    confirmNameEditButton->setAnchorPoint(Vec2(1.14,0.5));
    confirmNameEditButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    confirmNameEditButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_editNameInput->getText() != _nameText->getString())
            {
                if(_editNameInput->inputIsValid())
                {
                    ModalMessages::getInstance()->startLoading();
                    HttpRequestCreator* request = API::UpdateParentDetailsRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), _editNameInput->getText(), ParentDataProvider::getInstance()->getParentPin(), this);
                    request->execute();
                    _displayNameLayout->setVisible(true);
                    _editNameLayout->setVisible(false);
                }
            }
            else
            {
                _displayNameLayout->setVisible(true);
                _editNameLayout->setVisible(false);
            }
        }
    });
    _editNameInput->addChild(confirmNameEditButton);
    
    _displayNameLayout = ui::Layout::create();
    _displayNameLayout->setContentSize(_nameLayout->getContentSize());
    _displayNameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _nameLayout->addChild(_displayNameLayout);
    
    _nameText = ui::Text::create(ParentDataProvider::getInstance()->getParentDisplayName(), Style::Font::Medium, 107);
    _nameText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _nameText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _nameText->setTextColor(Color4B::BLACK);
    _displayNameLayout->addChild(_nameText);
    
    _editNameButton = ui::Button::create("res/settings/edit_button_circle.png");
    _editNameButton->setAnchorPoint(Vec2(-0.25,0.5));
    _editNameButton->setPosition((_displayNameLayout->getContentSize() * 0.5) + Size(_nameText->getContentSize().width * 0.5f,0));
    _editNameButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            _displayNameLayout->setVisible(false);
            _editNameLayout->setVisible(true);
        }
    });
    _displayNameLayout->addChild(_editNameButton);
    
    lowestY -= (_nameLayout->getContentSize().height + 80);
    
    const std::string& username = ParentDataProvider::getInstance()->getParentEmail();
    _emailText = ui::Text::create(username, Style::Font::Medium, 59);
    _emailText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,40,0,0)));
    _emailText->setTextColor(Color4B::BLACK);
    this->addChild(_emailText);
    
    lowestY -= (_emailText->getContentSize().height + 40);
    
    ui::Layout* pinEditboxLayout = ui::Layout::create();
    pinEditboxLayout->setContentSize(Size(this->getContentSize().width, 190));
    pinEditboxLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,35,0,0)));
    this->addChild(pinEditboxLayout);
    
    lowestY -= (pinEditboxLayout->getContentSize().height + 35);
    
    Label* pinText = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(PIN_LABEL), Style::Font::Medium, 59);
    pinText->setTextColor(Color4B(Style::Color::battleshipGrey));
    pinText->setHorizontalAlignment(TextHAlignment::RIGHT);
    pinText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    pinText->setPosition(Vec2(pinEditboxLayout->getContentSize().width * 0.225f, pinEditboxLayout->getContentSize().height / 2));
    pinEditboxLayout->addChild(pinText);
    
    _pinEditbox = TextInputLayer::createSettingsBoxTextInput(this->getContentSize().width * 0.6f, INPUT_IS_PIN);
    _pinEditbox->setCenterPosition(Vec2(pinEditboxLayout->getContentSize().width * 0.55f, pinEditboxLayout->getContentSize().height / 2));
    _pinEditbox->setText(ParentDataProvider::getInstance()->getParentPin());
    _pinEditbox->setEnabled(false);
    pinEditboxLayout->addChild(_pinEditbox);
    
    _editPinButton = ui::Button::create("res/settings/edit_text_input.png");
    _editPinButton->setPosition(Vec2(pinEditboxLayout->getContentSize().width * 0.85f - 10, pinEditboxLayout->getContentSize().height / 2));
    _editPinButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _editPinButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_editingPin)
            {
                if(_pinEditbox->inputIsValid())
                {
                    ModalMessages::getInstance()->startLoading();
                    HttpRequestCreator* request = API::UpdateParentDetailsRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), ParentDataProvider::getInstance()->getParentDisplayName(), _pinEditbox->getText(), this);
                    request->execute();
                    _editingPin = false;
                    _pinEditbox->setEnabled(false);
                    _editPinButton->loadTextureNormal("res/settings/edit_text_input.png");
                }
            }
            else
            {
                _editingPin = true;
                _pinEditbox->setEnabled(true);
                _editPinButton->loadTextureNormal("res/settings/confirm_edit_button.png");
            }
        }
    });
    pinEditboxLayout->addChild(_editPinButton);
    
    ui::Layout* passwordEditboxLayout = ui::Layout::create();
    passwordEditboxLayout->setContentSize(Size(this->getContentSize().width, 190));
    passwordEditboxLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    this->addChild(passwordEditboxLayout);
    
    lowestY -= passwordEditboxLayout->getContentSize().height;
    
    Label* passwordText = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(PASSWORD_LABEL), Style::Font::Medium, 59);
    passwordText->setTextColor(Color4B(Style::Color::battleshipGrey));
    passwordText->setHorizontalAlignment(TextHAlignment::RIGHT);
    passwordText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    passwordText->setPosition(Vec2(passwordEditboxLayout->getContentSize().width * 0.225f, passwordEditboxLayout->getContentSize().height / 2));
    passwordEditboxLayout->addChild(passwordText);
    
    _passwordEditBox = TextInputLayer::createSettingsChatTextInput(this->getContentSize().width * 0.6f);
    _passwordEditBox->setCenterPosition(Vec2(passwordEditboxLayout->getContentSize().width * 0.55f, passwordEditboxLayout->getContentSize().height / 2));
    passwordEditboxLayout->addChild(_passwordEditBox);
    
    ui::Button* editPasswordButton = ui::Button::create("res/settings/edit_text_input.png");
    editPasswordButton->setPosition(Vec2(passwordEditboxLayout->getContentSize().width * 0.85f - 10, passwordEditboxLayout->getContentSize().height / 2));
    editPasswordButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    passwordEditboxLayout->addChild(editPasswordButton);
    
    float remainingPadding = lowestY - 400;
    
    _accountTypeLayout = ui::Layout::create();
    _accountTypeLayout->setBackGroundImage("res/settings/rounded_rect_half.png");
    _accountTypeLayout->setBackGroundImageColor(Style::Color::skyBlue);
    _accountTypeLayout->setContentSize(Size(this->getContentSize().width, 400));
    _accountTypeLayout->setBackGroundImageScale9Enabled(true);
    _accountTypeLayout->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,remainingPadding,0,0)));
    
    if(ParentDataProvider::getInstance()->isPaidUser())
    {
        Label* subDeetsLab = Label::createWithTTF(StringUtils::format(StringMgr::getInstance()->getStringForKey(SETTINGS_ACCOUNT_PREMIUM_LABEL).c_str(),ParentDataProvider::getInstance()->getBillingDate().c_str()), Style::Font::Medium, 59);
        subDeetsLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        subDeetsLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        subDeetsLab->setTextColor(Color4B::WHITE);
        subDeetsLab->setHorizontalAlignment(TextHAlignment::CENTER);
        _accountTypeLayout->addChild(subDeetsLab);
        
        const std::string& billingProvider = ParentDataProvider::getInstance()->getBillingProvider();
        if(billingProvider == "APPLE" || billingProvider == "GOOGLE" || billingProvider == "AMAZON")
        {
            // add button to link to sub manager
        }
    }
    else
    {
        Label* subDeetsLab = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(SETTINGS_ACCOUNT_FREE_LABEL), Style::Font::Medium, 59);
        subDeetsLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        subDeetsLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        subDeetsLab->setTextColor(Color4B::WHITE);
        subDeetsLab->setHorizontalAlignment(TextHAlignment::CENTER);
        _accountTypeLayout->addChild(subDeetsLab);
    }
    
    this->addChild(_accountTypeLayout);
    
    Super::onEnter();
}

// delegate functions
void EditAccountLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagUpdateParentDetails)
    {
        ModalMessages::getInstance()->stopLoading();
        ParentDataParser::getInstance()->parseParentDetails(body);
        _nameText->setString(ParentDataProvider::getInstance()->getParentDisplayName());
        _editNameButton->setPosition((_displayNameLayout->getContentSize() * 0.5) + Size(_nameText->getContentSize().width * 0.5f,0));
        _editNameInput->setText(ParentDataProvider::getInstance()->getParentDisplayName());
    }
}
void EditAccountLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
}

NS_AZOOMEE_END

