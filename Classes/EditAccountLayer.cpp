//
//  EditAccountLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/08/2018.
//

#include "EditAccountLayer.h"
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
    
    _nameText = ui::Text::create(ParentDataProvider::getInstance()->getParentDisplayName(), Style::Font::Medium, 107);
    _nameText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,80,0,0)));
    _nameText->setTextColor(Color4B::BLACK);
    this->addChild(_nameText);
    
    lowestY -= (_nameText->getContentSize().height + 80);
    
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
    
    Label* pinText = Label::createWithTTF("Pin", Style::Font::Medium, 59);
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
    
    Label* passwordText = Label::createWithTTF("Password", Style::Font::Medium, 59);
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
        Label* subDeetsLab = Label::createWithTTF(StringUtils::format("Premium Subscription\nRenew’s on %s",ParentDataProvider::getInstance()->getBillingDate().c_str()), Style::Font::Medium, 59);
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
        Label* subDeetsLab = Label::createWithTTF("Free Account\nTap Renew for a special offer", Style::Font::Medium, 59);
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
    }
}
void EditAccountLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    
}

NS_AZOOMEE_END

