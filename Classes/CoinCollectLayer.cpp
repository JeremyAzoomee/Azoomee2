//
//  CoinCollectLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 04/02/2019.
//

#include "CoinCollectLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool CoinCollectLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	addBackground();
	return true;
}
void CoinCollectLayer::onEnter()
{
	_rewardAmount = _rewardData ? abs(_rewardData->getItemPrice()) : 0;
	
	addHeading();
	addPlinth();
	
	this->scheduleUpdate();
	
	_passingTouchBlocker = EventListenerTouchOneByOne::create();
	_passingTouchBlocker->setSwallowTouches(true);
	_passingTouchBlocker->onTouchBegan = [](Touch* touch, Event* event){ return true; };
	_passingTouchBlocker->onTouchEnded = [this](Touch* touch, Event* event){
		// Do your stuff here
	};
	_eventDispatcher->addEventListenerWithFixedPriority(_passingTouchBlocker, -1); // less than zero
	
	Super::onEnter();
}

void CoinCollectLayer::onExit()
{
	_eventDispatcher->removeEventListener(_passingTouchBlocker);
	_passingTouchBlocker = nullptr;
	unscheduleUpdate();
	Super::onExit();
}

void CoinCollectLayer::update(float deltaT)
{
	_nextParticleEmit -= deltaT;
	if(_nextParticleEmit <= 0)
	{
		const Size& visibleSize = this->getContentSize();
		_nextParticleEmit = RandomHelper::random_real(0.5f, 0.75f);
		ParticleSystemQuad* ps = createMeteorParticles();
		
		const Vec2& vel = Vec2(RandomHelper::random_int((int)(-MAX(visibleSize.height,visibleSize.width) * 1.3f), (int)(-MAX(visibleSize.height,visibleSize.width) * 1.1f)), //distance X
							   RandomHelper::random_int((int)(-MIN(visibleSize.height,visibleSize.width) * 1.0f), (int)(-MIN(visibleSize.height,visibleSize.width) * 0.4f))); //distance Y
		ps->setSpeed(-vel.length() / 5);
		ps->setAngle(CC_RADIANS_TO_DEGREES(vel.getAngle()));
		float duration = RandomHelper::random_real(3.0, 4.0);
		ps->setDuration(duration);
		ps->runAction(EaseOut::create(MoveBy::create(duration, vel), 0.3f));
		ps->setPosition(Vec2(RandomHelper::random_int((int)(visibleSize.width * 1.1f), (int)(visibleSize.width * 1.3f)), //distance X
							 RandomHelper::random_int((int)(visibleSize.height * 0.6f), (int)(visibleSize.height * 1.2f)))); //distance Y
	
		this->addChild(ps,-1);
	}
	
	if(_valueLabel)
	{
		if(_displayValue < _rewardAmount)
		{
			_displayValue += _incPerSec * deltaT;
			_valueLabel->setString(StringUtils::format("%d",MIN((int)_displayValue,_rewardAmount)));
		}
	}
	
	Super::update(deltaT);
}

void CoinCollectLayer::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	auto children = this->getChildren();
	
	for(auto child : children)
	{
		if(child->getName() == "coin")
		{
			child->stopAllActions();
			child->removeFromParent();
		}
	}
	
	if(_bgColour)
	{
		_bgColour->setContentSize(visibleSize);
	}
	if(_bottomGradient)
	{
		_bottomGradient->setContentSize(Size(visibleSize.width, 400));
	}
	if(_wires)
	{
		_wires->setContentSize(visibleSize);
	}
	
	if(_plinth)
	{
		_plinth->stopAllActions();
		_plinth->setPosition(Vec2(visibleSize.width * (isPortrait ? 0.5 : 0.25), 0));
		if(!_mainCoin)
		{
			addCoinCounter();
		}
	}
	
	if(_smoke)
	{
		_smoke->setPosition(_plinth->getPositionX(), 0);
	}
	
	if(_mainCoin)
	{
		const Vec2& oomeeWorldPos = _plinth->getPosition() + Vec2(0,_plinth->getContentSize().height + 200);
		_mainCoin->setPosition(isPortrait ? Vec2(visibleSize.width * 0.5f,visibleSize.height * 0.5f) : Vec2(visibleSize.width * 0.75f, oomeeWorldPos.y));
	}
	
	if(_counterFrame)
	{
		_counterFrame->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_BOTTOM);
		_counterFrame->setAnchorPoint(isPortrait ? Vec2(0.5,-0.5) : Vec2(0.5,1.5));
	}
	
	
}

