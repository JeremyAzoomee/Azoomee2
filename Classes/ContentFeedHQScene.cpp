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
#include "ContentOpener.h"
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
	_contentNode = Node::create();
	_contentNode->setContentSize(this->getContentSize());
	this->addChild(_contentNode);
	
	return true;
}
void ContentFeedHQScene::onEnter()
{
	if(_hqCategory == ConfigStorage::kVideoHQName)
	{
		if(!TutorialController::getInstance()->isTutorialActive())
		{
			if(!TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUWatchVideoID))
			{
				TutorialController::getInstance()->startTutorial(TutorialController::kFTUWatchVideoID);
			}
		}
	}
	else if(_hqCategory == ConfigStorage::kGameHQName)
	{
		if(!TutorialController::getInstance()->isTutorialActive())
		{
			if(!TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUPlayGameID))
			{
				TutorialController::getInstance()->startTutorial(TutorialController::kFTUPlayGameID);
			}
		}
	}
	
	if(!_initialised)
	{
		createContentScrollview();
		_initialised = true;
	}
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(startingReviewProcessRequired())
	{
		IosNativeFunctionsSingleton::getInstance()->startIosReviewProcess();
	}
#endif
	
	Super::onEnter();
}

void ContentFeedHQScene::onExit()
{
	if(TutorialController::getInstance()->isTutorialActive() && (TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUVideoHQContent || TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUGameHQContent))
	{
		TutorialController::getInstance()->endTutorial();
	}
	Super::onExit();
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
	
	std::vector<LayerColor*> carouselStorage;
	float totalHeightOfCarousels = 0;
	
	//addRecentlyPlayedCarousel();
	_contentIcons.clear();
	for(int rowIndex = 0; rowIndex < HQDataProvider::getInstance()->getNumberOfRowsForHQ(_hqCategory); rowIndex++)
	{
		const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(_hqCategory, rowIndex);
		float lowestElementYPosition = 0;
		std::vector<HQSceneElement*> buttons;
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
			
			hqSceneElement->setTouchCallback([rowIndex,elementIndex,this](const HQContentItemObjectRef& elementData){
				ContentOpener::getInstance()->doCarouselContentOpenLogic(elementData, rowIndex, elementIndex, _hqCategory);
			});
			
			cocos2d::Vec2 elementShape = HQDataProvider::getInstance()->getHighlightDataForSpecificItem(_hqCategory, rowIndex, elementIndex);
			
			HQScene2ElementPositioner hqScene2ElementPositioner;
			hqScene2ElementPositioner.setElement(hqSceneElement);
			hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
			hqScene2ElementPositioner.setHighlightData(elementShape);
			hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
			
			const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
			
			hqSceneElement->setPosition(elementPosition);
			carouselLayer->addChild(hqSceneElement);
			buttons.push_back(hqSceneElement);
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
		_contentIcons.push_back(buttons);
	}
	
	//we have all carousels in a vector, time to resize the scrollview and add them one by one
	bool isGroupHQ = _hqCategory == ConfigStorage::kGroupHQName;
	
	const Size& vScrollFrameSize = Size(visibleSize.width - sideMargin * 2, visibleSize.height - (isGroupHQ ? 200.0f : 500.0f) - (_showingMessagingLayer ? 350 : 0));
	
	_contentScrollview = cocos2d::ui::ScrollView::create();
	_contentScrollview->setContentSize(vScrollFrameSize);
	_contentScrollview->setPosition(Point(sideMargin, (isGroupHQ ? 0 : 300) + (_showingMessagingLayer ? 350 : 0)));
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
		
		cocos2d::Layer *carouselTitle = HQScene2CarouselTitle::createForCarousel(HQDataObjectManager::getInstance()->getHQDataObjectForKey(_hqCategory)->getHqCarousels()[carouselIndex]);
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
	
	_contentNode->addChild(_contentScrollview);
	
	//add gradient on top of scrollView
	
	cocos2d::Sprite* gradient = createGradientForScrollView(_contentScrollview->getContentSize().width);
	gradient->setPosition(_contentScrollview->getContentSize().width / 2 + _contentScrollview->getPosition().x, _contentScrollview->getPosition().y + _contentScrollview->getContentSize().height);
	_contentNode->addChild(gradient);
	
	//add group hq logo if necessary
	
	if(isGroupHQ)
	{
		addGroupHQLogo();
		_navBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		ui::Button* backButton = ui::Button::create("res/navigation/back_button.png");
		backButton->setAnchorPoint(Vec2(-0.25, 1.25));
		backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
		backButton->addTouchEventListener([&](Ref* pSender, ui::Button::TouchEventType eType){
			if(eType == ui::Button::TouchEventType::ENDED)
			{
				if(TutorialController::getInstance()->isTutorialActive() &&  TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUGroupHQContent)
				{
					TutorialController::getInstance()->endTutorial();
				}
				
				HQHistoryManager::getInstance()->popHQ();
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
			}
		 });
		 _contentNode->addChild(backButton,1);
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
		const std::string& secondsString = AnalyticsProperties::getInstance()->getStoredContentItemProperties().at(AnalyticsProperties::kSecondsInContentKey);
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
		const std::string &groupHQLogoUrl = HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName)->getGroupLogo();
		
		this->removeChild(this->getChildByName(kGroupLogoName));
		
		auto groupLogo = RemoteImageSprite::create();
		groupLogo->initWithUrlAndSizeWithoutPlaceholder(groupHQLogoUrl, ConfigStorage::getInstance()->getGroupHQLogoSize());
		groupLogo->setScale(0.8);
		groupLogo->setPosition(visibleSize.width / 2, visibleSize.height - groupLogo->getBoundingBox().size.height * 0.55);
		groupLogo->setName(kGroupLogoName);
		_contentNode->addChild(groupLogo);
	}
}

