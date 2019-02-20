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
	
	return true;
}
void CoinCollectLayer::onEnter()
{
	_rewardAmount = _rewardData ? abs(_rewardData->getItemPrice()) : 0;
	
	addBackground();
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
	Super::onExit();
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
							 RandomHelper::random_int(visibleSize.height * 0.6f, visibleSize.height * 1.2f))); //distance Y
	
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

void CoinCollectLayer::setOomeeFilepath(const std::string& oomeeFilepath)
{
	_oomeeFilepath = oomeeFilepath;
}

void CoinCollectLayer::addBackground()
{
	LayerColor* bgColour = LayerColor::create(Color4B(0,7,4,255));
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
	const Size& visibleSize = this->getContentSize();
	//bool isPortrait = visibleSize.width < visibleSize.height;
	
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
	sparkle->setPosVar(Vec2(visibleSize.width * 0.6f, visibleSize.height * 0.25f));
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
	sparkle->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height));
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
	
	Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/smoke.png");
	ParticleMeteor* smoke = ParticleMeteor::createWithTotalParticles(150);
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
	smoke->setPosVar(Vec2(_plinth->getContentSize().width / 1.5,50));
	smoke->setAngleVar(20);
	smoke->setAngle(90);
	smoke->setEmissionRate(75);
	smoke->setDuration(2);
	smoke->setSpeed(90);
	smoke->setSpeedVar(25);
	smoke->setAutoRemoveOnFinish(true);
	smoke->setPosition(Vec2(_plinth->getPositionX(), 0));
	this->addChild(smoke);
}
void CoinCollectLayer::addCoinCounter()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	
	const Vec2& oomeeWorldPos = _plinth->getPosition() + Vec2(0,_plinth->getContentSize().height + 200);
	
	Sprite* mainCoin = Sprite::create("res/rewards/coin.png");
	mainCoin->setPosition(isPortrait ? Vec2(visibleSize.width * 0.5f,visibleSize.height * 0.5f) : Vec2(visibleSize.width * 0.75f, oomeeWorldPos.y));
	mainCoin->setContentSize(mainCoin->getContentSize() * 2.5f);
	this->addChild(mainCoin,1);
	
	Sprite* coinGlow = Sprite::create("res/rewards/coin_glow.png");
	coinGlow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	mainCoin->addChild(coinGlow,-1);
	
	Sprite* counterFrame = Sprite::create("res/rewards/value_frame.png");
	counterFrame->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_BOTTOM);
	counterFrame->setAnchorPoint(isPortrait ? Vec2(0.5,-0.5) : Vec2(0.5,1.5));
	mainCoin->addChild(counterFrame);
	
	_valueLabel = Label::createWithTTF("0", Style::Font::Bold(), counterFrame->getContentSize().height * 0.8f);
	_valueLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_valueLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_valueLabel->setVerticalAlignment(TextVAlignment::CENTER);
	_valueLabel->setOverflow(Label::Overflow::SHRINK);
	_valueLabel->setDimensions(counterFrame->getContentSize().width * 0.8f, counterFrame->getContentSize().height * 0.8f);
	counterFrame->addChild(_valueLabel);
	
	const float animDur = _duration * 0.65f;
	
	_incPerSec = _rewardAmount / animDur;
	_displayValue = 0;
	
	for(int i = 0; i < animDur - 1; i++)
	{
		Sprite* coin1 = Sprite::create("res/rewards/coin.png");
		coin1->setScale(0);
		coin1->setPosition(oomeeWorldPos);
		coin1->runAction(Sequence::createWithTwoActions(DelayTime::create(i),MoveTo::create(1, mainCoin->getPosition())));
		coin1->runAction(Sequence::createWithTwoActions(DelayTime::create(i), ScaleTo::create(1, 2.5)));
		this->addChild(coin1);
		
		if(i + 1.33 < animDur)
		{
			Sprite* coin2 = Sprite::create("res/rewards/coin.png");
			coin2->setScale(0);
			coin2->setPosition(oomeeWorldPos);
			coin2->runAction(Sequence::createWithTwoActions(DelayTime::create(i + 0.33),MoveTo::create(1, mainCoin->getPosition())));
			coin2->runAction(Sequence::createWithTwoActions(DelayTime::create(i + 0.33), ScaleTo::create(1, 2.5)));
			this->addChild(coin2);
		}
		if(i + 1.66 < animDur)
		{
			Sprite* coin3 = Sprite::create("res/rewards/coin.png");
			coin3->setScale(0);
			coin3->setPosition(oomeeWorldPos);
			coin3->runAction(Sequence::createWithTwoActions(DelayTime::create(i + 0.66),MoveTo::create(1, mainCoin->getPosition())));
			coin3->runAction(Sequence::createWithTwoActions(DelayTime::create(i + 0.66), ScaleTo::create(1, 2.5)));
			this->addChild(coin3);
		}
	}
}
NS_AZOOMEE_END