void CoinCollectLayer::setOomeeFilepath(const std::string& oomeeFilepath)
{
	_oomeeFilepath = oomeeFilepath;
}

void CoinCollectLayer::addBackground()
{
	_bgColour = LayerColor::create(Color4B(0,7,4,255));
	this->addChild(_bgColour, -1);
	
	_bottomGradient = Sprite::create("res/decoration/TopNavGrad.png");
	_bottomGradient->setContentSize(Size(this->getContentSize().width, 400));
	_bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bottomGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_bottomGradient->setColor(Style::Color::skyBlue);
	_bottomGradient->setRotation(180);
	this->addChild(_bottomGradient, -1);
	
	_wires = ui::Scale9Sprite::create("res/rewards/wires.png");
	_wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_wires->setCapInsets(Rect(_wires->getContentSize()/2,Size(1,1)));
	_wires->setContentSize(this->getContentSize());
	this->addChild(_wires, -1);
	
}
void CoinCollectLayer::addHeading()
{
	const Size& visibleSize = this->getContentSize();
	
	ParticleSystemQuad* sparkle = createSparkleParticles(Vec2(visibleSize.width * 0.6f, visibleSize.height * 0.25f));
	sparkle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	sparkle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	this->addChild(sparkle);
	
	Label* heading = Label::createWithTTF("GOOD JOB!", Style::Font::Bold(), 200);
	heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	heading->setNormalizedPosition(Vec2(0.5,0.9));
	heading->enableGlow(Color4B::WHITE);
	heading->setScale(0.5);
	heading->runAction(EaseBackOut::create(ScaleTo::create(_duration * 0.5f, 1)));
	this->addChild(heading);
}
void CoinCollectLayer::addPlinth()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	RemoteImageSprite* oomee = RemoteImageSprite::create();
	oomee->initWithUrlAndSizeWithoutPlaceholder(_oomeeFilepath, Size(600,600));
	oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	oomee->setNormalizedPosition(Vec2(0.5,0.8));
	
	const float animDur = _duration * 0.25f;
	
	_plinth = Sprite::create("res/rewards/podium.png");
	_plinth->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_plinth->setPosition(Vec2(visibleSize.width * (isPortrait ? 0.5 : 0.25), -_plinth->getContentSize().height * 1.5));
	_plinth->runAction(Sequence::createWithTwoActions(MoveBy::create(animDur, Vec2(0,_plinth->getContentSize().height * 1.5)),CallFunc::create([this](){this->addCoinCounter();})));
	_plinth->runAction(Repeat::create(Sequence::create(EaseBounceInOut::create(MoveBy::create(0.125, Vec2(5,0))),EaseBounceInOut::create(MoveBy::create(0.25, Vec2(-10,0))), EaseBounceInOut::create(MoveBy::create(0.125, Vec2(5,0))), NULL), ceil(animDur)));
	_plinth->addChild(oomee);
	this->addChild(_plinth);
	
	_smoke = createSmokeParticles(Vec2(_plinth->getContentSize().width / 1.5,50));
	_smoke->setPosition(Vec2(_plinth->getPositionX(), 0));
	this->addChild(_smoke);
}
void CoinCollectLayer::addCoinCounter()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	const Vec2& oomeeWorldPos = _plinth->getPosition() + Vec2(0,_plinth->getContentSize().height + 200);
	
	_mainCoin = Sprite::create("res/rewards/coin.png");
	_mainCoin->setPosition(isPortrait ? Vec2(visibleSize.width * 0.5f,visibleSize.height * 0.5f) : Vec2(visibleSize.width * 0.75f, oomeeWorldPos.y));
	_mainCoin->setContentSize(_mainCoin->getContentSize() * 2.5f);
	this->addChild(_mainCoin,1);
	
	Sprite* coinGlow = Sprite::create("res/rewards/coin_glow.png");
	coinGlow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_mainCoin->addChild(coinGlow,-1);
	
	_counterFrame = Sprite::create("res/rewards/value_frame.png");
	_counterFrame->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_BOTTOM);
	_counterFrame->setAnchorPoint(isPortrait ? Vec2(0.5,-0.5) : Vec2(0.5,1.5));
	_mainCoin->addChild(_counterFrame);
	
	_valueLabel = Label::createWithTTF("0", Style::Font::Bold(), _counterFrame->getContentSize().height * 0.8f);
	_valueLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_valueLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_valueLabel->setVerticalAlignment(TextVAlignment::CENTER);
	_valueLabel->setOverflow(Label::Overflow::SHRINK);
	_valueLabel->setDimensions(_counterFrame->getContentSize().width * 0.8f, _counterFrame->getContentSize().height * 0.8f);
	_counterFrame->addChild(_valueLabel);
	
	const float animDur = _duration * 0.65f;
	
	_incPerSec = _rewardAmount / animDur;
	_displayValue = 0;
	
	const float delays[3] = {0.0f,0.33f,0.66f};
	
	for(int i = 0; i < animDur - 1; i++)
	{
		for(float delay : delays)
		{
			Sprite* coin = createCoinWithDelay((float)i + delay);
			coin->setPosition(oomeeWorldPos);
			this->addChild(coin);
		}
	}
}