void ContentFeedHQScene::onSizeChanged()
{
	bool wasPortrait = _isPortrait;
	Super::onSizeChanged();
	if(wasPortrait == _isPortrait) // orientation hasnt changed
	{
		return;
	}
	float scrollPerc = 0.1f;
	if(_contentScrollview)
	{
		scrollPerc = _contentScrollview->getScrolledPercentVertical();
	}
	_contentNode->removeAllChildren();
	_contentNode->setContentSize(this->getContentSize());
	createContentScrollview();
	
	if(!isnan(scrollPerc))
	{
		Director::getInstance()->getScheduler()->schedule([=](float deltat)
		{
			 _contentScrollview->scrollToPercentVertical(scrollPerc, 0, true);
		}, this, 0, 0, 0, false, "scroll");
	}
}

void ContentFeedHQScene::onTutorialStateChanged(const std::string& stateId)
{
	if(stateId == TutorialController::kFTUVideoHQContent || stateId == TutorialController::kFTUGameHQContent || stateId == TutorialController::kFTUGroupHQContent)
	{
		if(_contentIcons.size() > 0 && _contentIcons.at(0).size() > 0)
		{
			HQSceneElement* hqSceneElement = _contentIcons.at(0).at(0);
			hqSceneElement->setLocalZOrder(1);
			hqSceneElement->removeChildByName("tutHand");
			Sprite* hand = Sprite::create("res/tutorial/Pointer.png");
			hand->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			hand->setName(kTutHandName);
			hand->setRotation(-15.0f);
			hand->setPosition(hqSceneElement->getContentSize() / 2);
			hqSceneElement->addChild(hand,1);
			hand->setScale((ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory).height * 0.5f) / hand->getContentSize().height);
			hand->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(1.0f, Vec2(hqSceneElement->getContentSize().width * 0.05f, -hqSceneElement->getContentSize().height * 0.1f)), MoveBy::create(1.0f, Vec2(-hqSceneElement->getContentSize().width * 0.05f, hqSceneElement->getContentSize().height * 0.1f)))));
			hand->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, hand->getScale() * 1.2f), ScaleTo::create(1.0f, hand->getScale()))));
		}
	}
	else
	{
		if(_contentIcons.size() > 0 && _contentIcons.at(0).size() > 0)
		{
			HQSceneElement* hqSceneElement = _contentIcons.at(0).at(0);
			hqSceneElement->removeChildByName(kTutHandName);
		}
	}
}

NS_AZOOMEE_END
