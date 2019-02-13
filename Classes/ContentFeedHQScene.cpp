//
//  ContentFeedHQScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/01/2019.
//

#include "ContentFeedHQScene.h"
#include "HQDataProvider.h"
#include "ContentHistoryManager.h"
#include "HQScene2ElementPositioner.h"
#include "HQScene2PlaceHolderCreator.h"
#include "HQScene2CarouselTitle.h"
#include "ArtAppDelegate.h"
#include "HQHistoryManager.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <AzoomeeCommon/Utils/IosNativeFunctionsSingleton.h>
#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float ContentFeedHQScene::kSpaceForPrivacyPolicy = 100.0f;
const std::string ContentFeedHQScene::kGroupLogoName = "groupLogo";

bool ContentFeedHQScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_type = HQSceneType::CONTENT_FEED_HQ;
	
	return true;
}
void ContentFeedHQScene::onEnter()
{
	createContentScrollview();
	
	Super::onEnter();
}

void ContentFeedHQScene::createContentScrollview()
{
	const Size& visibleSize = this->getContentSize();
	
	const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
	const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
	const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
	const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
	
	const Size& contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory);
	const float unitWidth = (visibleSize.width - 2 * sideMargin - contentItemMargin / 2.0f) / unitsOnScreen;
	const float unitMultiplier = unitWidth / contentItemSize.width;
	
	if(_hqCategory == "")
	{
		return;
	}
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(startingReviewProcessRequired())
	{
		IosNativeFunctionsSingleton::getInstance()->startIosReviewProcess();
	}
#endif
	std::vector<LayerColor*> carouselStorage;
	float totalHeightOfCarousels = 0;
	
	//addRecentlyPlayedCarousel();
	
	for(int rowIndex = 0; rowIndex < HQDataProvider::getInstance()->getNumberOfRowsForHQ(_hqCategory); rowIndex++)
	{
		const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(_hqCategory, rowIndex);
		float lowestElementYPosition = 0;
		
		cocos2d::LayerColor* carouselLayer =  cocos2d::LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), visibleSize.width - 2 * sideMargin, 0);
		
		for(int elementIndex = 0; elementIndex < elementsForRow.size(); elementIndex++)
		{
			auto hqSceneElement = HQSceneElement::create();
			hqSceneElement->setCategory(_hqCategory);
			hqSceneElement->setItemData(elementsForRow.at(elementIndex));
			hqSceneElement->setElementRow(rowIndex);
			hqSceneElement->setElementIndex(elementIndex);
			hqSceneElement->setMargin(contentItemMargin);
			hqSceneElement->setManualSizeMultiplier(unitMultiplier);
			hqSceneElement->addHQSceneElement();
			
			cocos2d::Vec2 elementShape = HQDataProvider::getInstance()->getHighlightDataForSpecificItem(_hqCategory, rowIndex, elementIndex);
			
			HQScene2ElementPositioner hqScene2ElementPositioner;
			hqScene2ElementPositioner.setElement(hqSceneElement);
			hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
			hqScene2ElementPositioner.setHighlightData(elementShape);
			hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
			
			const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
			
			hqSceneElement->setPosition(elementPosition);
			carouselLayer->addChild(hqSceneElement);
			
			if(elementPosition.y < lowestElementYPosition)
			{
				lowestElementYPosition = elementPosition.y;
			}
		}
		
		//Filling up empty spaces with placeholders (Design requirement - except for Group HQ)
		
		if(_hqCategory != ConfigStorage::kGroupHQName)
		{
			HQScene2PlaceHolderCreator hqScene2PlaceHolderCreator;
			hqScene2PlaceHolderCreator.setLowestElementYPosition(lowestElementYPosition);
			hqScene2PlaceHolderCreator.setCarouselLayer(carouselLayer);
			hqScene2PlaceHolderCreator.setBaseUnitSize(contentItemSize * unitMultiplier);
			hqScene2PlaceHolderCreator.setMargin(contentItemMargin);
			hqScene2PlaceHolderCreator.setUseWirePlaceholder(rowIndex == 0);
			hqScene2PlaceHolderCreator.addPlaceHoldersToCarousel();
		}
		
		carouselLayer->setContentSize(Size(visibleSize.width - 2 * sideMargin, -lowestElementYPosition));
		
		for(cocos2d::Node* contentItem : carouselLayer->getChildren())
		{
			contentItem->setPositionY(contentItem->getPositionY() - lowestElementYPosition);
		}
		totalHeightOfCarousels += carouselLayer->getContentSize().height + spaceAboveCarousel;
		carouselStorage.push_back(carouselLayer);
	}
	
	//we have all carousels in a vector, time to resize the scrollview and add them one by one
	bool isGroupHQ = _hqCategory == ConfigStorage::kGroupHQName;
	
	Size vScrollFrameSize = Size(visibleSize.width - sideMargin * 2, visibleSize.height - (isGroupHQ ? 200.0f : 500.0f));
	
	_contentScrollview = cocos2d::ui::ScrollView::create();
	_contentScrollview->setContentSize(vScrollFrameSize);
	_contentScrollview->setPosition(Point(sideMargin, isGroupHQ ? 0 : 300));
	_contentScrollview->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	_contentScrollview->setTouchEnabled(true);
	_contentScrollview->setBounceEnabled(true);
	_contentScrollview->setScrollBarEnabled(false);
	_contentScrollview->setSwallowTouches(true);
	_contentScrollview->setInnerContainerSize(cocos2d::Size(visibleSize.width - 2 * sideMargin, totalHeightOfCarousels + kSpaceForPrivacyPolicy));
	
	float lastCarouselPosition = _contentScrollview->getInnerContainerSize().height;
	for(int carouselIndex = 0; carouselIndex < carouselStorage.size(); carouselIndex++)
	{
		if(HQDataProvider::getInstance()->getElementsForRow(_hqCategory, carouselIndex).size() == 0)
		{
			continue;
		}
		
		lastCarouselPosition -= spaceAboveCarousel;
		
		cocos2d::Layer *carouselTitle = HQScene2CarouselTitle::createForCarousel(HQDataObjectStorage::getInstance()->getHQDataObjectForKey(_hqCategory)->getHqCarousels()[carouselIndex]);
		carouselTitle->setPosition(cocos2d::Vec2(_contentScrollview->getContentSize().width / 2, lastCarouselPosition));
		_contentScrollview->addChild(carouselTitle);
		
		cocos2d::Node* carousel = carouselStorage.at(carouselIndex);
		lastCarouselPosition -= carousel->getContentSize().height;
		carousel->setPosition(0, lastCarouselPosition);
		_contentScrollview->addChild(carousel);
	}
	
	//add privacy policy to the very end to keep COPPA happy
	
	lastCarouselPosition -= kSpaceForPrivacyPolicy;
	PrivacyLayer* privacyLayer = PrivacyLayer::createWithColor();
	privacyLayer->setCenterPosition(Vec2(_contentScrollview->getContentSize().width / 2, lastCarouselPosition + privacyLayer->getContentSize().height));
	_contentScrollview->addChild(privacyLayer);
	
	this->addChild(_contentScrollview);
	
	//add gradient on top of scrollView
	
	cocos2d::Sprite* gradient = createGradientForScrollView(_contentScrollview->getContentSize().width);
	gradient->setPosition(_contentScrollview->getContentSize().width / 2 + _contentScrollview->getPosition().x, _contentScrollview->getPosition().y + _contentScrollview->getContentSize().height);
	this->addChild(gradient);
	
	//add group hq logo if necessary
	
	if(isGroupHQ)
	{
		addGroupHQLogo();
		_navLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		_settingsButton->setVisible(false);
		ui::Button* backButton = ui::Button::create("res/navigation/back_button.png");
		backButton->setAnchorPoint(Vec2(-0.25, 1.25));
		backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
		backButton->addTouchEventListener([&](Ref* pSender, ui::Button::TouchEventType eType){
			if(eType == ui::Button::TouchEventType::ENDED)
			{
				HQHistoryManager::getInstance()->popHQ();
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
			}
		 });
		 this->addChild(backButton,1);
	}
}

