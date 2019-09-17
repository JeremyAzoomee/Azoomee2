//
//  PurchaseCapsule.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 17/09/2019.
//

#include "PurchaseCapsule.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include "AgeGate.h"
#include "SceneManagerScene.h"
#include "LoginLogicHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float PurchaseCapsule::kDropshadowPadding = 60.0f;

bool PurchaseCapsule::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/hqscene/purchase_capsule.png");
    
    _stencil = ui::Scale9Sprite::create("res/hqscene/purchase_capsule.png");
    _stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _stencil->setPosition(Vec2(-kDropshadowPadding, -kDropshadowPadding));
    
    _clippingNode = ClippingNode::create(_stencil);
    _clippingNode->setAlphaThreshold(0.8f);
    _clippingNode->setPosition(Vec2(kDropshadowPadding, kDropshadowPadding));
    addChild(_clippingNode);
    
    _purchaseLayout = ui::Layout::create();
    _purchaseLayout->setSizeType(SizeType::PERCENT);
    _purchaseLayout->setSizePercent(Vec2(0.68f, 1.0f));
    _purchaseLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _purchaseLayout->setBackGroundColor(Style::Color::macaroniAndCheese);
    _purchaseLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _purchaseLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
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
    _purchaseLayout->addChild(_purchaseText);
    
    _loginLayout = ui::Layout::create();
    _loginLayout->setSizeType(SizeType::PERCENT);
    _loginLayout->setSizePercent(Vec2(0.32f, 1.0f));
    _loginLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _loginLayout->setBackGroundColor(Color3B::WHITE);
    _loginLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _loginLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
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
                LoginLogicHandler::getInstance()->setLoginOrigin(LoginOrigin::HQ);
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
    _loginText->setTextColor(Color4B(Style::Color::macaroniAndCheese));
    _loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loginText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _loginText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _loginText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _loginText->setOverflow(Label::Overflow::SHRINK);
    _loginLayout->addChild(_loginText);
    
    setContentSize(Size(1240,296));
    
    return true;
}

void PurchaseCapsule::onEnter()
{
    Super::onEnter();
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
    _purchaseText->setTextAreaSize(Size(_purchaseLayout->getContentSize().width * 0.7f, _purchaseText->getContentSize().height));
    _loginText->setTextAreaSize(Size(_loginLayout->getContentSize().width * 0.7f, _loginText->getContentSize().height));
}

void PurchaseCapsule::setUserType(UserBillingType userType)
{
    _userType = userType;
    setupForCurrentState();
}

void PurchaseCapsule::setupForCurrentState()
{
    switch(_userType)
    {
        case ANON:
        {
            _purchaseLayout->setSizePercent(Vec2(0.68f, 1.0f));
            _loginLayout->setSizePercent(Vec2(0.32f,1.0f));
#ifdef AZOOMEE_VODACOM_BUILD
            _purchaseText->setString(_("Unlock everything"));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            _purchaseText->setString(_("Buy Azoomee Pass"));
#else
            _purchaseText->setString(_("Get 7 Days free"));
#endif
            break;
        }
        case LAPSED:
        {
            _purchaseText->setString(_("Reactivate your account"));
            _purchaseLayout->setSizePercent(Vec2(1.0f, 1.0f));
            _loginLayout->setSizePercent(Vec2(0.0f,1.0f));
            break;
        }
        case PAID:
        {
            _purchaseText->setString(_("Congratulations! You are now a Premium User"));
            _purchaseLayout->setSizePercent(Vec2(1.0f, 1.0f));
            _loginLayout->setSizePercent(Vec2(0.0f,1.0f));
            break;
        }
    }
}

NS_AZOOMEE_END
