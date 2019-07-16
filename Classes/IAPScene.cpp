//
//  IAPScene.cpp
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#include "IAPScene.h"
#include "SceneManagerScene.h"
#include "IAPProductDataHandler.h"
#include <AzoomeeCommon/Strings.h>

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
	
	_productLayout = ProductLayout::create();
	_productLayout->setSizeType(ui::Layout::SizeType::PERCENT);
	_productLayout->setSizePercent(isPortrait ? Vec2(1.0f, 0.34f) : Vec2(0.5f,0.68f));
	_productLayout->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_TOP_RIGHT);
	_productLayout->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_TOP_RIGHT);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_productLayout->setProductLayoutType(ProductLayoutType::PASS);
	std::vector<std::pair<std::string, std::string>> products = {{_("Get 30 days of Azoomee for only"),IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}};
#else
	_productLayout->setProductLayoutType(ProductLayoutType::SUBSCRIPTION);
	std::vector<std::pair<std::string, std::string>> products = {{_("7-day free trial"),IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}};
#endif
	_productLayout->setProductData(products);
	_productLayout->setIapActionCallback([](IAPAction action){
		switch (action) {
			case IAPAction::PURCHASE:
			{
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Signup));
				break;
			}
			case IAPAction::RESTORE:
			{
				
				break;
			}
		}
	});
	this->addChild(_productLayout);
	
	_footer = IAPFooter::create();
	_footer->setSizeType(ui::Layout::SizeType::PERCENT);
	_footer->setSizePercent(isPortrait ? Vec2(1.0f, 0.16f) : Vec2(0.5f,0.32f));
	_footer->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
	_footer->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
	this->addChild(_footer);
	
	_closeButton = ui::Button::create("res/onboarding/close.png");
	_closeButton->setAnchorPoint(Vec2(-0.2,1.2));
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_closeButton->ignoreContentAdaptWithSize(false);
	_closeButton->setContentSize(Size(100, 100));
	_closeButton->setColor(Color3B::WHITE);
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
	std::vector<std::string> jsonStrings = {
		"{\"url\":\"res/onboarding/Wide Game Asset.jpg\",\"title\":\"Amazing Games!\",\"subHeading\":\"More added every week\"}",
		"{\"url\":\"res/onboarding/Wide Fun Learning Asset.jpg\",\"title\":\"Fun Learning!\",\"subHeading\":\"With games and videos kids love\"}",
		"{\"url\":\"res/onboarding/Wide video asset.jpg\",\"title\":\"Fantastic videos!\",\"subHeading\":\"ad-free and handpicked by humans\"}"
	};
	std::vector<MarketingPageData> pageData;
	for(int i = 0; i < jsonStrings.size(); i++)
	{
		std::string jsonString = jsonStrings.at(i);
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
	
	_productLayout->setSizePercent(isPortrait ? Vec2(1.0f, 0.34f) : Vec2(0.5f,0.68f));
	_productLayout->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_TOP_RIGHT);
	_productLayout->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_TOP_RIGHT);
	
	_footer->setSizePercent(isPortrait ? Vec2(1.0f, 0.16f) : Vec2(0.5f,0.32f));
	_footer->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
	_footer->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
}

NS_AZOOMEE_END