bool ContentFeedHQScene::showingPostContentCTARequired()
{
	if(!ContentHistoryManager::getInstance()->getReturnedFromContent() || _hqCategory == ConfigStorage::kGroupHQName)
	{
		return false;
	}
	
	int secondsInContent = 0;
	try
	{
		const std::string& secondsString = AnalyticsProperties::getInstance()->getStoredContentItemProperties().at("SecondsInContent");
		secondsInContent = std::atoi(secondsString.substr(secondsString.find("|")+1).c_str());
	}
	catch(std::out_of_range)
	{
		ContentHistoryManager::getInstance()->setReturnedFromContent(false);
		return false;
	}
	
	HQContentItemObjectRef lastContent = ContentHistoryManager::getInstance()->getLastOpenedContent();
	if(lastContent == nullptr || (lastContent->getType() == ConfigStorage::kContentTypeGame && secondsInContent < 180))
	{
		ContentHistoryManager::getInstance()->setReturnedFromContent(false);
		return false;
	}
	
	return true;
}

bool ContentFeedHQScene::startingReviewProcessRequired()
{
	if(showingPostContentCTARequired())
	{
		return true;
	}
	
	if(_hqCategory == ConfigStorage::kArtAppHQName && ArtAppDelegate::getInstance()->getSecondsSpentInArtApp() > 180)
	{
		ArtAppDelegate::getInstance()->setSecondsSpentInArtApp(0);
		return true;
	}
	
	return false;
}

Sprite* ContentFeedHQScene::createGradientForScrollView(float scrollViewWith)
{
	Sprite* verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
	verticalScrollGradient->setAnchorPoint(Vec2(0.5, 1.0));
	verticalScrollGradient->setScaleX(scrollViewWith / verticalScrollGradient->getContentSize().width);
	verticalScrollGradient->setColor(Color3B::BLACK);
	
	return verticalScrollGradient;
}

void ContentFeedHQScene::addGroupHQLogo()
{
	if(HQHistoryManager::getInstance()->getGroupHQSourceId() != "")
	{
		const Size& visibleSize = this->getContentSize();
		const std::string &groupHQLogoUrl = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName)->getGroupLogo();
		
		this->removeChild(this->getChildByName(kGroupLogoName));
		
		auto groupLogo = RemoteImageSprite::create();
		groupLogo->initWithUrlAndSizeWithoutPlaceholder(groupHQLogoUrl, ConfigStorage::getInstance()->getGroupHQLogoSize());
		groupLogo->setScale(0.8);
		groupLogo->setPosition(visibleSize.width / 2, visibleSize.height - groupLogo->getBoundingBox().size.height * 0.55);
		groupLogo->setName(kGroupLogoName);
		this->addChild(groupLogo);
	}
}

void ContentFeedHQScene::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
