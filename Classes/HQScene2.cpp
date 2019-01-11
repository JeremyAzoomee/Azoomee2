#include "HQScene2.h"
#include "HQSceneElement.h"
#include "HQDataProvider.h"
#include "HQScene2ElementPositioner.h"
#include "HQScene2PlaceHolderCreator.h"
#include "HQScene2CarouselTitle.h"
#include "HQHistoryManager.h"
#include "ContentHistoryManager.h"
#include "DynamicNodeHandler.h"
#include "HQSceneArtsApp.h"
#include "MeHQ.h"
#include "OfflineHubBackButton.h"
#include "RecentlyPlayedManager.h"
#include "ArtAppDelegate.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <AzoomeeCommon/Utils/IosNativeFunctionsSingleton.h>
#endif


using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float HQScene2::kSpaceForPrivacyPolicy = 100.0f;
const std::string& HQScene2::kScrollViewName = "scrollview";
const std::string& HQScene2::kGroupLogoName = "groupLogo";
const std::string& HQScene2::kArtScrollViewName = "ArtScrollView";
const float HQScene2::kGroupContentItemImagePlaceholder = 150.0f;

Scene* HQScene2::createSceneForOfflineArtsAppHQ()
{
    auto scene = Scene::create();
    auto layer = HQScene2::create();
    scene->addChild(layer);
    
    //if created as a scene, and not as a layer, we are in offline mode, and we are using scene only for art app, so adding initial lines:
    layer->setName(ConfigStorage::kArtAppHQName);
    
    auto offlineArtsAppScrollView = HQSceneArtsApp::create();
    offlineArtsAppScrollView->setName(kArtScrollViewName);
    offlineArtsAppScrollView->setOriginPosition(Director::getInstance()->getVisibleOrigin() + Vec2(0,Director::getInstance()->getVisibleSize().height * 0.80f));
    offlineArtsAppScrollView->setRows(2);
	offlineArtsAppScrollView->setShowPrivacyButton(false);
    layer->addChild(offlineArtsAppScrollView);
    
    auto offlineHubBackButton = OfflineHubBackButton::create();
    offlineHubBackButton->setPosition(Point(100, Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height - 250));
    layer->addChild(offlineHubBackButton);
    
    return scene;
}

bool HQScene2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Vec2(0,0);//Director::getInstance()->getVisibleOrigin();
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        _visibleSize.width -= 200;
        _origin.x += 100;
    }
    
    return true;
}

void HQScene2::onEnter()
{
	TutorialController::getInstance()->registerDelegate(this);
	Layer::onEnter();
}

void HQScene2::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
	Layer::onExit();
}

void HQScene2::setHQCategory(const std::string &hqCategory)
{
    _hqCategory = hqCategory;
    this->setName(_hqCategory);     //this is to keep legacy code compatible
}

void HQScene2::startBuildingScrollView()
{
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Vec2(0,0);//Director::getInstance()->getVisibleOrigin();
    
    if(_visibleSize.width >= _visibleSize.height)
    {
        _orientation = 0;
    }
    else
    {
        _orientation = 1;
    }
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        if(_orientation)
        {
            _visibleSize.height -= 100;
        }
        else
        {
            _visibleSize.width -= 200;
            _origin.x += 100;
        }
    }
    
    float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
    float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
    float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    _contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory);
    _unitWidth = (_visibleSize.width - 2 * sideMargin - contentItemMargin / 2.0f) / unitsOnScreen;
    _unitMultiplier = calculateUnitMultiplier();
    
    if(_hqCategory == "" || this->getChildByName(kScrollViewName)) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        return;
    }
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(startingReviewProcessRequired())
    {
        IosNativeFunctionsSingleton::getInstance()->startIosReviewProcess();
    }
