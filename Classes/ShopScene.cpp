//
//  ShopScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 07/03/2019.
//

#include "ShopScene.h"
#include "SceneManagerScene.h"
#include "CoinDisplay.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = this->getContentSize();
	
	LayerColor* bgColour = LayerColor::create(Color4B(3, 36, 78,60));
	this->addChild(bgColour);
	
	Sprite* wires = Sprite::create("res/shop/wires.png");
	wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	wires->setScale(visibleSize.width / wires->getContentSize().width);
	//wires->setOpacity(60);
	this->addChild(wires);
	
	Sprite* gradient = Sprite::create("res/shop/gradient_haze.png");
	gradient->setScaleX(visibleSize.width / gradient->getContentSize().width);
	gradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	gradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(gradient);
	
	ui::Button* backButton = ui::Button::create("res/shop/back_white_v_2.png");
	backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	backButton->setAnchorPoint(Vec2(-0.25,1.25));
	backButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
		}
	});
	this->addChild(backButton,1);
	
	CoinDisplay* coinDisplay = CoinDisplay::create();
	coinDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	coinDisplay->setAnchorPoint(Vec2(1.2,1.5));
	this->addChild(coinDisplay, 1);
	
	_shopCarousel = ShopCarousel::create();
	this->addChild(_shopCarousel);
	
	return true;
}
void ShopScene::onEnter()
{
	Super::onEnter();
}
void ShopScene::onExit()
{
	Super::onExit();
}
void ShopScene::onSizeChanged()
{
	Super::onSizeChanged();
	
	DynamicNodeHandler::getInstance()->rebuildCurrentCTA();
}

NS_AZOOMEE_END
