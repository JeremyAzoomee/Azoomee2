//
//  KidDetailsLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/09/2018.
//

#include "KidDetailsLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/NativeShare/NativeShare.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "SettingsMessageBoxDeleteChild.h"
#include "SettingsMessageBoxFREvent.h"
#include "SettingsMessageBoxNotification.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool KidDetailsLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/settings/rounded_rect.png");
    setBackGroundImageScale9Enabled(true);
    setClippingEnabled(true);
    
    return true;
}

void KidDetailsLayer::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
    Sprite* leftWire = Sprite::create("res/settings/wire_left.png");
    leftWire->setAnchorPoint(::Vec2::ANCHOR_MIDDLE_LEFT);
    leftWire->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    leftWire->setScale(contentSize.height / leftWire->getContentSize().height);
    this->addChild(leftWire);
    
    Sprite* rightWire = Sprite::create("res/settings/wire_right.png");
    rightWire->setAnchorPoint(::Vec2::ANCHOR_MIDDLE_RIGHT);
    rightWire->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    rightWire->setScale(contentSize.height / rightWire->getContentSize().height);
    this->addChild(rightWire);
    
    ui::Layout* centralContentLayout = ui::Layout::create();
    centralContentLayout->setContentSize(contentSize);
    centralContentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    centralContentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    centralContentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    this->addChild(centralContentLayout);
    
    _nameLayout = ui::Layout::create();
    _nameLayout->setContentSize(Size(contentSize.width * 0.6f, 157));
    _nameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,80,0,0)));
    
    centralContentLayout->addChild(_nameLayout);
    
    _editNameLayout = ui::Layout::create();
    _editNameLayout->setContentSize(_nameLayout->getContentSize());
    _editNameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _editNameLayout->setVisible(false);
    _nameLayout->addChild(_editNameLayout);
    
    _editNameInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_CHILD_NAME);
    _editNameInput->setCenterPosition(_editNameLayout->getContentSize() / 2);
    _editNameInput->setText(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum));
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
                    HttpRequestCreator* request = API::UpdateChildNameRequest(ParentDataProvider::getInstance()->getIDForAvailableChildren(_childNum), _editNameInput->getText(), this);
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
    
    _nameText = Label::createWithTTF(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum), Style::Font::Medium(), 107);
    _nameText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _nameText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _nameText->setTextColor(Color4B::BLACK);
    reduceLabelTextToFitWidth(_nameText, _nameLayout->getContentSize().width * 0.8f);
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
    
    ui::Layout* oomeeLayout = ui::Layout::create();
    oomeeLayout->setContentSize(Size(contentSize.height * 0.4f, contentSize.height * 0.4f));
    oomeeLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    centralContentLayout->addChild(oomeeLayout);
    
    Sprite* bgCircle1 = Sprite::create("res/oomeeMaker/circle_0.png");
    bgCircle1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgCircle1->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    bgCircle1->setScale(0);
    bgCircle1->setOpacity(25);
    bgCircle1->setRotation(RandomHelper::random_real(0.0,M_PI));
    bgCircle1->setColor(Style::Color::darkTeal);
    oomeeLayout->addChild(bgCircle1, -1);
    
    auto popIn1 = EaseBackOut::create(ScaleTo::create(0.5, ((oomeeLayout->getContentSize().height * 0.85) / bgCircle1->getContentSize().height)));
    auto rotate1 = RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360));
    
    bgCircle1->runAction(popIn1);
    bgCircle1->runAction(rotate1);
    
    Sprite* bgCircle2 = Sprite::create("res/oomeeMaker/circle_1.png");
    bgCircle2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgCircle2->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    bgCircle2->setScale(0);
    bgCircle2->setOpacity(25);
    bgCircle2->setRotation(RandomHelper::random_real(0.0,M_PI));
    bgCircle2->setColor(Style::Color::darkTeal);
    oomeeLayout->addChild(bgCircle2, -1);
    
    auto popIn2 = EaseBackOut::create(ScaleTo::create(0.5, ((oomeeLayout->getContentSize().height * 1.15f) / bgCircle2->getContentSize().height)));
    auto rotate2 = RepeatForever::create(RotateBy::create(30 +  CCRANDOM_0_1() * 30, -360));
    
    bgCircle2->runAction(popIn2);
    bgCircle2->runAction(rotate2);
    
    _oomee = RemoteImageSprite::create();
    _oomee->initWithUrlAndSizeWithoutPlaceholder(ParentDataProvider::getInstance()->getAvatarForAnAvailableChild(_childNum), oomeeLayout->getContentSize());
    _oomee->setKeepAspectRatio(true);
    _oomee->setAnchorPoint(Vec2(0.5,0.4));
    _oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    oomeeLayout->addChild(_oomee);
	
	Label* kidCodeHeading = Label::createWithTTF(_("Share your kid code"), Style::Font::Medium(), 64);
	kidCodeHeading->setTextColor(Color4B::BLACK);
	kidCodeHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	kidCodeHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	kidCodeHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	kidCodeHeading->setVerticalAlignment(TextVAlignment::CENTER);
	kidCodeHeading->setOverflow(Label::Overflow::SHRINK);
	kidCodeHeading->setDimensions(this->getContentSize().width * 0.7f, 100);
	
	ui::Layout* kidCodeHeadingHolder = ui::Layout::create();
	kidCodeHeadingHolder->setContentSize(kidCodeHeading->getContentSize());
	kidCodeHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	kidCodeHeadingHolder->addChild(kidCodeHeading);
	centralContentLayout->addChild(kidCodeHeadingHolder);
	
    Sprite* kidCodeBackground = Sprite::create("res/settings/kid_code_bg.png");
    kidCodeBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kidCodeBackground->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    
    _kidCodeLayout = ui::Layout::create();
    _kidCodeLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,25,0,0)));
    _kidCodeLayout->setContentSize(kidCodeBackground->getContentSize());
    _kidCodeLayout->addChild(kidCodeBackground);
    _kidCodeLayout->setTouchEnabled(true);
    _kidCodeLayout->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            const std::string& childsName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum);
            
            nativeShareScreenString(StringUtils::format(_("%s uses Azoomee to chat safely with family & friends. Enter %s's Kid Code %s in Azoomee to start chatting with your child.\nDownload Azoomee here: http://bit.ly/azoomeekids").c_str(),childsName.c_str(),childsName.c_str(),ParentDataProvider::getInstance()->getInviteCodeForAnAvailableChild(_childNum).c_str()));
        }
    });
    centralContentLayout->addChild(_kidCodeLayout);
    
    Label* kidCode = Label::createWithTTF(ParentDataProvider::getInstance()->getInviteCodeForAnAvailableChild(_childNum), Style::Font::Medium(), 91);
    kidCode->setTextColor(Color4B::WHITE);
    kidCode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kidCode->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _kidCodeLayout->addChild(kidCode);
    
    Sprite* shareButton = Sprite::create("res/settings/kid_code_share.png");
    shareButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    shareButton->setPosition(Vec2(_kidCodeLayout->getContentSize().width - 30, _kidCodeLayout->getContentSize().height * 0.5f));
    _kidCodeLayout->addChild(shareButton);
	
	Label* inputHeading = Label::createWithTTF(_("Enter Friend's Kid Code"), Style::Font::Medium(), 64);
	inputHeading->setTextColor(Color4B::BLACK);
	inputHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	inputHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	inputHeading->setVerticalAlignment(TextVAlignment::CENTER);
	inputHeading->setOverflow(Label::Overflow::SHRINK);
	inputHeading->setDimensions(this->getContentSize().width * 0.7f, 100);
	
	ui::Layout* inputHeadingHolder = ui::Layout::create();
	inputHeadingHolder->setContentSize(inputHeading->getContentSize());
	inputHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	inputHeadingHolder->addChild(inputHeading);
	centralContentLayout->addChild(inputHeadingHolder);
	
    ui::Layout* addFriendInputLayout = ui::Layout::create();
    addFriendInputLayout->setContentSize(Size(_kidCodeLayout->getContentSize().width, 160));
    addFriendInputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,25,0,0)));
    centralContentLayout->addChild(addFriendInputLayout);
    
    ui::Button* addFriendButton = ui::Button::create("res/settings/add_freind_code_button.png");
    addFriendButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    addFriendButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	
    addFriendButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_kidCodeInput->inputIsValid())
            {
                ModalMessages::getInstance()->startLoading();
                HttpRequestCreator* request = API::FriendRequest(ParentDataProvider::getInstance()->getIDForAvailableChildren(_childNum), ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum), _kidCodeInput->getText(), this);
                request->execute();
            }
        }
    });
    float inputWidth = addFriendInputLayout->getContentSize().width - addFriendButton->getContentSize().width + 10;
    _kidCodeInput = TextInputLayer::createSettingsBoxTextInput(inputWidth, INPUT_IS_KIDS_CODE);
    _kidCodeInput->setCenterPosition(Vec2(inputWidth * 0.5f, 80));
    addFriendInputLayout->addChild(_kidCodeInput);
    addFriendInputLayout->addChild(addFriendButton);
	
    _deleteButton = ui::Button::create("res/settings/bin_button_4.png");
    _deleteButton->setAnchorPoint(Vec2(-0.5,1.5));
    _deleteButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _deleteButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			const std::string& targetChildId = ParentDataProvider::getInstance()->getIDForAvailableChildren(_childNum);
			if(ChildDataProvider::getInstance()->getIsChildLoggedIn() && ChildDataProvider::getInstance()->getLoggedInChildId() == targetChildId)
			{
				SettingsMessageBoxNotification* messageBox = SettingsMessageBoxNotification::create();
				messageBox->setHeading(_("You can't do that right now, this child is currently logged in."));
				messageBox->setDelegate(this);
				Director::getInstance()->getRunningScene()->addChild(messageBox,100);
			}
			else
			{
            	SettingsMessageBoxDeleteChild* messageBox = SettingsMessageBoxDeleteChild::create();
            	messageBox->setDelegate(this);
            	Director::getInstance()->getRunningScene()->addChild(messageBox,100);
			}
        }
    });
    this->addChild(_deleteButton);
    
    Super::onEnter();
}