#endif
    
    if(this->getName() == ConfigStorage::kMeHQName)
    {
        auto meHQ = this->getChildByName(ConfigStorage::kMeHQName);
        if(!meHQ)
        {
            auto meHQLayer = MeHQ::create();
            meHQLayer->setName(ConfigStorage::kMeHQName);
            this->addChild(meHQLayer);
        }
        return;
    }
    
    if(this->getName() == ConfigStorage::kArtAppHQName)
    {
        auto artsLayer = this->getChildByName(ConfigStorage::kArtAppHQName);
        if(!artsLayer)
        {
            auto offlineArtsAppScrollView = HQSceneArtsApp::create();
            offlineArtsAppScrollView->setName(ConfigStorage::kArtAppHQName);
            offlineArtsAppScrollView->setOriginPosition(_origin + Vec2(0,_visibleSize.height * (_orientation ? 0.85f : 0.7f)));
            offlineArtsAppScrollView->setRows(_orientation ? 5 : 2);
            this->addChild(offlineArtsAppScrollView);
        }
        return;
    }
    
    _carouselStorage.clear();
    float totalHeightOfCarousels = 0;
    
    addRecentlyPlayedCarousel();
    
    for(int rowIndex = 0; rowIndex < HQDataProvider::getInstance()->getNumberOfRowsForHQ(_hqCategory); rowIndex++)
    {
        const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(_hqCategory, rowIndex);
        float lowestElementYPosition = 0;
        
        cocos2d::LayerColor* carouselLayer = createNewCarousel();
        
        for(int elementIndex = 0; elementIndex < elementsForRow.size(); elementIndex++)
        {
            cocos2d::Layer* currentElement = createElementForCarousel(carouselLayer, elementsForRow.at(elementIndex), rowIndex, elementIndex);
            cocos2d::Vec2 elementShape = HQDataProvider::getInstance()->getHighlightDataForSpecificItem(_hqCategory, rowIndex, elementIndex);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(currentElement);
            hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner.setHighlightData(elementShape);
            hqScene2ElementPositioner.setBaseUnitSize(_contentItemSize * _unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            currentElement->setPosition(elementPosition);
            carouselLayer->addChild(currentElement);
            
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
            hqScene2PlaceHolderCreator.setBaseUnitSize(_contentItemSize * _unitMultiplier);
            hqScene2PlaceHolderCreator.setMargin(contentItemMargin);
            hqScene2PlaceHolderCreator.setUseWirePlaceholder(rowIndex == 0);
            hqScene2PlaceHolderCreator.addPlaceHoldersToCarousel();
        }
        
        postSizeAndAlignCarousel(carouselLayer, lowestElementYPosition); //with the flexible sizing method, the contentSize of the carousel is not predictable, we need to do it after all elements are in place.
        totalHeightOfCarousels += carouselLayer->getContentSize().height + spaceAboveCarousel;
        _carouselStorage.push_back(carouselLayer);
    }
    
    //we have all carousels in a vector, time to resize the scrollview and add them one by one
    cocos2d::ui::ScrollView* scrollView = createScrollView();
    scrollView->setInnerContainerSize(cocos2d::Size(_visibleSize.width - 2 * sideMargin, totalHeightOfCarousels + kSpaceForPrivacyPolicy));
    
    float lastCarouselPosition = scrollView->getInnerContainerSize().height;
    for(int carouselIndex = 0; carouselIndex < _carouselStorage.size(); carouselIndex++)
    {
        if(HQDataProvider::getInstance()->getElementsForRow(_hqCategory, carouselIndex).size() == 0)
        {
            continue;
        }
        
        lastCarouselPosition -= spaceAboveCarousel;
        
        cocos2d::Layer *carouselTitle = HQScene2CarouselTitle::createForCarousel(HQDataObjectStorage::getInstance()->getHQDataObjectForKey(_hqCategory)->getHqCarousels()[carouselIndex]);
        carouselTitle->setPosition(cocos2d::Vec2(scrollView->getContentSize().width / 2, lastCarouselPosition));
        scrollView->addChild(carouselTitle);
        
        cocos2d::Node* carousel = _carouselStorage.at(carouselIndex);
        lastCarouselPosition -= carousel->getContentSize().height;
        carousel->setPosition(0, lastCarouselPosition);
        scrollView->addChild(carousel);
    }
    
    //add privacy policy to the very end to keep COPPA happy
    
    lastCarouselPosition -= kSpaceForPrivacyPolicy;
    PrivacyLayer* privacyLayer = PrivacyLayer::createWithColor();
    privacyLayer->setCenterPosition(Vec2(scrollView->getContentSize().width / 2, lastCarouselPosition + privacyLayer->getContentSize().height));
    scrollView->addChild(privacyLayer);
    
    this->addChild(scrollView);
    
    //add gradient on top of scrollView
    
    cocos2d::Sprite* gradient = createGradientForScrollView(scrollView->getContentSize().width);
    gradient->setPosition(scrollView->getContentSize().width / 2 + scrollView->getPosition().x, scrollView->getPosition().y + scrollView->getContentSize().height - gradient->getContentSize().width / 2 + 5);
    this->addChild(gradient);
    
    //add group hq logo if necessary
    
    if(_hqCategory == ConfigStorage::kGroupHQName)
    {
        addGroupHQLogo();
    }
	
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
	
    //show post content cta if necessary
    
    //if(showingPostContentCTARequired())
    //{
    //    showPostContentCTA();
    //}
}

