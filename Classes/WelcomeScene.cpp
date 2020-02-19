//
//  WelcomeScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/01/2019.
//

#include "WelcomeScene.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "SceneManagerScene.h"
#include "BackEndCaller.h"
#include "LoginController.h"
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool WelcomeScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = this->getContentSize();
	
    _bgColour = ui::Layout::create();
    _bgColour->setSizeType(ui::Layout::SizeType::PERCENT);
    _bgColour->setSizePercent(Vec2(1.0,1.0));
    _bgColour->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _bgColour->setBackGroundColor(Colours::Color_3B::darkIndigo);
    addChild(_bgColour);
	
    _contentTiles = Sprite::create("res/introAssets/welcome_tiles.png");
    _contentTiles->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(_contentTiles);
    
	_bottomGradient = LayerGradient::create();
    _bottomGradient->setStartColor(Colours::Color_3B::darkIndigo);
    _bottomGradient->setStartOpacity(255);
    _bottomGradient->setEndColor(Colours::Color_3B::darkIndigo);
    _bottomGradient->setEndOpacity(0);
    _bottomGradient->setVector(Vec2(0,1));
    _bottomGradient->setIgnoreAnchorPointForPosition(false);
    _bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bottomGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_bottomGradient);

    _fillColour = Sprite::create("res/decoration/white_1px.png");
    _fillColour->setColor(Colours::Color_3B::darkIndigo);
    _fillColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _fillColour->setNormalizedPosition(Vec2(0.5f, -0.1f));
    addChild(_fillColour);
    
	_body = ui::Layout::create();
	_body->setLayoutType(ui::Layout::Type::VERTICAL);
	_body->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_body->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	addChild(_body);
	
    _logo = ui::ImageView::create("res/introAssets/Azoomee_WHITE.png");
    _logo->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _body->addChild(_logo);
    
    _text = DynamicText::create(_("introText"), Style::Font::PoppinsBold(), 80);
    _text->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _text->setTextVerticalAlignment(TextVAlignment::CENTER);
    _text->setTextColor(Color4B::WHITE);
    _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _text->enableShadow(Color4B(0,0,0,125), Size(4,-8));
    _text->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,20,0,0)));
    _body->addChild(_text);
    
    _button = TextButton::create("res/settings/rounded_button.png");
    _button->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,20,0,0)));
    _button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _button->ignoreContentAdaptWithSize(false);
    _button->setContentSize(Size(670,140));
    _button->setColor(Colours::Color_3B::strongPink);
    _button->setTextColour(Color4B::WHITE);
    _button->setTextFontInfo(Style::Font::PoppinsBold(), 67);
    _button->setText(_("introButton"));
    _button->setTextAreaSizePercent(Vec2(0.7f, 0.8f));
    _button->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("WelcomeScreen_GetStarted");
            LoginController::getInstance()->anonLogin();
        }
    });
    _body->addChild(_button);
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 20 + _text->getContentSize().height + _button->getContentSize().height));
	
    
    _loginLayout = ui::Layout::create();
    _loginLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _loginLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loginLayout->setNormalizedPosition(Vec2(0.5,0.2));
    _loginLayout->setTouchEnabled(true);
    _loginLayout->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("WelcomeScreen_Login");
            LoginController::getInstance()->setLoginOrigin(LoginOrigin::LOGOUT);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
        }
    });
    addChild(_loginLayout);
    
    _loginText = DynamicText::create(_("Already have an account?"), Style::Font::PoppinsRegular(), 37);
    _loginText->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _loginText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _loginText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _loginText->setTextColor(Color4B::WHITE);
    _loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loginText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _loginLayout->addChild(_loginText);
    
    _loginButton = DynamicText::create(_("Log In"), Style::Font::PoppinsBold(), 53);
    _loginButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _loginButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loginButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _loginButton->setTextVerticalAlignment(TextVAlignment::CENTER);
    _loginButton->enableUnderline(true);
    _loginButton->ignoreContentAdaptWithSize(false);
    _loginButton->setTextColor(Color4B::WHITE);
    _loginLayout->addChild(_loginButton);
    
    _loginLayout->setContentSize(Size(contentSize.width, _loginButton->getContentSize().height + _loginText->getContentSize().height));
	
    _oomee = Sprite::create("res/childOnboarding/oomee_torso.png");
    _oomee->setAnchorPoint(Vec2(0.5f, 0.55f));
    _oomee->setNormalizedPosition(Vec2(0.5f, 0.0f));
    addChild(_oomee);
    
    _moveVec = Vec2(1 ,0);
    _moveVec.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(10));
    
	return true;
}

void WelcomeScene::onEnter()
{
    scheduleUpdate();
	Super::onEnter();
}

void WelcomeScene::onExit()
{
    unscheduleUpdate();
	Super::onExit();
}

void WelcomeScene::onSizeChanged()
{
	Super::onSizeChanged();
	
	const Size& contentSize = this->getContentSize();
	bool isPortrait = contentSize.height > contentSize.width;
	
    _contentTiles->setPosition(contentSize / 2);
    
    _bottomGradient->setContentSize(Size(contentSize.width + 5, contentSize.height * 0.71f));
	
    _fillColour->setContentSize(Size(contentSize.width + 5, contentSize.height * 0.39f));
    
    _text->setMaxLineWidth(contentSize.width * (isPortrait ? 0.8f : 0.6f));
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 40 + _text->getContentSize().height + _button->getContentSize().height));
	_body->forceDoLayout();
    
    _loginLayout->setContentSize(Size(contentSize.width, _loginButton->getContentSize().height + _loginText->getContentSize().height));
    _loginLayout->forceDoLayout();
    
    _oomee->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2(0.25f, 0.0f));
}

void WelcomeScene::update(float deltaT)
{
    Super::update(deltaT);
    const float pxPerSec = 25.0f;
    _contentTiles->setPosition(_contentTiles->getPosition() + (pxPerSec * _moveVec * deltaT));
    
    const Size& contentSize = getContentSize();
    const Rect& imgBounds = _contentTiles->getBoundingBox();
    
    if(_moveVec.x > 0)
    {
        if(imgBounds.getMinY() > 0 || imgBounds.getMinX() > 0)
        {
            _moveVec = -_moveVec;
        }
    }
    else
    {
        if(imgBounds.getMaxY() < contentSize.height || imgBounds.getMaxX() < contentSize.width)
        {
            _moveVec = -_moveVec;
        }
    }
}

NS_AZ_END
