//
//  PurchaseCapsule.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 17/09/2019.
//

#include "PurchaseCapsule.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include "AgeGate.h"
#include "SceneManagerScene.h"
#include "LoginController.h"
#include "Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float PurchaseCapsule::kDropshadowPadding = 60.0f;
const float PurchaseCapsule::kLoginWidthPercent = 0.35f;

bool PurchaseCapsule::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/hqscene/purchase_capsule.png");
    setSwallowTouches(true);
    
    _stencil = ui::Scale9Sprite::create("res/hqscene/purchase_capsule.png");
    _stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _stencil->setPosition(Vec2(-kDropshadowPadding, -kDropshadowPadding));
    
    _clippingNode = ClippingNode::create(_stencil);
    _clippingNode->setAlphaThreshold(0.9f);
    _clippingNode->setPosition(Vec2(kDropshadowPadding, kDropshadowPadding));
    addChild(_clippingNode);
    
    _purchaseLayout = ui::Layout::create();
    _purchaseLayout->setSizeType(SizeType::PERCENT);
    _purchaseLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _purchaseLayout->setBackGroundColor(Colours::Color_3B::macaroniAndCheese);
    _purchaseLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _purchaseLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _purchaseLayout->setTouchEnabled(true);
    _purchaseLayout->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        switch(eType)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                _purchaseText->setScale(1.1f);
                break;
            }
            case ui::Widget::TouchEventType::ENDED:
            {
                _purchaseText->setScale(1.0f);
    #ifdef ALLOW_UNPAID_SIGNUP
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Signup));
    #else
    #ifndef AZOOMEE_VODACOM_BUILD
                AgeGate* ageGate = AgeGate::create();
                ageGate->setActionCompletedCallback([ageGate](AgeGateResult result){
                    ageGate->removeFromParent();
                    if(result == AgeGateResult::SUCCESS)
                    {
                        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::IAP));
                    }
                });
                Director::getInstance()->getRunningScene()->addChild(ageGate,AGE_GATE_Z_ORDER);
    #else
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::VodacomOnboarding));
    #endif
    #endif
                break;
            }
            case ui::Widget::TouchEventType::CANCELED:
            {
                _purchaseText->setScale(1.0f);
                break;
            }
            case ui::Widget::TouchEventType::MOVED:
            {
                break;
            }
        }
    });
    _clippingNode->addChild(_purchaseLayout);
    
    _purchaseText = DynamicText::create("", Style::Font::PoppinsBold(), 53);
    _purchaseText->setTextColor(Color4B::WHITE);
    _purchaseText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _purchaseText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _purchaseText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _purchaseText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _purchaseText->setOverflow(Label::Overflow::SHRINK);
    _purchaseText->enableUnderline(true);
    _purchaseLayout->addChild(_purchaseText);
    
    _loginLayout = ui::Layout::create();
    _loginLayout->setSizeType(SizeType::PERCENT);
    _loginLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _loginLayout->setBackGroundColor(Color3B::WHITE);
    _loginLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _loginLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    _loginLayout->setTouchEnabled(true);
    _loginLayout->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        switch(eType)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                _loginText->setScale(1.1f);
                break;
            }
            case ui::Widget::TouchEventType::ENDED:
            {
                _loginText->setScale(1.0f);
                LoginController::getInstance()->setLoginOrigin(LoginOrigin::HQ);
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
                break;
            }
            case ui::Widget::TouchEventType::CANCELED:
            {
                _loginText->setScale(1.0f);
                break;
            }
            case ui::Widget::TouchEventType::MOVED:
            {
                break;
            }
        }
    });
    _clippingNode->addChild(_loginLayout);
    
    _loginText = DynamicText::create(_("Log In"), Style::Font::PoppinsBold(), 53);
    _loginText->setTextColor(Color4B(Colours::Color_3B::macaroniAndCheese));
    _loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loginText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _loginText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _loginText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _loginText->setOverflow(Label::Overflow::SHRINK);
    _loginText->enableUnderline(true);
    _loginLayout->addChild(_loginText);
    
    setContentSize(Size(1240,296));
    
    return true;
}

void PurchaseCapsule::onEnter()
{
    Super::onEnter();
    onSizeChanged();
}

void PurchaseCapsule::onExit()
{
    Super::onExit();
}

void PurchaseCapsule::onSizeChanged()
{
    Super::onSizeChanged();
    _clippingNode->setContentSize(getContentSize() - Size(2 * kDropshadowPadding, 2 * kDropshadowPadding));
    _stencil->setContentSize(getContentSize());
    _purchaseText->setTextAreaSize(Size(_purchaseLayout->getContentSize().width * 0.75f, _purchaseText->getContentSize().height));
    _loginText->setTextAreaSize(Size(_loginLayout->getContentSize().width * 0.75f, _loginText->getContentSize().height));
}

void PurchaseCapsule::setUserType(BillingStatus userType)
{
    _userType = userType;
    setupForCurrentState();
}

void PurchaseCapsule::setupForCurrentState()
{
    switch(_userType)
    {
        case BillingStatus::ANON:
        {
            float loginWidthPercent = kLoginWidthPercent;
            _purchaseLayout->setSizePercent(Vec2(1.0f - loginWidthPercent, 1.0f));
            _loginLayout->setSizePercent(Vec2(loginWidthPercent,1.0f));
#ifdef AZOOMEE_VODACOM_BUILD
            _purchaseText->setString(_("Unlock everything"));
#else
            _purchaseText->setString(_("Get 7 Days free"));
#endif
            _purchaseLayout->setTouchEnabled(true);
            _loginLayout->setTouchEnabled(true);
            break;
        }
        case BillingStatus::FREE_REGISTERED:
        {
            _purchaseText->setString(_("Reactivate your account"));
            float loginWidthPercent = 0.0f;
            _purchaseLayout->setSizePercent(Vec2(1.0f - loginWidthPercent, 1.0f));
            _loginLayout->setSizePercent(Vec2(loginWidthPercent,1.0f));
            _purchaseLayout->setTouchEnabled(true);
            _loginLayout->setTouchEnabled(false);
            break;
        }
        case BillingStatus::SUBSCRIBED: case BillingStatus::FREE_TRIAL:
        {
            _purchaseText->setString(_("Congratulations! You are now a Premium User"));
            float loginWidthPercent = 0.0f;
            _purchaseLayout->setSizePercent(Vec2(1.0f - loginWidthPercent, 1.0f));
            _loginLayout->setSizePercent(Vec2(loginWidthPercent,1.0f));
            _purchaseLayout->setTouchEnabled(false);
            _loginLayout->setTouchEnabled(false);
            setVisible(false);
            break;
        }
    }
    onSizeChanged();
}

NS_AZOOMEE_END
