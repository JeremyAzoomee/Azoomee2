//
//  CoinChestLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/02/2019.
//

#include "CoinChestLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool CoinChestLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = getContentSize();
	bool isPortrait = contentSize.width < contentSize.height;
	
	_bgColour = LayerColor::create(Color4B(0,7,4,255));
	this->addChild(_bgColour, -1);
	
	_wires = Sprite::create("res/rewards/big_wires.png");
	_wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_wires->setScale(MAX(contentSize.width, contentSize.height) / _wires->getContentSize().width);
	_wires->setRotation(isPortrait ? 90 : 0);
	this->addChild(_wires, -1);
	
	_wireGlow = Sprite::create("res/rewards/big_wires_glow.png");
	_wireGlow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_wireGlow->setScale(MAX(contentSize.width, contentSize.height) / _wireGlow->getContentSize().width);
	_wireGlow->setRotation(isPortrait ? 90 : 0);
	_wireGlow->setOpacity(0);
	this->addChild(_wireGlow, -1);
	
	_backButton = ui::Button::create("res/rewards/close_white.png");
	_backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_backButton->setAnchorPoint(Vec2(1.25,1.25));
	this->addChild(_backButton,1);
	
	_useButton = ui::Button::create("res/shop/cta.png");
	_useButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_useButton->setNormalizedPosition(isPortrait ? Vec2(0.5,0.1) : Vec2(0.7,0.3));
	_useButton->setScale9Enabled(true);
	this->addChild(_useButton);
	
	Label* useLabel = Label::createWithTTF(_("GO TO SHOP"), Style::Font::Bold(), 75);
	useLabel->setTextColor(Color4B(0, 245, 246, 255));
	useLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	useLabel->setVerticalAlignment(TextVAlignment::CENTER);
	useLabel->setOverflow(Label::Overflow::SHRINK);
	useLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	useLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_useButton->addChild(useLabel);
	_useButton->setContentSize(Size(useLabel->getContentSize().width + 150, _useButton->getContentSize().height));
	useLabel->setDimensions(_useButton->getContentSize().width - 150, _useButton->getContentSize().height * 0.8f);
	
	return true;
}
void CoinChestLayer::onEnter()
{
	_passingTouchBlocker->onTouchEnded = [this](Touch* touch, Event* event){
		// Do your stuff here
		const Point& pos = touch->getLocation();
		const Point& transformedPoint = Point(pos.x - Director::getInstance()->getVisibleOrigin().x, pos.y - Director::getInstance()->getVisibleOrigin().y ); //tranform relative to origin as this inst included because of the notification node
		if(_backButton->getBoundingBox().containsPoint(transformedPoint))
		{
			if(this->_delegate)
			{
				this->_delegate->onAnimationComplete(_rewardData);
			}
		}
		else if(_useButton->getBoundingBox().containsPoint(transformedPoint))
		{
			if(this->_delegate)
			{
				this->_delegate->onAnimationComplete(_rewardData);
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Shop));
			}
		}
	};
	
	addChest();
	addText();
	
	_wires->runAction(FadeOut::create(_duration));
	_wireGlow->runAction(FadeIn::create(_duration));
	
	AudioMixer::getInstance()->playEffect("Rewards_Anim_ChestCoinsScreen.wav");
	
	Super::onEnter();
	stopActionByTag(kAutoCallbackActionTag);
}

void CoinChestLayer::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	if(_text)
	{
		_text->setNormalizedPosition(isPortrait ? Vec2(0.5,0.75) : Vec2(0.70,0.6));
	}
	if(_chestNode)
	{
		_chestNode->setNormalizedPosition(isPortrait ? Vec2(0.5,0.4) : Vec2(0.33,0.5));
	}
	if(_bgColour)
	{
		_bgColour->setContentSize(visibleSize);
	}
	if(_wires)
	{
		_wires->setScale(MAX(visibleSize.width, visibleSize.height) / _wires->getContentSize().width);
		_wires->setRotation(isPortrait ? 90 : 0);
		_wireGlow->setScale(MAX(visibleSize.width, visibleSize.height) / _wires->getContentSize().width);
		_wireGlow->setRotation(isPortrait ? 90 : 0);
	}
	if(_useButton)
	{
		_useButton->setNormalizedPosition(isPortrait ? Vec2(0.5,0.1) : Vec2(0.7,0.3));
	}
}

