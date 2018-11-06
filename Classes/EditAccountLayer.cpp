//
//  EditAccountLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/08/2018.
//

#include "EditAccountLayer.h"
#include "DynamicNodeHandler.h"
#include "SettingsMessageBoxNotification.h"
#include "SettingsMessageBoxTryAgain.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/NativeShare/NativeShare.h>

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
	const Size& contentSize = this->getContentSize();
	
    int lowestY = contentSize.height;
    
    _nameLayout = ui::Layout::create();
    _nameLayout->setContentSize(Size(contentSize.width * 0.6f, 157));
    _nameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,80,0,0)));
    
    this->addChild(_nameLayout);
    
    _editNameLayout = ui::Layout::create();
    _editNameLayout->setContentSize(_nameLayout->getContentSize());
    _editNameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _editNameLayout->setVisible(false);
    _nameLayout->addChild(_editNameLayout);
    
    _editNameInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_CHILD_NAME);
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
    
    _nameText = ui::Text::create(ParentDataProvider::getInstance()->getParentDisplayName(), Style::Font::Medium(), 107);
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
			_editNameInput->focusAndShowKeyboard();
        }
    });
    _displayNameLayout->addChild(_editNameButton);
    
    lowestY -= (_nameLayout->getContentSize().height + 80);
    
    const std::string& username = ParentDataProvider::getInstance()->getParentEmail();
    _emailText = ui::Text::create(username, Style::Font::Medium(), 59);
    _emailText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,40,0,0)));
    _emailText->setTextColor(Color4B::BLACK);
    this->addChild(_emailText);
    
    lowestY -= (_emailText->getContentSize().height + 40);
    
    ui::Layout* pinEditboxLayout = ui::Layout::create();
    pinEditboxLayout->setContentSize(Size(contentSize.width, 190));
    pinEditboxLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,35,0,0)));
    this->addChild(pinEditboxLayout);
    
    lowestY -= (pinEditboxLayout->getContentSize().height + 35);
    
    Label* pinText = Label::createWithTTF(_("PIN"), Style::Font::Medium(), 59);
    pinText->setTextColor(Color4B(Style::Color::battleshipGrey));
	pinText->setHorizontalAlignment(TextHAlignment::RIGHT);
	pinText->setVerticalAlignment(TextVAlignment::CENTER);
	pinText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	pinText->setOverflow(Label::Overflow::SHRINK);
	pinText->setDimensions(pinEditboxLayout->getContentSize().width * 0.2f, 100);
    pinText->setPosition(Vec2(pinEditboxLayout->getContentSize().width * 0.225f, pinEditboxLayout->getContentSize().height / 2));
    pinEditboxLayout->addChild(pinText);
    
    _pinEditbox = TextInputLayer::createSettingsBoxTextInput(contentSize.width * 0.6f, INPUT_IS_PIN);
    _pinEditbox->setCenterPosition(Vec2(pinEditboxLayout->getContentSize().width * 0.55f, pinEditboxLayout->getContentSize().height / 2));
    _pinEditbox->setText(ParentDataProvider::getInstance()->getParentPin());
    _pinEditbox->setEnabled(false);
    pinEditboxLayout->addChild(_pinEditbox);
    
    _editPinButton = ui::Button::create("res/settings/edit_text_input.png");
    _editPinButton->setPosition(Vec2(pinEditboxLayout->getContentSize().width * 0.85f - 20, pinEditboxLayout->getContentSize().height / 2));
    _editPinButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_editPinButton->setContentSize(Size(_pinEditbox->getContentSize().height + 10, _pinEditbox->getContentSize().height + 10));
	_editPinButton->ignoreContentAdaptWithSize(false);
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
					_pinRequest = true;
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
				_pinEditbox->focusAndShowKeyboard();
            }
        }
    });
    pinEditboxLayout->addChild(_editPinButton);
    
    ui::Layout* passwordEditboxLayout = ui::Layout::create();
    passwordEditboxLayout->setContentSize(Size(contentSize.width, 190));
    passwordEditboxLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    this->addChild(passwordEditboxLayout);
    
    lowestY -= passwordEditboxLayout->getContentSize().height;
    
    Label* passwordText = Label::createWithTTF(_("Password"), Style::Font::Medium(), 59);
    passwordText->setTextColor(Color4B(Style::Color::battleshipGrey));
    passwordText->setHorizontalAlignment(TextHAlignment::RIGHT);
	passwordText->setVerticalAlignment(TextVAlignment::CENTER);
    passwordText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	passwordText->setOverflow(Label::Overflow::SHRINK);
	passwordText->setDimensions(passwordEditboxLayout->getContentSize().width * 0.2f, 100);
    passwordText->setPosition(Vec2(passwordEditboxLayout->getContentSize().width * 0.225f, passwordEditboxLayout->getContentSize().height / 2));
    passwordEditboxLayout->addChild(passwordText);
    
    _passwordEditBox = TextInputLayer::createSettingsBoxTextInput(contentSize.width * 0.6f, INPUT_IS_NEW_PASSWORD);
    _passwordEditBox->setCenterPosition(Vec2(passwordEditboxLayout->getContentSize().width * 0.55f, passwordEditboxLayout->getContentSize().height / 2));
	_passwordEditBox->setText("xxxxxx");
	_passwordEditBox->setPlaceholderText(_("Change password"));
	_passwordEditBox->setEnabled(false);
    passwordEditboxLayout->addChild(_passwordEditBox);
	
	_passwordState = EditPasswordState::LOCKED;
	
    _editPasswordButton = ui::Button::create("res/settings/edit_text_input.png");
    _editPasswordButton->setPosition(Vec2(passwordEditboxLayout->getContentSize().width * 0.85f - 20, passwordEditboxLayout->getContentSize().height / 2));
    _editPasswordButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_editPasswordButton->setContentSize(Size(_passwordEditBox->getContentSize().height + 10, _passwordEditBox->getContentSize().height + 10));
	_editPasswordButton->ignoreContentAdaptWithSize(false);
	_editPasswordButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			switch (_passwordState) {
				case EditPasswordState::LOCKED:
				{
					_passwordEditBox->setEnabled(true);
					_passwordEditBox->setText("");
					_passwordEditBox->setPlaceholderText(_("Enter current password"));
					_passwordEditBox->focusAndShowKeyboard();
					_editPasswordButton->loadTextureNormal("res/settings/edit_next.png");
					_passwordState = EditPasswordState::CURRENT_PASSWORD;
					break;
				}
				case EditPasswordState::CURRENT_PASSWORD:
				{
					if(_passwordEditBox->inputIsValid())
					{
						_currentPassword = _passwordEditBox->getText();
						_passwordState = EditPasswordState::NEW_PASSWORD;
						_passwordEditBox->setPlaceholderText(_("Enter new password"));
						_passwordEditBox->setText("");
						_passwordEditBox->focusAndShowKeyboard();
						_editPasswordButton->loadTextureNormal("res/settings/confirm_edit_button.png");
						break;
					}
				}
				case EditPasswordState::NEW_PASSWORD:
				{
					if(_passwordEditBox->inputIsValid())
					{
						ModalMessages::getInstance()->startLoading();
						HttpRequestCreator* request = API::UpdateParentPasswordRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), _currentPassword, _passwordEditBox->getText(), this);
						request->execute();
						
						_passwordEditBox->setEnabled(false);
						_passwordState = EditPasswordState::LOCKED;
						_passwordEditBox->setPlaceholderText(_("Change password"));
						_passwordEditBox->setText("");
						_editPasswordButton->loadTextureNormal("res/settings/edit_text_input.png");
					}
					break;
				}
			}
		}
	});
    passwordEditboxLayout->addChild(_editPasswordButton);
    
    float remainingPadding = lowestY - 400;
    
    _accountTypeLayout = ui::Layout::create();
    _accountTypeLayout->setBackGroundImage("res/settings/rounded_rect_half.png");
    _accountTypeLayout->setBackGroundImageColor(Style::Color::skyBlue);
    _accountTypeLayout->setContentSize(Size(contentSize.width, 400));
    _accountTypeLayout->setBackGroundImageScale9Enabled(true);
    _accountTypeLayout->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,remainingPadding,0,0)));
    
    if(ParentDataProvider::getInstance()->isPaidUser())
    {
#ifdef VODACOM_BUILD
        Label* subDeetsLab = Label::createWithTTF(StringUtils::format(_("Valid voucher\nExpires on %s").c_str(),ParentDataProvider::getInstance()->getBillingDate().c_str()), Style::Font::Medium(), 59);
#else
		Label* subDeetsLab = Label::createWithTTF(_("Premium Subscription"), Style::Font::Medium(), 59);
#endif
        subDeetsLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        subDeetsLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        subDeetsLab->setTextColor(Color4B::WHITE);
        subDeetsLab->setHorizontalAlignment(TextHAlignment::CENTER);
        _accountTypeLayout->addChild(subDeetsLab);
        
        const std::string& billingProvider = ParentDataProvider::getInstance()->getBillingProvider();
        if(billingProvider == ConfigStorage::kBillingProviderApple || billingProvider == ConfigStorage::kBillingProviderGoogle || billingProvider == ConfigStorage::kBillingProviderAmazon)
        {
			subDeetsLab->setNormalizedPosition(Vec2(0.5f,0.66f));
			ui::Button* manageButton = ui::Button::create("res/settings/sub_button.png");
			manageButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			manageButton->setNormalizedPosition(Vec2(0.5f,0.33f));
			manageButton->addTouchEventListener([billingProvider](Ref* pSender, ui::Widget::TouchEventType eType){
				if(eType == ui::Widget::TouchEventType::ENDED)
				{
					if(billingProvider == ConfigStorage::kBillingProviderApple)
					{
						Application::getInstance()->openURL(ConfigStorage::kIOSSubURL);
					}
					else if(billingProvider == ConfigStorage::kBillingProviderGoogle)
					{
						Application::getInstance()->openURL(ConfigStorage::kAndroidSubURL);
					}
					else if(billingProvider == ConfigStorage::kBillingProviderAmazon)
					{
						Application::getInstance()->openURL(ConfigStorage::kAmazonSubURL);
					}
				}
			});
			_accountTypeLayout->addChild(manageButton);
			
			Label* manageLab = Label::createWithTTF(_("Manage"), Style::Font::Medium(), manageButton->getContentSize().height * 0.4f);
			manageLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
			manageLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			manageLab->setTextColor(Color4B::BLACK);
			manageLab->setHorizontalAlignment(TextHAlignment::CENTER);
			manageButton->addChild(manageLab);
        }
    }
    else
    {
#ifdef VODACOM_BUILD
		Label* subDeetsLab = Label::createWithTTF(_("Inactive Voucher\nUnlock all content by adding a voucher"), Style::Font::Medium(), 59);
#else
		Label* subDeetsLab = Label::createWithTTF(_("Free Account\nTap Renew for a special offer"), Style::Font::Medium(), 59);
#endif
        subDeetsLab->setNormalizedPosition(Vec2(0.5f,0.66f));
        subDeetsLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        subDeetsLab->setTextColor(Color4B::WHITE);
        subDeetsLab->setHorizontalAlignment(TextHAlignment::CENTER);
        _accountTypeLayout->addChild(subDeetsLab);
		
		ui::Button* resubButton = ui::Button::create("res/settings/sub_button.png");
		resubButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		resubButton->setNormalizedPosition(Vec2(0.5f,0.33f));
		resubButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Widget::TouchEventType::ENDED)
			{
				DynamicNodeHandler::getInstance()->startIAPFlow();
			}
		});
		_accountTypeLayout->addChild(resubButton);
		