void KidDetailsLayer::setChildNum(int childNum)
{
    _childNum = childNum;
}

void KidDetailsLayer::setDeleteChildCallback(const Azoomee::KidDetailsLayer::DeleteChildCallback &callback)
{
    _deleteCallback = callback;
}

// Delegate Functions

void KidDetailsLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagDeleteChild)
    {
        ModalMessages::getInstance()->stopLoading();
        if(_deleteCallback)
        {
            _deleteCallback();
        }
    }
    else if(requestTag == API::TagFriendRequest)
    {
        ModalMessages::getInstance()->stopLoading();
        SettingsMessageBoxFREvent* messageBox = SettingsMessageBoxFREvent::create();
        messageBox->setDelegate(this);
        messageBox->setType(EventType::SUCCESS);
        Director::getInstance()->getRunningScene()->addChild(messageBox,100);
    }
    else if(requestTag == API::TagUpdateChildNameRequest)
    {
        ModalMessages::getInstance()->stopLoading();
		rapidjson::Document data;
		data.Parse(body.c_str());
		if(!data.HasParseError())
		{
			ParentDataParser::getInstance()->parseChildUpdateData(_childNum, body);
			_nameText->setString(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum));
			reduceLabelTextToFitWidth(_nameText, _nameLayout->getContentSize().width * 0.8f);
			_editNameButton->setPosition((_displayNameLayout->getContentSize() * 0.5) + Size(_nameText->getContentSize().width * 0.5f,0));
			_editNameInput->setText(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum));
		}
    }
}

void KidDetailsLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    if(requestTag == API::TagFriendRequest)
    {
        SettingsMessageBoxFREvent* messageBox = SettingsMessageBoxFREvent::create();
        messageBox->setDelegate(this);
        messageBox->setType(EventType::FAIL);
        Director::getInstance()->getRunningScene()->addChild(messageBox,100);
    }
}

void KidDetailsLayer::onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type)
{
    switch(type)
    {
        case SettingsMessageBoxButtonType::CANCEL:
        {
            pSender->removeFromParent();
            break;
        }
        case SettingsMessageBoxButtonType::DELETE:
        {
            pSender->removeFromParent();
            ModalMessages::getInstance()->startLoading();
            HttpRequestCreator* request = API::DeleteChild(ParentDataProvider::getInstance()->getIDForAvailableChildren(_childNum),
                                                           ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum),
                                                           ParentDataProvider::getInstance()->getSexForAnAvailableChild(_childNum),
                                                           this);
            request->execute();
            break;
        }
        case SettingsMessageBoxButtonType::CLOSE:
        {
            pSender->removeFromParent();
            break;
        }
        case SettingsMessageBoxButtonType::SEND:
        {
            SettingsMessageBoxFREvent* messageBox = dynamic_cast<SettingsMessageBoxFREvent*>(pSender);
            if(messageBox)
            {
                ModalMessages::getInstance()->startLoading();
                HttpRequestCreator* request = API::FriendRequest(ParentDataProvider::getInstance()->getIDForAvailableChildren(_childNum), ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum), messageBox->getInputString(), this);
                request->execute();
            }
            pSender->removeFromParent();
            break;
        }
        default:
            break;
    }
}

NS_AZOOMEE_END

