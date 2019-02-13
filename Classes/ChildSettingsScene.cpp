//
//  ChildSettingsScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 13/02/2019.
//

#include "ChildSettingsScene.h"

#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildSettingsScene::init()
{
	if(Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	
	LayerColor* bgColour = LayerColor::create(Color4B::WHITE, visibleSize.width, visibleSize.height);
	this->addChild(bgColour);
	
	_contentLayout = ui::Layout::create();
	_contentLayout->setContentSize(visibleSize);
	_contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	_contentLayout->setBackGroundColor(Style::Color::greyBlue);
	_contentLayout->setBackGroundColorOpacity(0);
	_contentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
	this->addChild(_contentLayout);
	
	bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	bool isPortrait = visibleSize.height > visibleSize.width;
	
	_titleLayout = ui::Layout::create();
	_titleLayout->setContentSize(Size(visibleSize.width, (isIphoneX && isPortrait) ? 250 : 150));
	_titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	_titleLayout->setBackGroundColor(Style::Color::skyBlue);
	_contentLayout->addChild(_titleLayout);
	
	_titleBarButton = ui::Button::create("res/settings/exit_button.png");
	_titleBarButton->setNormalizedPosition((isIphoneX && isPortrait) ? Vec2(0,0.33f) : Vec2::ANCHOR_MIDDLE_LEFT);
	_titleBarButton->setAnchorPoint(Vec2(0.0f,0.5f));
	_titleBarButton->setContentSize(Size(150, 150));
	_titleBarButton->ignoreContentAdaptWithSize(false);
	_titleLayout->addChild(_titleBarButton);
	_titleBarButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	
	_titleText = ui::Text::create(_("Settings"), Style::Font::Medium(), 91);
	_titleText->setNormalizedPosition(isIphoneX ? Vec2(0.5f,0.25f) : Vec2::ANCHOR_MIDDLE);
	_titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_titleText->setTextColor(Color4B::WHITE);
	_titleLayout->addChild(_titleText);
	
	_childSelect = ui::Button::create();
	
	
	return true;
}

void ChildSettingsScene::onEnter()
{
	Super::onEnter();
}

void ChildSettingsScene::onSizeChanged()
{
	Super::onSizeChanged();
}

NS_AZOOMEE_END
