//
//  CoinCollectLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 04/02/2019.
//

#include "CoinCollectLayer.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool CoinCollectLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
	return true;
}
void CoinCollectLayer::onEnter()
{
	addBackground();
	addHeading();
	addPlinth();
	
	this->scheduleUpdate();
	
	Super::onEnter();
}

void CoinCollectLayer::update(float deltaT)
{
	_nextParticleEmit -= deltaT;
	if(_nextParticleEmit <= 0)
	{
		const Size& visibleSize = this->getContentSize();
		_nextParticleEmit = RandomHelper::random_real(0.5f, 0.75f);
		Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/meteor.png");
		ParticleMeteor* ps = ParticleMeteor::createWithTotalParticles(300);
		ps->setBlendFunc(BlendFunc::ADDITIVE);
		ps->setEndColor(Color4F(0.32, 0.67, 1,0));
		ps->setEndColorVar(Color4F(0.32, 0.67, 1,0.1));
		ps->setStartColor(Color4F(0.32, 0.67, 1,0.3));
		ps->setStartColorVar(Color4F(0.32, 0.67, 1,0.1));
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
							 RandomHelper::random_int(visibleSize.height * 0.3f, visibleSize.height * 1.0f))); //distance Y
	
		this->addChild(ps,-1);
	}
	
	Super::update(deltaT);
}

void CoinCollectLayer::setRewardAmount(int rewardAmount)
{
	_rewardAmount = rewardAmount;
}
void CoinCollectLayer::setAnimDuration(float secs)
{
	_animDuration = secs;
}
void CoinCollectLayer::setOomeeFilepath(const std::string& oomeeFilepath)
{
	_oomeeFilepath = oomeeFilepath;
}

void CoinCollectLayer::addBackground()
{
	LayerColor* bgColour = LayerColor::create(Color4B(0,7,4,34));
	this->addChild(bgColour, -1);
	
	Sprite* bottomGradient = Sprite::create("res/decoration/TopNavGrad.png");
	bottomGradient->setContentSize(Size(this->getContentSize().width, 400));
	bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bottomGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	bottomGradient->setColor(Style::Color::skyBlue);
	bottomGradient->setRotation(180);
	this->addChild(bottomGradient, -1);
	
	ui::Scale9Sprite* wires = ui::Scale9Sprite::create("res/rewards/wires.png");
	wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	wires->setCapInsets(Rect(wires->getContentSize()/2,Size(1,1)));
	wires->setContentSize(this->getContentSize());
	this->addChild(wires, -1);
	
}
void CoinCollectLayer::addHeading()
{
	
}
void CoinCollectLayer::addPlinth()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	Sprite* oomee = Sprite::create("res/defaultOomees/oomee_11.png");
	oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	oomee->setNormalizedPosition(Vec2(0.5,0.8));
	
	Sprite* plinth = Sprite::create("res/rewards/podium.png");
	plinth->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	plinth->setPosition(Vec2(visibleSize.width * (isPortrait ? 0.5 : 0.25), -plinth->getContentSize().height * 1.5));
	plinth->runAction(MoveBy::create(5, Vec2(0,plinth->getContentSize().height * 1.5)));
	plinth->runAction(Repeat::create(Sequence::create(EaseBounceInOut::create(MoveBy::create(0.125, Vec2(3,0))),EaseBounceInOut::create(MoveBy::create(0.25, Vec2(-6,0))), EaseBounceInOut::create(MoveBy::create(0.125, Vec2(3,0))), NULL), 10));
	plinth->addChild(oomee);
	this->addChild(plinth);
	
	Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/smoke.png");
	ParticleMeteor* smoke = ParticleMeteor::createWithTotalParticles(150);
	smoke->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
	smoke->setEndColor(Color4F(0.9, 0.85, 0.85,0));
	smoke->setEndColorVar(Color4F(0,0,0,0.1));
	smoke->setStartColor(Color4F(1.0, 0.95, 0.95,0.3));
	smoke->setStartColorVar(Color4F(0,0,0,0.1));
	smoke->setTexture(particleTex);
	smoke->setLife(3.0);
	smoke->setLifeVar(0.5);
	smoke->setGravity(Vec2(0,-25));
	smoke->setStartSize(100);
	smoke->setStartSizeVar(25);
	smoke->setEndSize(125);
	smoke->setEndSizeVar(25);
	smoke->setPosVar(Vec2(plinth->getContentSize().width / 2,20));
	smoke->setAngleVar(30);
	smoke->setAngle(90);
	smoke->setEmissionRate(75);
	smoke->setDuration(5);
	smoke->setSpeed(75);
	smoke->setSpeedVar(25);
	smoke->setAutoRemoveOnFinish(true);
	smoke->setPosition(Vec2(plinth->getPositionX(), 0));
	this->addChild(smoke);
}
void CoinCollectLayer::addCoinCounter()
{
	
}
NS_AZOOMEE_END
