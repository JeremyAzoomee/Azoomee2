//
//  CoinChestLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/02/2019.
//

#include "CoinChestLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool CoinChestLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
    _bgColour = LayerColor::create(Color4B(Style::Color::darkIndigo));
	this->addChild(_bgColour, -1);
	
    _pattern = RoundedRectSprite::create();
    _pattern->setTexture("res/decoration/pattern_stem_tile.png");
    _pattern->setCornerRadius(0);
    _pattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _pattern->setColor(Style::Color::white);
    _pattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _pattern->setContentSize(getContentSize());
    _pattern->setOpacity(100);
    addChild(_pattern);
	
	return true;
}
void CoinChestLayer::onEnter()
{
	addChest();
	addText();
	
	AudioMixer::getInstance()->playEffect("Blip.mp3");
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CallFunc::create([](){
		AudioMixer::getInstance()->playEffect("Big Win.mp3");
	})));
	
	
	Super::onEnter();
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
	if(_pattern)
    {
        _pattern->setContentSize(visibleSize);
    }
}

void CoinChestLayer::addText()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	_text = ui::Layout::create();
	_text->setLayoutType(ui::Layout::Type::VERTICAL);
	
	ui::Text* youWon = ui::Text::create(_("YOU WON"), Style::Font::RewardRegular(), 120);
	youWon->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_text->addChild(youWon);
	
	ui::Text* value = ui::Text::create(StringUtils::format("%d",abs(_rewardData->getItemPrice())), Style::Font::RewardRegular(), 360);
	value->setTextColor(Color4B(Style::Color::macaroniAndCheese));
	value->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_text->addChild(value);
	
	ui::Text* coins = ui::Text::create(_("COINS"), Style::Font::RewardRegular(), 120);
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
