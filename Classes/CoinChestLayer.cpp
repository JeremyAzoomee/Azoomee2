//
//  CoinChestLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/02/2019.
//

#include "CoinChestLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool CoinChestLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
	return true;
}
void CoinChestLayer::onEnter()
{
	ui::Scale9Sprite* wires = ui::Scale9Sprite::create("res/rewards/wires.png");
	wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	wires->setCapInsets(Rect(wires->getContentSize()/2,Size(1,1)));
	wires->setContentSize(this->getContentSize());
	this->addChild(wires, -1);
	
	addText();
	addChest();
	
	this->scheduleUpdate();
	
	Super::onEnter();
}

void CoinChestLayer::update(float deltaT)
{
	Super::update(deltaT);
}

void CoinChestLayer::setRewardValue(int rewardValue)
{
	_rewardValue = rewardValue;
}

void CoinChestLayer::addText()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	ui::Layout* text = ui::Layout::create();
	text->setLayoutType(ui::Layout::Type::VERTICAL);
	
	ui::Text* youWon = ui::Text::create("YOU WON", Style::Font::Regular(), 120);
	youWon->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	text->addChild(youWon);
	
	ui::Text* value = ui::Text::create(StringUtils::format("%d",_rewardValue), Style::Font::Bold(), 360);
	value->setTextColor(Color4B(Style::Color::macaroniAndCheese));
	value->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	text->addChild(value);
	
	ui::Text* coins = ui::Text::create("COINS", Style::Font::Regular(), 120);
	coins->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	text->addChild(coins);
	
	text->setContentSize(Size(value->getContentSize().width, youWon->getContentSize().height + value->getContentSize().height + coins->getContentSize().height));
	text->setNormalizedPosition(isPortrait ? Vec2(0.5,0.75) : Vec2(0.70,0.5));
	text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(text);
}
void CoinChestLayer::addChest()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	Sprite* bgLights = Sprite::create("res/rewards/wow_bg.png");
	bgLights->setNormalizedPosition(isPortrait ? Vec2(0.5,0.33) : Vec2(0.33,0.5));
	bgLights->runAction(RepeatForever::create(RotateBy::create(1.0,90)));
	bgLights->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 0.85), ScaleTo::create(2, 1.15), ScaleTo::create(1, 1), NULL)));
	this->addChild(bgLights);
	
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
	sparkle->setPosition(isPortrait ? Vec2(visibleSize.width * 0.5, visibleSize.height * 0.33) : Vec2(visibleSize.width * 0.33, visibleSize.height * 0.5));
	sparkle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	this->addChild(sparkle);
	
	Sprite* chest = Sprite::create("res/rewards/chest_of_coins.png");
	chest->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	chest->setNormalizedPosition(isPortrait ? Vec2(0.5,0.33) : Vec2(0.33,0.5));
	chest->setScale(0);
	chest->runAction(EaseBackOut::create(ScaleTo::create(2, 1)));
	this->addChild(chest);
}

NS_AZOOMEE_END