#ifdef VODACOM_BUILD
		Label* resubLab = Label::createWithTTF(_("Add new voucher"), Style::Font::Medium(), resubButton->getContentSize().height * 0.4f);
#else
		Label* resubLab = Label::createWithTTF(_("Renew subscription"), Style::Font::Medium(), resubButton->getContentSize().height * 0.4f);
#endif
		resubLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		resubLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		resubLab->setTextColor(Color4B::BLACK);
		resubLab->setHorizontalAlignment(TextHAlignment::CENTER);
		resubButton->addChild(resubLab);
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
		if(_pinRequest)
		{
			_pinRequest = false;
			_pinEditbox->setText(ParentDataProvider::getInstance()->getParentPin());
			SettingsMessageBoxNotification* messageBox = SettingsMessageBoxNotification::create();
			messageBox->setHeading(_("Your pin number has been updated!"));
			messageBox->setDelegate(this);
			Director::getInstance()->getRunningScene()->addChild(messageBox,100);
		}
		else
		{
			_nameText->setString(ParentDataProvider::getInstance()->getParentDisplayName());
			_editNameButton->setPosition((_displayNameLayout->getContentSize() * 0.5) + Size(_nameText->getContentSize().width * 0.5f,0));
			_editNameInput->setText(ParentDataProvider::getInstance()->getParentDisplayName());
		}
    }
	else if(requestTag == API::TagUpdateParentPassword)
	{
		ModalMessages::getInstance()->stopLoading();
		SettingsMessageBoxNotification* messageBox = SettingsMessageBoxNotification::create();
		messageBox->setHeading(_("Your password has been updated!"));
		messageBox->setDelegate(this);
		Director::getInstance()->getRunningScene()->addChild(messageBox,100);
		
	}
}
void EditAccountLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
	if(requestTag == API::TagUpdateParentPassword)
	{
		SettingsMessageBoxTryAgain* messageBox = SettingsMessageBoxTryAgain::create();
		messageBox->setHeading(_("Oops!"));
		messageBox->setSubHeading(_("The current password you entered doesn't seem to match up with what we’ve got"));
		messageBox->setDelegate(this);
		Director::getInstance()->getRunningScene()->addChild(messageBox,100);
	}
}

void EditAccountLayer::onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type)
{
	pSender->removeFromParent();
	if(type == SettingsMessageBoxButtonType::TRY_AGAIN)
	{
		_passwordEditBox->setEnabled(true);
		_passwordEditBox->setPlaceholderText(_("Enter current password"));
		_passwordEditBox->focusAndShowKeyboard();
		_editPasswordButton->loadTextureNormal("res/settings/edit_next.png");
		_passwordState = EditPasswordState::CURRENT_PASSWORD;
	}
}

NS_AZOOMEE_END