cocos2d::ParticleSystemQuad* CoinCollectLayer::createSparkleParticles(const Vec2& emissionArea)
{
	Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/star.png");
	ParticleSystemQuad* sparkle = ParticleSystemQuad::createWithTotalParticles(30);
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
	sparkle->setPosVar(emissionArea);
	sparkle->setAngleVar(0);
	sparkle->setAngle(0);
	sparkle->setEmissionRate(10);
	sparkle->setDuration(-1);
	sparkle->setSpeed(0);
	sparkle->setSpeedVar(0);
	sparkle->setStartSpin(0);
	sparkle->setStartSpinVar(50);
	sparkle->setEndSpin(0);
	sparkle->setEndSpinVar(50);
	sparkle->setAutoRemoveOnFinish(true);
	return sparkle;
}
cocos2d::ParticleSystemQuad* CoinCollectLayer::createSmokeParticles(const Vec2& emissionArea)
{
	Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/smoke.png");
	ParticleSystemQuad* smoke = ParticleMeteor::createWithTotalParticles(150);
	smoke->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
	smoke->setEndColor(Color4F(0.9, 0.85, 0.85,0));
	smoke->setEndColorVar(Color4F(0,0,0,0.1));
	smoke->setStartColor(Color4F(1.0, 0.95, 0.95,0.3));
	smoke->setStartColorVar(Color4F(0,0,0,0.1));
	smoke->setTexture(particleTex);
	smoke->setLife(2.5);
	smoke->setLifeVar(0.5);
	smoke->setGravity(Vec2(0,-25));
	smoke->setStartSize(150);
	smoke->setStartSizeVar(25);
	smoke->setEndSize(175);
	smoke->setEndSizeVar(25);
	smoke->setPosVar(emissionArea);
	smoke->setAngleVar(20);
	smoke->setAngle(90);
	smoke->setEmissionRate(75);
	smoke->setDuration(2);
	smoke->setSpeed(90);
	smoke->setSpeedVar(25);
	return smoke;
}
cocos2d::ParticleSystemQuad* CoinCollectLayer::createMeteorParticles()
{
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
	return ps;
}

Sprite* CoinCollectLayer::createCoinWithDelay(float delay)
{
	Sprite* coin = Sprite::create("res/rewards/coin.png");
	coin->setScale(0);
	coin->runAction(Sequence::createWithTwoActions(DelayTime::create(delay),MoveTo::create(1, _mainCoin->getPosition())));
	coin->runAction(Sequence::createWithTwoActions(DelayTime::create(delay), ScaleTo::create(1, 2.5)));
	coin->setName("coin");
	return coin;
}

NS_AZOOMEE_END
