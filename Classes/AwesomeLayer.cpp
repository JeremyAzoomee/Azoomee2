//
//  AwesomeLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/02/2019.
//

#include "AwesomeLayer.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::vector<cocos2d::Color4F> AwesomeLayer::kColours = {cocos2d::Color4F(1, 0.81, 0.06, 1), cocos2d::Color4F(1, 0.43, 0.01, 1), cocos2d::Color4F(0.42, 0.22, 0.73, 1), cocos2d::Color4F(0.13, 0.58, 0.87, 1)};

bool AwesomeLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}
void AwesomeLayer::onEnter()
{
	LayerColor* bgColour = LayerColor::create(Color4B(0,7,4,255));
	this->addChild(bgColour, -1);
	
	addStars();
	addAwsomeText();
	
	this->scheduleUpdate();
	
	Super::onEnter();
}

void AwesomeLayer::update(float deltaT)
{
	_nextParticleEmit -= deltaT;
	if(_nextParticleEmit <= 0)
	{
		const Color4F& colour = kColours.at(RandomHelper::random_int(0, int(kColours.size() - 1)));
		Color4F startColour = colour;
		startColour.a = 0.3f;
		Color4F endColour = colour;
		endColour.a = 0.0f;
		
		const Size& visibleSize = this->getContentSize();
		_nextParticleEmit = RandomHelper::random_real(0.25f, 0.5f);
		Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/meteor.png");
		ParticleMeteor* ps = ParticleMeteor::createWithTotalParticles(300);
		ps->setBlendFunc(BlendFunc::ADDITIVE);
		ps->setEndColor(endColour);
		ps->setEndColorVar(Color4F(0, 0, 0,0.1));
		ps->setStartColor(startColour);
		ps->setStartColorVar(Color4F(0, 0, 0,0.1));
		ps->setTexture(particleTex);
		ps->setLife(0.4);
		ps->setLifeVar(0.05);
		ps->setGravity(Vec2(0,0));
		ps->setStartSize(65);
		ps->setStartSizeVar(15);
		ps->setEndSize(40);
		ps->setEndSizeVar(5);
		ps->setPosVar(Vec2(5,5));
		ps->setAngleVar(0);
		ps->setEmissionRate(300);
		ps->setAutoRemoveOnFinish(true);
		ps->setPositionType(ParticleSystem::PositionType::GROUPED);
		
		const Vec2& vel = Vec2(RandomHelper::random_int(-MAX(visibleSize.height,visibleSize.width) * 1.3f, -MAX(visibleSize.height,visibleSize.width) * 1.1f), //distance X
							   RandomHelper::random_int(-MIN(visibleSize.height,visibleSize.width) * 1.0f, -MIN(visibleSize.height,visibleSize.width) * 0.4f)); //distance Y
		ps->setSpeed(-vel.length() / 5);
		ps->setAngle(CC_RADIANS_TO_DEGREES(vel.getAngle()));
		float duration = RandomHelper::random_real(3.0, 4.0);
		ps->setDuration(duration);
		ps->runAction(EaseOut::create(MoveBy::create(duration, vel), 0.3f));
		ps->setPosition(Vec2(RandomHelper::random_int(visibleSize.width * 1.1f, visibleSize.width * 1.3f), //distance X
							 RandomHelper::random_int(visibleSize.height * 0.7f, visibleSize.height * 1.3f))); //distance Y
		
		this->addChild(ps,-1);
	}
	
	Super::update(deltaT);
}

void AwesomeLayer::addAwsomeText()
{
	Sprite* bgLights = Sprite::create("res/rewards/wow_bg.png");
	bgLights->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	bgLights->setScale(1.8f);
	bgLights->runAction(RepeatForever::create(RotateBy::create(1.0,90)));
	bgLights->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.45), ScaleTo::create(2, 2.15), ScaleTo::create(1, 1), NULL)));
	this->addChild(bgLights);
	
	Label* text = Label::createWithTTF("AWESOME", Style::Font::Bold(), 200);
	text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	text->setScale(0.5);
	text->runAction(EaseBackOut::create(ScaleTo::create(_duration * 0.5f, 1)));
	this->addChild(text);
}
void AwesomeLayer::addStars()
{
	const Size& visibleSize = this->getContentSize();
	
	Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/star.png");
	
	for(const Color4F& colour : kColours)
	{
		Color4F endColour = colour;
		endColour.a = 0.1f;
		ParticleSystemQuad* sparkle = ParticleSystemQuad::createWithTotalParticles(15);
		sparkle->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
		sparkle->setEndColor(endColour);
		sparkle->setEndColorVar(Color4F(0,0,0,0.1));
		sparkle->setStartColor(colour);
		sparkle->setStartColorVar(Color4F(0,0,0,0.1));
		sparkle->setTexture(particleTex);
		sparkle->setLife(3.0);
		sparkle->setLifeVar(1.0);
		sparkle->setGravity(Vec2(0,0));
		sparkle->setStartSize(50);
		sparkle->setStartSizeVar(50);
		sparkle->setEndSize(175);
		sparkle->setEndSizeVar(100);
		sparkle->setPosVar(visibleSize);
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
		sparkle->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height));
		sparkle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		this->addChild(sparkle);
	}
}

NS_AZOOMEE_END
