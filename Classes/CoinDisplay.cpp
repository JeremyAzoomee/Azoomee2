//
//  CoinDisplay.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 30/01/2019.
//

#include "CoinDisplay.h"
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
	
	return true;
}
void CoinDisplay::onEnter()
{
	ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/meHQ/chat_bg.png");
	stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	stencil->setContentSize(Size(350,100));
	
	this->setContentSize(stencil->getContentSize());
	
	_valueFrame = ClippingNode::create(stencil);
	_valueFrame->setAlphaThreshold(0.5);
	this->addChild(_valueFrame);
	
	_valueBG = LayerGradient::create(Color4B(33, 147, 221, 255), Color4B(23, 214, 249,255), Vec2(1.0,0.25));
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
	this->addChild(_coinSprite);
	
	scheduleUpdate();
	
	Super::onEnter();
}

void CoinDisplay::update(float deltaT)
{
	if(sAnimating)
	{
		sCoinCount += sIncPerSec * deltaT;
		if(sCoinCount >= sTargetVal)
		{
			sCoinCount = sTargetVal;
			_coinsLabel->setScale(1);
			sAnimating = false;
		}
		_coinsLabel->setString(StringUtils::format("%d",(int)sCoinCount));
	}
	else
	{
		sTargetVal = ChildDataProvider::getInstance()->getLoggedInChild()->_coins;
		if(sTargetVal != sCoinCount)
		{
			sIncPerSec = (sTargetVal - sCoinCount) / 2.0f;
			_coinsLabel->setScale(1.2f);
			sAnimating = true;
		}
	}
	
	Super::update(deltaT);
}

NS_AZOOMEE_END
