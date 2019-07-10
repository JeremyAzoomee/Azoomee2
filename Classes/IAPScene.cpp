//
//  IAPScene.cpp
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#include "IAPScene.h"
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool IAPScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = getContentSize();
	bool isPortrait = contentSize.width < contentSize.height;
	
	_marketingCarousel = MarketingCarousel::create();
	_marketingCarousel->setSizeType(ui::Layout::SizeType::PERCENT);
	_marketingCarousel->setSizePercent(isPortrait ? Vec2(1.0f, 0.5f) : Vec2(0.5f,1.0f));
	_marketingCarousel->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
	_marketingCarousel->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
	this->addChild(_marketingCarousel);
	
	_closeButton = ui::Button::create("res/onboarding/close.png");
	_closeButton->setAnchorPoint(Vec2(-0.3,1.2));
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_closeButton->ignoreContentAdaptWithSize(false);
	_closeButton->setContentSize(Size(200, 200));
	_closeButton->setColor(Color3B(245, 2, 109));
	_closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
		}
	});
	this->addChild(_closeButton);
	
	return true;
}
void IAPScene::onEnter()
{
	std::vector<MarketingPageData> pageData;
	for(int i = 0; i < 3; i++)
	{
		std::string jsonString = "{\"url\":\"res/onboarding/testimg.jpg\",\"title\":\"test\",\"subHeading\":\"test sub heading\"}";
		rapidjson::Document jsonDoc;
		jsonDoc.Parse(jsonString.c_str());
		MarketingPageData data = MarketingPageData();
		data.initWithData(jsonDoc);
		pageData.push_back(data);
	}
	_marketingCarousel->setPageData(pageData);
	
	Super::onEnter();
}
void IAPScene::onExit()
{
	Super::onExit();
}
void IAPScene::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = getContentSize();
	bool isPortrait = contentSize.width < contentSize.height;
	
	_marketingCarousel->setSizePercent(isPortrait ? Vec2(1.0f, 0.5f) : Vec2(0.5f,1.0f));
	_marketingCarousel->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
	_marketingCarousel->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
}

NS_AZOOMEE_END
