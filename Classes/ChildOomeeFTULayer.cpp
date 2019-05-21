//
//  ChildOomeeFTULayer.cpp
//  azoomee2
//
//  Created by Macauley on 02/05/2019.
//

#include "ChildOomeeFTULayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Tutorial/TutorialSpeechBubble.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildOomeeFTULayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void ChildOomeeFTULayer::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
	const bool isPortrait = contentSize.width < contentSize.height;
	
	Sprite* bgLights = Sprite::create("res/rewards/wow_bg.png");
	bgLights->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	bgLights->runAction(RepeatForever::create(RotateBy::create(1.0,90)));
	this->addChild(bgLights);
	
	addParticles();
	
	Sprite* oomee = Sprite::create(ConfigStorage::getInstance()->getLocalImageForOomee(_childCreator->getOomeeNum()));
	oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	oomee->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.55f) : Vec2(0.5,0.45f));
	oomee->setContentSize(oomee->getContentSize() * ((contentSize.height * (isPortrait ? 0.6f : 0.75f)) / oomee->getContentSize().height));
	this->addChild(oomee);
	
	oomee->setScale(0);
	oomee->runAction(Sequence::create(DelayTime::create(1.0f),EaseBackOut::create(ScaleTo::create(1.0f, 1.0f)) ,NULL));
	
	TutorialSpeechBubble* bubble = TutorialSpeechBubble::create();
	bubble->setBubbleOrigin(BubbleOrigin::BOTTOM);
	bubble->setMaxWidth(contentSize.width * (isPortrait ? 0.75f : 0.4f));
	bubble->setText(_("Fantastic!\nLet me show you around"));
	bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	bubble->setNormalizedPosition(Vec2(0.5f, 0.85f));
	oomee->addChild(bubble);
	
	ui::Button* continueButton = ui::Button::create("res/buttons/blue_arrow_button.png");
	continueButton->setVisible(false);
	continueButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
	{
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->nextLayer();
			}
		}
	});
	if(isPortrait)
	{
		continueButton->setAnchorPoint(Vec2(0.5f, 1.5f));
		continueButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
		oomee->addChild(continueButton);
	}
	else
	{
		continueButton->setAnchorPoint(Vec2(-0.25f,0.5f));
		continueButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
		bubble->addChild(continueButton);
	}
	
	bubble->animateIn(2.0f, [continueButton](){
		continueButton->setVisible(true);
	});
	
	saveDefaultOomeeToOomeeMakerFiles();
	
	Super::onEnter();
}

void ChildOomeeFTULayer::addParticles()
{
	const std::vector<cocos2d::Color4F> colours = {
		cocos2d::Color4F(1, 0.81, 0.06, 1),
		cocos2d::Color4F(1, 0.43, 0.01, 1),
		cocos2d::Color4F(0.42, 0.22, 0.73, 1),
		cocos2d::Color4F(0.13, 0.58, 0.87, 1)
	};
	
	const Size& visibleSize = this->getContentSize();
	
	Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/rewards/star.png");
	
	for(const Color4F& colour : colours)
	{
		Color4F endColour = colour;
		endColour.a = 0.1f;
		ParticleSystemQuad* sparkle = ParticleSystemQuad::createWithTotalParticles(30);
		sparkle->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
		sparkle->setEndColor(endColour);
		sparkle->setEndColorVar(Color4F(0,0,0,1.0));
		sparkle->setStartColor(colour);
		sparkle->setStartColorVar(Color4F(0,0,0,0.1));
		sparkle->setTexture(particleTex);
		sparkle->setLife(1.0);
		sparkle->setLifeVar(0.5);
		sparkle->setGravity(Vec2(0,0));
		sparkle->setStartSize(25);
		sparkle->setStartSizeVar(15);
		sparkle->setEndSize(135);
		sparkle->setEndSizeVar(45);
		sparkle->setPosVar(visibleSize);
		sparkle->setAngleVar(0);
		sparkle->setAngle(0);
		sparkle->setEmissionRate(15);
		sparkle->setDuration(-1);
		sparkle->setSpeed(0);
		sparkle->setSpeedVar(0);
		sparkle->setStartSpin(0);
		sparkle->setStartSpinVar(50);
		sparkle->setEndSpin(0);
		sparkle->setEndSpinVar(50);
		sparkle->setAutoRemoveOnFinish(true);
		sparkle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
		sparkle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		this->addChild(sparkle);
	}
	
}

void ChildOomeeFTULayer::saveDefaultOomeeToOomeeMakerFiles()
{
	const std::string& imageData = FileUtils::getInstance()->getStringFromFile(ConfigStorage::getInstance()->getLocalImageForOomee(_childCreator->getOomeeNum()));
	const std::string& oomeeConfig = FileUtils::getInstance()->getStringFromFile(ConfigStorage::getInstance()->getLocalConfigForOomee(_childCreator->getOomeeNum()));
	std::string filePath = FileUtils::getInstance()->getWritablePath() + "oomeeMaker/";
	if(!FileUtils::getInstance()->isDirectoryExist(filePath))
	{
		FileUtils::getInstance()->createDirectory(filePath);
	}
	filePath += _childCreator->getCreatedChildId() + "/";
	if(!FileUtils::getInstance()->isDirectoryExist(filePath))
	{
		FileUtils::getInstance()->createDirectory(filePath);
	}
	
	FileUtils::getInstance()->writeStringToFile(imageData, filePath + "default_oomee.png");
	FileUtils::getInstance()->writeStringToFile(oomeeConfig, filePath + "default_oomee.oomee");
}

NS_AZOOMEE_END

