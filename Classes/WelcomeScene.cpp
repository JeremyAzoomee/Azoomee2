//
//  WelcomeScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/01/2019.
//

#include "WelcomeScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool WelcomeScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	createOomeeGuideUI();
	createWelcomeUI();
	
	return true;
}

void WelcomeScene::onEnter()
{
	Super::onEnter();
}

void WelcomeScene::onExit()
{
	Super::onExit();
}

void WelcomeScene::onSizeChanged()
{
	Super::onSizeChanged();
}

void WelcomeScene::createWelcomeUI()
{
	_welcomeLayout = ui::Layout::create();
	_welcomeLayout->setContentSize(this->getContentSize());
	_welcomeLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_welcomeLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_welcomeLayout);
	
	Label* title = Label::createWithTTF(_("Welcome to Azoomee"), Style::Font::Regular(), 120);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	title->setNormalizedPosition(Vec2(0.5f,0.9f));
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setVerticalAlignment(TextVAlignment::CENTER);
	title->setOverflow(Label::Overflow::SHRINK);
	title->setDimensions(this->getContentSize().width * 0.8f, 300);
	_welcomeLayout->addChild(title);
	
	ui::Button* beginButton = ui::Button::create("res/buttons/button_dark.png");
	beginButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	beginButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	beginButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->_oomeeGuideLayout->setVisible(true);
			this->_oomeeGuideLayout->setTouchEnabled(true);
			this->_welcomeLayout->setVisible(false);
		}
	});
	
	Label* beginText = Label::createWithTTF(_("Let's go"), Style::Font::Regular(), beginButton->getContentSize().height * 0.5f);
	beginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	beginText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	beginText->setHorizontalAlignment(TextHAlignment::CENTER);
	beginText->setVerticalAlignment(TextVAlignment::CENTER);
	beginText->setOverflow(Label::Overflow::SHRINK);
	beginText->setDimensions(beginButton->getContentSize().width * 0.8f, beginButton->getContentSize().height * 0.7f);
	beginButton->addChild(beginText);
	_welcomeLayout->addChild(beginButton);
}

void WelcomeScene::createOomeeGuideUI()
{
	_oomeeGuideLayout = ui::Layout::create();
	_oomeeGuideLayout->setContentSize(this->getContentSize());
	_oomeeGuideLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_oomeeGuideLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_oomeeGuideLayout->setVisible(false);
	_oomeeGuideLayout->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
			TutorialController::getInstance()->startTutorial(TutorialController::kFTUOomeeTutorialID);
		}
	});
	this->addChild(_oomeeGuideLayout);
	
	Sprite* placeholder = Sprite::create("res/childSelection/om_GenericParent.png");
	placeholder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_oomeeGuideLayout->addChild(placeholder);
	
}

NS_AZOOMEE_END