void HQScene2::rebuildScrollView()
{
    this->removeAllChildren();
    startBuildingScrollView();
}

void HQScene2::addRecentlyPlayedCarousel()
{
    //inject recently played row
    if(_hqCategory == ConfigStorage::kGroupHQName)
    {
        return;
    }
    
    const std::vector<HQContentItemObjectRef> recentContent = RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(_hqCategory);
    auto HQData = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(_hqCategory);
    auto carouselData = HQData->getHqCarousels();
    HQCarouselObjectRef recentContentCarousel = HQCarouselObject::create();
    recentContentCarousel->setTitle(ConfigStorage::kRecentlyPlayedCarouselName);
    bool carouselExists = false;
    for(auto carousel : carouselData)
    {
        if(carousel->getTitle() == ConfigStorage::kRecentlyPlayedCarouselName)
        {
            recentContentCarousel = carousel;
            carouselExists = true;
            break;
        }
    }
    if(!carouselExists)
    {
        HQData->addCarouselToHqFront(recentContentCarousel);
    }
    
    recentContentCarousel->removeAllItemsFromCarousel();
    for(const HQContentItemObjectRef& item : recentContent)
    {
        if(item)
        {
            if(!item->isEntitled() && ParentDataProvider::getInstance()->isPaidUser())
            {
                AnalyticsSingleton::getInstance()->lockedContentItemInRecentlyPlayedEvent(item);
            }
            else
            {
                recentContentCarousel->addContentItemToCarousel(item);
            }
        }
    }
}

bool HQScene2::startingReviewProcessRequired()
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

