//
//  CoinDisplay.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 30/01/2019.
//

#include "CoinDisplay.h"
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string CoinDisplay::kAnimClipNodeName = "clipNode";
const std::string CoinDisplay::kAnimFrameGlowName = "frameGlow";
const std::string CoinDisplay::kAnimSliderName = "slider";
const std::string CoinDisplay::kAnimStarName = "star";

float CoinDisplay::sCoinCount = 0;
bool CoinDisplay::sAnimating = false;
float CoinDisplay::sIncPerSec = 0;
int CoinDisplay::sTargetVal = 0;

bool CoinDisplay::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/meHQ/chat_bg.png");
	stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	stencil->setContentSize(Size(350,100));
	
	this->setContentSize(stencil->getContentSize());
	
	_valueFrame = ClippingNode::create(stencil);
	_valueFrame->setAlphaThreshold(0.5);
	this->addChild(_valueFrame);
	
	_valueBG = LayerColor::create(Color4B(30, 74, 123, 65));
	_valueBG->setContentSize(stencil->getContentSize());
	_valueFrame->addChild(_valueBG);
	
	_coinsLabel = Label::createWithTTF(StringUtils::format("%d",(int)sCoinCount), Style::Font::Bold(), 75);
	_coinsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_coinsLabel->setNormalizedPosition(Vec2(0.6,0.5));
	_coinsLabel->setTextColor(Color4B::WHITE);
	_coinsLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_coinsLabel->setVerticalAlignment(TextVAlignment::CENTER);
	_coinsLabel->setOverflow(Label::Overflow::SHRINK);
	_coinsLabel->setDimensions(200, 75);
	_valueBG->addChild(_coinsLabel);
	
	_coinSprite = Sprite::create("res/rewards/coin.png");
	_coinSprite->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	this->addChild(_coinSprite, 1); // coin sprite on higher zorder to sit above glow anim sprites on banner, which are added later
	
	this->setTouchEnabled(true);
	this->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			TutorialController::getInstance()->setTutorialCompleted(TutorialController::kFTUShopID);
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUSpendRewards)
			{
				TutorialController::getInstance()->nextStep();
			}
			AnalyticsSingleton::getInstance()->coinCounterPressedEvent();
			AudioMixer::getInstance()->playEffect("CoinCounterIcon_Click.wav");
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Shop));
		}
	});
	
	return true;
}
void CoinDisplay::onEnter()
{
	scheduleUpdate();
	if(_animate)
	{
		this->runAction(Sequence::create(DelayTime::create(4.0f),CallFunc::create([this](){
			createGlowAnim();
		}),NULL));
	}
	Super::onEnter();
}

void CoinDisplay::onExit()
{
	unscheduleUpdate();
	if(_animInitialised)
	{
		removeGlowAnim();
	}
	Super::onExit();
}

void CoinDisplay::update(float deltaT)
{
	if(sAnimating)
	{
		sCoinCount += sIncPerSec * deltaT;
		if(abs(sCoinCount - sTargetVal) < abs(sIncPerSec))
		{
			sCoinCount = sTargetVal;
			_coinsLabel->setScale(1);
			sAnimating = false;
		}
		_coinsLabel->setString(StringUtils::format("%d",(int)sCoinCount));
	}
	else
	{
		sTargetVal = ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins();
		if(sTargetVal != sCoinCount)
		{
			sIncPerSec = (sTargetVal - sCoinCount) / 2.0f;
			_coinsLabel->setScale(1.2f);
			sAnimating = true;
			AudioMixer::getInstance()->playEffect("CoinCounterIcon_NumberGoingUp.wav");
		}
	}
	
	Super::update(deltaT);
}

void CoinDisplay::setAnimate(bool animate)
{
	_animate = animate;
}

void CoinDisplay::createGlowAnim()
{
	this->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.25, 1.1),EaseBounceOut::create(ScaleTo::create(0.75, 1.0)),DelayTime::create(7.0f), NULL)));
	
	Sprite* coinStencil = Sprite::create("res/rewards/coin.png");
	coinStencil->setPosition(coinStencil->getContentSize() / 2);
	ClippingNode* coinClippingNode = ClippingNode::create(coinStencil);
	coinClippingNode->setAlphaThreshold(0.5);
	coinClippingNode->setName(kAnimClipNodeName);
	_coinSprite->addChild(coinClippingNode, 1);
	
	Sprite* coinGlow = Sprite::create("res/shop/Glow_Counter_Animation.png");
	coinGlow->setOpacity(180);
	coinGlow->setScale((_coinSprite->getContentSize().height * 1.4f) / coinGlow->getContentSize().height);
	coinGlow->setPosition(Vec2(0,_coinSprite->getContentSize().height));
	coinGlow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	coinGlow->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1), MoveTo::create(0.6, Vec2(_coinSprite->getContentSize().width + coinGlow->getContentSize().width * coinGlow->getScale(),-coinGlow->getContentSize().height * coinGlow->getScale())), MoveTo::create(0, Vec2(0,_coinSprite->getContentSize().height)), DelayTime::create(6.4), NULL)));
	coinClippingNode->addChild(coinGlow);
	
	Sprite* frameGlow = Sprite::create("res/shop/Glow_Counter_Animation.png");
	frameGlow->setOpacity(75);
	frameGlow->setScale((_valueBG->getContentSize().height * 1.6f) / frameGlow->getContentSize().height);
	frameGlow->setPosition(Vec2(-frameGlow->getContentSize().width * frameGlow->getScale(),_valueBG->getContentSize().height / 2));
	frameGlow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	frameGlow->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.6),MoveTo::create(0.6, Vec2(_valueBG->getContentSize().width,_valueBG->getContentSize().height / 2)), MoveTo::create(0, Vec2(-frameGlow->getContentSize().width * frameGlow->getScale(),_valueBG->getContentSize().height / 2)), DelayTime::create(5.8), NULL)));
	frameGlow->setName(kAnimFrameGlowName);
	_valueBG->addChild(frameGlow);
	
	Sprite* slider = Sprite::create("res/shop/side_shooter.png");
	slider->setPosition(Vec2(0,this->getContentSize().height));
	slider->setScale(0.75f);
	slider->setRotation(90);
	slider->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.7),MoveTo::create(0.3, Vec2(this->getContentSize().width,this->getContentSize().height)), MoveTo::create(0, Vec2(0,this->getContentSize().height)), DelayTime::create(5.0), NULL)));
	slider->setName(kAnimSliderName);
	this->addChild(slider);
	
	Sprite* star = Sprite::create("res/shop/star.png");
	star->setPosition(this->getContentSize());
	star->runAction(RepeatForever::create(RotateBy::create(0.5, 180)));
	star->setScale(0);
	star->runAction(RepeatForever::create(Sequence::create(DelayTime::create(3.0), ScaleTo::create(0.5, 1), ScaleTo::create(0.25, 0), DelayTime::create(4.25), NULL)));
	star->setName(kAnimSliderName);
	this->addChild(star);
	
	_animInitialised = true;
}

void CoinDisplay::removeGlowAnim()
{
	_coinSprite->removeChildByName(kAnimClipNodeName);
	_valueBG->removeChildByName(kAnimFrameGlowName);
	this->removeChildByName(kAnimSliderName);
	this->removeChildByName(kAnimStarName);
	_animInitialised = false;
}

NS_AZOOMEE_END
