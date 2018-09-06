//
//  KidDetailsLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/09/2018.
//

#include "KidDetailsLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/NativeShare/NativeShare.h>

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
    
    Sprite* leftWire = Sprite::create("res/settings/wire_left.png");
    leftWire->setAnchorPoint(::Vec2::ANCHOR_MIDDLE_LEFT);
    leftWire->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    leftWire->setScale(this->getContentSize().height / leftWire->getContentSize().height);
    this->addChild(leftWire);
    
    Sprite* rightWire = Sprite::create("res/settings/wire_right.png");
    rightWire->setAnchorPoint(::Vec2::ANCHOR_MIDDLE_RIGHT);
    rightWire->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    rightWire->setScale(this->getContentSize().height / rightWire->getContentSize().height);
    this->addChild(rightWire);
    
    ui::Layout* centralContentLayout = ui::Layout::create();
    centralContentLayout->setContentSize(this->getContentSize());
    centralContentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    centralContentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    centralContentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    this->addChild(centralContentLayout);
    
    ui::Layout* nameLayout = ui::Layout::create();
    nameLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    
    _nameLabel = Label::createWithTTF(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum), Style::Font::Medium, 107);
    _nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _nameLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _nameLabel->setTextColor(Color4B::BLACK);
    if(_nameLabel->getContentSize().width > this->getContentSize().width * 0.6f)
    {
        reduceLabelTextToFitWidth(_nameLabel, this->getContentSize().width * 0.6f);
    }
    
    ui::Button* editNameButton = ui::Button::create("res/settings/edit_button_circle.png");
    editNameButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    editNameButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    
    nameLayout->setContentSize(Size(_nameLabel->getContentSize().width + (3 * editNameButton->getContentSize().width), _nameLabel->getContentSize().height));
    nameLayout->addChild(_nameLabel);
    nameLayout->addChild(editNameButton);
    centralContentLayout->addChild(nameLayout);
    
    ui::Layout* oomeeLayout = ui::Layout::create();
    oomeeLayout->setContentSize(Size(this->getContentSize().height * 0.45f, this->getContentSize().height * 0.45f));
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
    
    Sprite* kidCodeBackground = Sprite::create("res/settings/kid_code_bg.png");
    kidCodeBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kidCodeBackground->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    
    _kidCodeLayout = ui::Layout::create();
    _kidCodeLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    _kidCodeLayout->setContentSize(kidCodeBackground->getContentSize());
    _kidCodeLayout->addChild(kidCodeBackground);
    _kidCodeLayout->setTouchEnabled(true);
    _kidCodeLayout->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            const std::string& childsName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(_childNum);
            
            nativeShareScreenString(StringUtils::format("%s uses Azoomee to chat safely with family & friends. Enter %s's Kid Code %s in Azoomee to start chatting with your child.\nDownload Azoomee here: http://bit.ly/azoomeekids",childsName.c_str(),childsName.c_str(),ParentDataProvider::getInstance()->getInviteCodeForAnAvailableChild(_childNum).c_str()));
        }
    });
    centralContentLayout->addChild(_kidCodeLayout);
    
    Label* kidCode = Label::createWithTTF(ParentDataProvider::getInstance()->getInviteCodeForAnAvailableChild(_childNum), Style::Font::Medium, 91);
    kidCode->setTextColor(Color4B::WHITE);
    kidCode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kidCode->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _kidCodeLayout->addChild(kidCode);
    
    Sprite* shareButton = Sprite::create("res/settings/kid_code_share.png");
    shareButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    shareButton->setNormalizedPosition(Vec2(0.9,0.5));
    _kidCodeLayout->addChild(shareButton);
    
    ui::Layout* addFriendInputLayout = ui::Layout::create();
    addFriendInputLayout->setContentSize(Size(_kidCodeLayout->getContentSize().width, 160));
    addFriendInputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    centralContentLayout->addChild(addFriendInputLayout);
    
    ui::Button* addFriendButton = ui::Button::create("res/settings/add_freind_code_button.png");
    addFriendButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    addFriendButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    addFriendInputLayout->addChild(addFriendButton);
    
    float inputWidth = addFriendInputLayout->getContentSize().width - addFriendButton->getContentSize().width;
    _kidCodeInput = TextInputLayer::createSettingsChatTextInput(inputWidth);
    _kidCodeInput->setCenterPosition(Vec2(inputWidth * 0.5f, 80));
    addFriendInputLayout->addChild(_kidCodeInput);
    
    _deleteButton = ui::Button::create("res/settings/bin_button_4.png");
    _deleteButton->setAnchorPoint(Vec2(-0.5,1.5));
    _deleteButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _deleteButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            
        }
    });
    this->addChild(_deleteButton);
    
    Super::onEnter();
}

void KidDetailsLayer::setChildNum(int childNum)
{
    _childNum = childNum;
}

// Delegate Functions

void KidDetailsLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    
}

void KidDetailsLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    
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
        case SettingsMessageBoxButtonType::REJECT:
        {
            pSender->removeFromParent();
            break;
        }
        default:
            break;
    }
}

NS_AZOOMEE_END