bool HQScene2::showingPostContentCTARequired()
{
    if(!ContentHistoryManager::getInstance()->getReturnedFromContent() || _hqCategory == ConfigStorage::kGroupHQName)
    {
        return false;
    }
    
    int secondsInContent = 0;
    try
    {
        std::string secondsString = AnalyticsProperties::getInstance()->getStoredContentItemProperties().at("SecondsInContent");
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

void HQScene2::showPostContentCTA()
{
    ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    HQContentItemObjectRef lastContent = ContentHistoryManager::getInstance()->getLastOpenedContent();
    std::string targetHQ = ConfigStorage::kGameHQName;
    if(lastContent->getType() == ConfigStorage::kContentTypeGame)
    {
        targetHQ = ConfigStorage::kVideoHQName;
    }
    
    std::vector<HQCarouselObjectRef> hqCarousels = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(targetHQ)->getHqCarousels();
    bool possibleContentFound = false;
    while(!possibleContentFound && hqCarousels.size() > 0) // look for available content in random carousel
    {
        int randCarouselIndex = rand()%hqCarousels.size();
        HQCarouselObjectRef randomCarousel = hqCarousels[randCarouselIndex];
        std::vector<HQContentItemObjectRef> carouselItems = randomCarousel->getContentItems();
        while(!possibleContentFound && carouselItems.size() > 0) //look for random available content in carousel
        {
            int randIndex = rand()%carouselItems.size();
            HQContentItemObjectRef randomContent = carouselItems[randIndex];
            if(randomContent->isEntitled() && randomContent->getContentItemId() != lastContent->getContentItemId())
            {
                AnalyticsSingleton::getInstance()->registerCTASource("postContent", lastContent->getContentItemId(), lastContent->getType(), randomContent->getContentItemId());
                DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(lastContent->getType() + ".json", randomContent->getJSONRepresentationOfStructure());
                possibleContentFound = true;
            }
            else
            {
                carouselItems.erase(carouselItems.begin() + randIndex);
            }
        }
        //no different available content to recomend in the carousel
        if(!possibleContentFound)
        {
            hqCarousels.erase(hqCarousels.begin() + randCarouselIndex);
        }
    }
}

cocos2d::ui::ScrollView* HQScene2::createScrollView()
{
    float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    
    Size vScrollFrameSize = Size(_visibleSize.width - sideMargin * 2, _visibleSize.height - 300.0f);
    
    cocos2d::ui::ScrollView *vScrollView = cocos2d::ui::ScrollView::create();
    vScrollView->setContentSize(vScrollFrameSize);
    vScrollView->setPosition(Point(_origin.x + sideMargin, _origin.y));
    vScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    vScrollView->setTouchEnabled(true);
    vScrollView->setBounceEnabled(true);
    vScrollView->setScrollBarEnabled(false);
    vScrollView->setSwallowTouches(false);
    vScrollView->setName(kScrollViewName);
    
    return vScrollView;
}

Sprite* HQScene2::createGradientForScrollView(float scrollViewWith)
{
    Sprite* verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
    verticalScrollGradient->setAnchorPoint(Vec2(0.5, 1.0));
    verticalScrollGradient->setScaleX(scrollViewWith / verticalScrollGradient->getContentSize().width);
    verticalScrollGradient->setColor(Color3B::BLACK);
    
    return verticalScrollGradient;
}

cocos2d::Layer* HQScene2::createElementForCarousel(cocos2d::Node *toBeAddedTo, const HQContentItemObjectRef &itemData, int rowNumber, int elementIndex)
{
    float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    auto hqSceneElement = HQSceneElement::create();
    hqSceneElement->setCategory(_hqCategory);
    hqSceneElement->setItemData(itemData);
    hqSceneElement->setElementRow(rowNumber);
    hqSceneElement->setElementIndex(elementIndex);
    hqSceneElement->setMargin(contentItemMargin);
    hqSceneElement->setManualSizeMultiplier(_unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
    
    hqSceneElement->addHQSceneElement();
    
    return hqSceneElement;
}

cocos2d::LayerColor* HQScene2::createNewCarousel()
{
    float sideMargin = HQDataProvider::getInstance()->getSideMargin();

    cocos2d::LayerColor*carouselLayer = cocos2d::LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), _visibleSize.width - 2 * sideMargin, 0);
    
    return carouselLayer;
}

void HQScene2::postSizeAndAlignCarousel(cocos2d::Node* carouselLayer, float lowestElementY)
{
    float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    
    carouselLayer->setContentSize(Size(_visibleSize.width - 2 * sideMargin, -lowestElementY));
    
    for(cocos2d::Node* contentItem : carouselLayer->getChildren())
    {
        contentItem->setPositionY(contentItem->getPositionY() - lowestElementY);
    }
}

float HQScene2::calculateUnitMultiplier()
{
    if(_hqCategory == "")
    {
        return 1.0f;
    }
    
    return _unitWidth / _contentItemSize.width;
}

void HQScene2::addGroupHQLogo()
{
    if(HQHistoryManager::getInstance()->getGroupHQSourceId() != "")
    {
        const std::string &groupHQLogoUrl = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName)->getGroupLogo();
        
        this->removeChild(this->getChildByName(kGroupLogoName));
        
        auto groupLogo = RemoteImageSprite::create();
        groupLogo->initWithUrlAndSizeWithoutPlaceholder(groupHQLogoUrl, ConfigStorage::getInstance()->getGroupHQLogoSize());
        groupLogo->setScale(0.8);
        groupLogo->setPosition(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height - groupLogo->getBoundingBox().size.height * 0.55);
        groupLogo->setName(kGroupLogoName);
        this->addChild(groupLogo);
    }
}