void CoinChestLayer::addText()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	_text = ui::Layout::create();
	_text->setLayoutType(ui::Layout::Type::VERTICAL);
	
	ui::Text* youWon = ui::Text::create(_("YOU WON"), Style::Font::PassionOneRegular, 120);
	youWon->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_text->addChild(youWon);
	
	ui::Text* value = ui::Text::create(StringUtils::format("%d",abs(_rewardData->getItemPrice())), Style::Font::PassionOneRegular, 360);
	value->setTextColor(Color4B(Style::Color::macaroniAndCheese));
	value->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_text->addChild(value);
	
	ui::Text* coins = ui::Text::create(_("COINS"), Style::Font::PassionOneRegular, 120);
	coins->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_text->addChild(coins);
	
	_text->setContentSize(Size(value->getContentSize().width, youWon->getContentSize().height + value->getContentSize().height + coins->getContentSize().height));
	_text->setNormalizedPosition(isPortrait ? Vec2(0.5,0.75) : Vec2(0.70,0.6));
	_text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_text);
}
void CoinChestLayer::addChest()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	_chestNode = Node::create();
	_chestNode->setNormalizedPosition(isPortrait ? Vec2(0.5,0.33) : Vec2(0.33,0.5));
	this->addChild(_chestNode);
	
	Sprite* bgLights = Sprite::create("res/rewards/wow_bg.png");
	bgLights->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	bgLights->setScale(1.8f);
	bgLights->runAction(RepeatForever::create(RotateBy::create(1.0,90)));
	bgLights->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.45), ScaleTo::create(2, 2.15), ScaleTo::create(1, 1), NULL)));
	_chestNode->addChild(bgLights);
	
	Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/star.png");
	ParticleSystemQuad* sparkle = ParticleSystemQuad::createWithTotalParticles(15);
	sparkle->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
	sparkle->setEndColor(Color4F(1, 0.81, 0.06,0.1));
	sparkle->setEndColorVar(Color4F(0,0,0,0.1));
	sparkle->setStartColor(Color4F(1, 0.81, 0.06,1));
	sparkle->setStartColorVar(Color4F(0,0,0,0.1));
	sparkle->setTexture(particleTex);
	sparkle->setLife(3.0);
	sparkle->setLifeVar(1.0);
	sparkle->setGravity(Vec2(0,0));
	sparkle->setStartSize(75);
	sparkle->setStartSizeVar(25);
	sparkle->setEndSize(225);
	sparkle->setEndSizeVar(50);
	sparkle->setPosVar(Vec2(visibleSize.width * 0.25f, visibleSize.height * 0.25));
	sparkle->setAngleVar(0);
	sparkle->setAngle(0);
	sparkle->setEmissionRate(8);
	sparkle->setDuration(-1);
	sparkle->setSpeed(0);
	sparkle->setSpeedVar(0);
	sparkle->setStartSpin(0);
	sparkle->setStartSpinVar(50);
	sparkle->setEndSpin(0);
	sparkle->setEndSpinVar(50);
	sparkle->setAutoRemoveOnFinish(true);
	sparkle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	sparkle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_chestNode->addChild(sparkle);
	
	Sprite* chest = Sprite::create("res/rewards/chest_of_coins.png");
	chest->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	chest->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	chest->setScale(0.5);
	chest->runAction(EaseBackOut::create(ScaleTo::create(_duration * 0.5f, 1)));
	_chestNode->addChild(chest);
}

NS_AZOOMEE_END
