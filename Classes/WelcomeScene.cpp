//
//  WelcomeScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/01/2019.
//

#include "WelcomeScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "SceneManagerScene.h"
#include "BackEndCaller.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool WelcomeScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = this->getContentSize();
	bool isPortrait = contentSize.height > contentSize.width;
	
	_bgColour = LayerColor::create(Color4B(0,7,4,255), contentSize.width, contentSize.height);
	this->addChild(_bgColour);
	
	_wires = Sprite::create("res/rewards/big_wires.png");
	_wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_wires->setScale(MAX(contentSize.width, contentSize.height) / _wires->getContentSize().width);
	_wires->setRotation(isPortrait ? 90 : 0);
	_wires->setOpacity(65);
	this->addChild(_wires);
	
	_bottomGradient = Sprite::create("res/decoration/TopNavGrad.png");
	_bottomGradient->setContentSize(Size(contentSize.width, 400));
	_bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bottomGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_bottomGradient->setColor(Style::Color::skyBlue);
	_bottomGradient->setRotation(180);
	this->addChild(_bottomGradient);
	
	_contentTiles = Sprite::create("res/introAssets/content_tiles_complete.png");
	_contentTiles->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_contentTiles->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_contentTiles->setScale(contentSize.width / _contentTiles->getContentSize().width);
	this->addChild(_contentTiles);
	
	_body = ui::Layout::create();
	_body->setLayoutType(ui::Layout::Type::VERTICAL);
	_body->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_body->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	this->addChild(_body);
	
	_logo = ui::ImageView::create("res/introAssets/Azoomee_WHITE.png");
	_logo->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_body->addChild(_logo);
	
	_textHolder = ui::Layout::create();
	_textHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	_body->addChild(_textHolder);
	
	_text = Label::createWithTTF(_("introText"), Style::Font::Regular(), 90);
	_text->setWidth(contentSize.width * 0.6f);
	_text->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	_text->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	_text->setTextColor(Color4B::WHITE);
	_text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_textHolder->setContentSize(_text->getContentSize());
	_textHolder->addChild(_text);
	
	_button = ui::Button::create("res/buttons/blue_cta.png");
	_button->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_button->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			BackEndCaller::getInstance()->anonymousDeviceLogin();
		}
	});
	_body->addChild(_button);
	
	Label* buttonText = Label::createWithTTF(_("introButton"), Style::Font::Regular(), 80);
	buttonText->setTextColor(Color4B::WHITE);
	buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonText->setNormalizedPosition(Vec2(0.4f,0.5f));
	buttonText->setOverflow(Label::Overflow::SHRINK);
	buttonText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	buttonText->setDimensions(_button->getContentSize().width * 0.66, _button->getContentSize().height * 0.6f);
	_button->addChild(buttonText);
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 50 + _text->getContentSize().height + _button->getContentSize().height));
	
	ui::Button* loginButton = ui::Button::create("res/settings/rounded_button.png");
	loginButton->setAnchorPoint(Vec2(1.15, 1.75f));
	loginButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	loginButton->setColor(Style::Color::skyBlue);
	loginButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
		}
	});
	
	this->addChild(loginButton);
	
	Label* loginText = Label::createWithTTF(_("Log in"), Style::Font::Regular(), 60);
	loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	loginText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	loginText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	loginText->setOverflow(Label::Overflow::SHRINK);
	loginText->setDimensions(loginButton->getContentSize().width * 0.7f, loginButton->getContentSize().height * 0.7f);
	loginText->setTextColor(Color4B::WHITE);
	loginButton->addChild(loginText);
	
	return true;
}

void WelcomeScene::onEnter()
{
	Super::onEnter();
}

void WelcomeScene::onExit()
{
	Super::onExit();
}

void WelcomeScene::onSizeChanged()
{
	Super::onSizeChanged();
	
	const Size& contentSize = this->getContentSize();
	bool isPortrait = contentSize.height > contentSize.width;
	
	_bgColour->setContentSize(contentSize);
	
	_wires->setScale(MAX(contentSize.width, contentSize.height) / _wires->getContentSize().width);
	_wires->setRotation(isPortrait ? 90 : 0);
	
	_text->setWidth(contentSize.width * 0.6f);
	
	_bottomGradient->setContentSize(Size(contentSize.width, 400));
	
	_contentTiles->setScale(contentSize.width / _contentTiles->getContentSize().width);
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 50 + _text->getContentSize().height + _button->getContentSize().height));
}

NS_AZOOMEE_END