//tutorial controls
void HQScene2::highlightFirstElement()
{
	if(_hqCategory == ConfigStorage::kGameHQName || _hqCategory == ConfigStorage::kVideoHQName || _hqCategory == ConfigStorage::kGroupHQName)
	{
		if(_carouselStorage.size() > 0)
		{
			for(auto carousel : _carouselStorage)
			{
				for(auto item : carousel->getChildren())
				{
					HQSceneElement* contentIcon = dynamic_cast<HQSceneElement*>(item);
					if(contentIcon)
					{
						contentIcon->setTouchDisabled(false);
						contentIcon->enableHighlight(true);
						return;
					}
				}
			}
		}
	}
}
void HQScene2::disableContent()
{
	if(_hqCategory == ConfigStorage::kGameHQName || _hqCategory == ConfigStorage::kVideoHQName || _hqCategory == ConfigStorage::kGroupHQName)
	{
		if(_carouselStorage.size() > 0)
		{
			for(auto carousel : _carouselStorage)
			{
				for(auto item : carousel->getChildren())
				{
					HQSceneElement* contentIcon = dynamic_cast<HQSceneElement*>(item);
					if(contentIcon)
					{
						contentIcon->setTouchDisabled(true);
					}
				}
			}
		}
	}
}
void HQScene2::enableContent()
{
	if(_hqCategory == ConfigStorage::kGameHQName || _hqCategory == ConfigStorage::kVideoHQName || _hqCategory == ConfigStorage::kGroupHQName)
	{
		if(_carouselStorage.size() > 0)
		{
			for(auto carousel : _carouselStorage)
			{
				for(auto item : carousel->getChildren())
				{
					HQSceneElement* contentIcon = dynamic_cast<HQSceneElement*>(item);
					if(contentIcon)
					{
						contentIcon->setTouchDisabled(false);
					}
				}
			}
		}
	}
}

void HQScene2::onTutorialStateChanged(const std::string& stateId)
{
	if(stateId == TutorialController::kFTUGameHQNav || stateId == TutorialController::kFTUVideoHQNav || stateId == TutorialController::kFTUGroupHQBack)
	{
		disableContent();
	}
	else if(stateId == TutorialController::kFTUGameHQContent)
	{
		if(_hqCategory == ConfigStorage::kGameHQName)
		{
			disableContent();
			highlightFirstElement();
		}
	}
	else if(stateId == TutorialController::kFTUVideoHQContent)
	{
		if(_hqCategory == ConfigStorage::kVideoHQName)
		{
			disableContent();
			highlightFirstElement();
		}
	}
	else if(stateId == TutorialController::kFTUGroupHQContent)
	{
		if(_hqCategory == ConfigStorage::kGroupHQName)
		{
			disableContent();
			highlightFirstElement();
		}
	}
	else
	{
		enableContent();
	}
}

NS_AZOOMEE_END
