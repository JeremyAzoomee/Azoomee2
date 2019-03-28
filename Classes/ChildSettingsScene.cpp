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
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildSettingsScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	const bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	const bool isPortrait = visibleSize.height > visibleSize.width;
	
	LayerColor* bgColour = LayerColor::create(Color4B::WHITE, visibleSize.width, visibleSize.height);
	this->addChild(bgColour);
	
	auto wireLeft = Sprite::create(StringUtils::format("res/childSelection/wireLeft%s.png", isPortrait ? "_portrait" : ""));
	wireLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	wireLeft->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	wireLeft->setScale(visibleSize.height / wireLeft->getContentSize().height);
	wireLeft->setColor(Color3B::BLACK);
	wireLeft->setOpacity(125);
	this->addChild(wireLeft);
	
	auto wireRight = Sprite::create(StringUtils::format("res/childSelection/wireRight%s.png", isPortrait ? "_portrait" : ""));
	wireRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	wireRight->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	wireRight->setScale(visibleSize.height / wireRight->getContentSize().height);
	wireRight->setColor(Color3B::BLACK);
	wireRight->setOpacity(125);
	this->addChild(wireRight);
	
	_contentLayout = ui::Layout::create();
	_contentLayout->setContentSize(visibleSize);
	_contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	_contentLayout->setBackGroundColor(Style::Color::greyBlue);
	_contentLayout->setBackGroundColorOpacity(0);
	_contentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
	this->addChild(_contentLayout);
	
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
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
		}
	});
	
	_titleText = ui::Text::create(_("Settings"), Style::Font::Medium(), 91);
	_titleText->setNormalizedPosition(isIphoneX ? Vec2(0.5f,0.25f) : Vec2::ANCHOR_MIDDLE);
	_titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_titleText->setTextColor(Color4B::WHITE);
	_titleLayout->addChild(_titleText);
	
	Sprite* icon = Sprite::create("res/settings/switch_profile_icon.png");
	icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	icon->setAnchorPoint(Vec2(-0.125,0.5));
	
	_childSelect = ui::Button::create("res/settings/rounded_button.png");
	
	float scale = (icon->getContentSize().height * 1.25) / _childSelect->getContentSize().height;
	
	_childSelect->setContentSize(_childSelect->getContentSize() * scale);
	_childSelect->ignoreContentAdaptWithSize(false);
	_childSelect->setColor(Style::Color::skyBlue);
	_childSelect->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_childSelect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_childSelect->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ChildDataParser::getInstance()->setChildLoggedIn(false);
			HQHistoryManager::getInstance()->emptyHistory();
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChildSelector));
		}
	});
	_contentLayout->addChild(_childSelect);
	
	_childSelect->addChild(icon);
	
	Label* childSelectText = Label::createWithTTF(_("Switch Profile"), Style::Font::Regular(), 70);
	childSelectText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	childSelectText->setColor(Color3B::WHITE);
	childSelectText->setHorizontalAlignment(TextHAlignment::CENTER);
	childSelectText->setVerticalAlignment(TextVAlignment::CENTER);
	childSelectText->setOverflow(Label::Overflow::SHRINK);
	childSelectText->setDimensions(_childSelect->getContentSize().width * 0.6f, _childSelect->getContentSize().height * 0.6f);
	childSelectText->setNormalizedPosition(Vec2(0.6,0.5));
	_childSelect->addChild(childSelectText);
	
	_parentsArea = ui::Button::create("res/settings/rounded_button.png");
	_parentsArea->setContentSize(_parentsArea->getContentSize() * scale);
	_parentsArea->ignoreContentAdaptWithSize(false);
	_parentsArea->setColor(Style::Color::skyBlue);
	_parentsArea->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	_parentsArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_parentsArea->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::SettingsFromHQ));
		}
	});
	_contentLayout->addChild(_parentsArea);
	
	Sprite* icon2 = Sprite::create("res/settings/parents_area_icon.png");
	icon2->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	icon2->setAnchorPoint(Vec2(-0.125,0.5));
	_parentsArea->addChild(icon2);
	
	Label* parentAreaText = Label::createWithTTF(_("Parents' Zone"), Style::Font::Regular(), 70);
	parentAreaText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	parentAreaText->setColor(Color3B::WHITE);
	parentAreaText->setHorizontalAlignment(TextHAlignment::CENTER);
	parentAreaText->setVerticalAlignment(TextVAlignment::CENTER);
	parentAreaText->setOverflow(Label::Overflow::SHRINK);
	parentAreaText->setDimensions(_parentsArea->getContentSize().width * 0.6f, _parentsArea->getContentSize().height * 0.6f);
	parentAreaText->setNormalizedPosition(Vec2(0.6,0.5));
	_parentsArea->addChild(parentAreaText);
	
	
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
