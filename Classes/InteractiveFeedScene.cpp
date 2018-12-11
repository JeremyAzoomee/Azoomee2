//
//  InteractiveFeedScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/12/2018.
//

#include "InteractiveFeedScene.h"
#include "InfinitePageView.h"
#include <AzoomeeChat/UI/AvatarWidget.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool InteractiveFeedScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	InfinitePageView* pageView = InfinitePageView::create();
	pageView->setDirection(ui::ScrollView::Direction::VERTICAL);
	pageView->setContentSize(this->getContentSize());
	pageView->setPosition(Vec2(0,0));
	pageView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
	pageView->setBackGroundColor(Color3B::RED);
	pageView->setBounceEnabled(true);
	pageView->setPageData({"res/testAssets/yeti.json","res/testAssets/playSkytte.json",
		"res/testAssets/Jiwi.json", "res/testAssets/marvellousMakes.json","res/testAssets/unicorn.json",
		"res/testAssets/homework.json", "res/testAssets/football.json", "res/testAssets/art.json",
		"res/testAssets/gwr.json"
	});
	this->addChild(pageView);
	
	pageView->setSwallowTouches(false);
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		if(_touchStarted)
		{
			return false;
		}
		_touchTrail = ParticleFlower::create();
		_touchTrail->initWithTotalParticles(200);
		_touchTrail->setLife(2);
		_touchTrail->setStartSize(80);
		_touchTrail->setStartSizeVar(10);
		_touchTrail->setEndSize(40);
		_touchTrail->setEndSizeVar(5);
		_touchTrail->setPositionType(ParticleSystem::PositionType::RELATIVE);
		_touchTrail->setPosition(this->convertTouchToNodeSpace(touch) - Director::getInstance()->getVisibleOrigin());
		this->addChild(_touchTrail);
		_touchStarted = true;
		return true;
	};
	
	listener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		_touchTrail->setPosition(this->convertTouchToNodeSpace(touch) - Director::getInstance()->getVisibleOrigin());
	};
	
	listener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		_touchTrail->setDuration(0.5);
		_touchTrail->setAutoRemoveOnFinish(true);
		
		_touchStarted = false;
	};
	
	listener->onTouchCancelled = listener->onTouchEnded;
	
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	
	ui::Button* gridButton = ui::Button::create("res/navigation/grid_icon.png");
	gridButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	gridButton->setAnchorPoint(Vec2(1.25,1.25));
	gridButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	this->addChild(gridButton);
	
	Chat::AvatarWidget* childSelectButton = Chat::AvatarWidget::create();
	Chat::FriendRef data = Chat::Friend::create(ChildDataProvider::getInstance()->getLoggedInChildId(), ChildDataProvider::getInstance()->getLoggedInChildName(), ChildDataProvider::getInstance()->getLoggedInChildAvatarId());
	childSelectButton->setAvatarForFriend(data);
	childSelectButton->setContentSize(gridButton->getContentSize());
	childSelectButton->setAnchorPoint(Vec2(-0.25,1.25));
	childSelectButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	childSelectButton->setTouchEnabled(true);
	childSelectButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ChildDataParser::getInstance()->setChildLoggedIn(false);
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
		}
	});
	this->addChild(childSelectButton);
	
	
	
	return true;
}

void InteractiveFeedScene::onEnter()
{
	Super::onEnter();
}

NS_AZOOMEE_END
