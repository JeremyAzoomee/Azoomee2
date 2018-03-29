//
//  MeHQ.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/03/2018.
//

#include "MeHQ.h"
#include "HQScene2.h"
#include "HQSceneElement.h"
#include "SceneManagerScene.h"
#include "RecentlyPlayedManager.h"
#include "HQScene2ElementPositioner.h"
#include "FavouritesManager.h"
#include "HQDataProvider.h"
#include "HQScene2CarouselTitle.h"
#include "PrivacyAndTermsLayer.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->setPosition(Director::getInstance()->getVisibleOrigin());
    
    _contentScrollView = cocos2d::ui::ScrollView::create();
    _contentScrollView->setContentSize(Size(this->getContentSize().width * 0.75f, this->getContentSize().height * 0.5));
    _contentScrollView->setPosition(Point(0,0));
    _contentScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _contentScrollView->setTouchEnabled(true);
    _contentScrollView->setBounceEnabled(true);
    _contentScrollView->setScrollBarEnabled(false);
    _contentScrollView->setSwallowTouches(false);
    _contentScrollView->setInnerContainerSize(Size(this->getContentSize().width * 0.75f, 0));
    
    float totalHeightOfCarousels = 0;
    
    _artScrollView = HQSceneArtsApp::create();
    _artScrollView->setShowPrivacyButton(false);
    _artScrollView->setSize(Size(_contentScrollView->getContentSize().width, 0));
    _artScrollView->setOriginPosition(Vec2(0,this->getContentSize().height * 0.8f));
    _artScrollView->setRows(1);
    this->addChild(_artScrollView);
    
    setHQCarouselData();
    
    std::vector<cocos2d::Layer*> carouselStorage;
    
    for(int rowIndex = 0; rowIndex < HQDataProvider::getInstance()->getNumberOfRowsForHQ(_hqCategory); rowIndex++)
    {
        const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(_hqCategory, rowIndex);
        float lowestElementYPosition = 0;
        
        cocos2d::LayerColor* carouselLayer = cocos2d::LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), _contentScrollView->getContentSize().width, 0);
        
        for(int elementIndex = 0; elementIndex < elementsForRow.size(); elementIndex++)
        {
            auto currentElement = HQSceneElement::create();
            currentElement->setCategory(_hqCategory);
            currentElement->setItemData(elementsForRow.at(elementIndex));
            currentElement->setElementRow(rowIndex);
            currentElement->setElementIndex(elementIndex);
            currentElement->setMargin(HQScene2::kContentItemMargin);
            currentElement->setManualSizeMultiplier(1.1); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
            currentElement->addHQSceneElement();
            
            cocos2d::Vec2 elementShape = HQDataProvider::getInstance()->getHighlightDataForSpecificItem(_hqCategory, rowIndex, elementIndex);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(currentElement);
            hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner.setHighlightData(elementShape);
            hqScene2ElementPositioner.setBaseUnitSize(Size(590,442) * 1.1);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            currentElement->setPosition(elementPosition);
            carouselLayer->addChild(currentElement);
            
            if(elementPosition.y < lowestElementYPosition)
            {
                lowestElementYPosition = elementPosition.y;
            }
        }
        
        carouselLayer->setContentSize(Size(_contentScrollView->getContentSize().width, -lowestElementYPosition));
        
        for(cocos2d::Node* contentItem : carouselLayer->getChildren())
        {
            contentItem->setPositionY(contentItem->getPositionY() - lowestElementYPosition);
        }
        
        totalHeightOfCarousels += carouselLayer->getContentSize().height + HQScene2::kSpaceAboveCarousel;
        carouselStorage.push_back(carouselLayer);
    }
    
    //we have all carousels in a vector, time to resize the scrollview and add them one by one
    _contentScrollView->setInnerContainerSize(Size(_contentScrollView->getContentSize().width, totalHeightOfCarousels));
    
    float lastCarouselPosition = _contentScrollView->getInnerContainerSize().height;
    for(int carouselIndex = 0; carouselIndex < carouselStorage.size(); carouselIndex++)
    {
        if(HQDataProvider::getInstance()->getElementsForRow(_hqCategory, carouselIndex).size() == 0)
        {
            continue;
        }
        
        lastCarouselPosition -= HQScene2::kSpaceAboveCarousel;
        
        cocos2d::Layer *carouselTitle = HQScene2CarouselTitle::createForCarousel(HQDataObjectStorage::getInstance()->getHQDataObjectForKey(_hqCategory)->getHqCarousels()[carouselIndex]);
        carouselTitle->setPosition(cocos2d::Vec2(_contentScrollView->getContentSize().width / 2, lastCarouselPosition));
        _contentScrollView->addChild(carouselTitle);
        
        cocos2d::Node* carousel = carouselStorage.at(carouselIndex);
        lastCarouselPosition -= carousel->getContentSize().height;
        carousel->setPosition(0, lastCarouselPosition);
        _contentScrollView->addChild(carousel);
    }
    
    this->addChild(_contentScrollView);
    
    _oomeeMakerButton = ui::Button::create("res/artapp/popup_bg.png");
    _oomeeMakerButton->setScale9Enabled(true);
    _oomeeMakerButton->setZoomScale(0);
    _oomeeMakerButton->setContentSize(Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.5f));
    _oomeeMakerButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomeeMakerButton->setPosition(Vec2(this->getContentSize().width * 0.9f, this->getContentSize().height * 0.5f));
    _oomeeMakerButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(OomeeMakerEntryPointScene));
        }
    });
    this->addChild(_oomeeMakerButton);
    
    RemoteImageSprite* profileImage = RemoteImageSprite::create();
    profileImage->initWithUrlAndSizeWithoutPlaceholder(ChildDataProvider::getInstance()->getLoggedInChildAvatarId(), Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.5f), true);
    profileImage->setPosition(_oomeeMakerButton->getContentSize() / 2);
    profileImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomeeMakerButton->addChild(profileImage);
    
    
    return true;
}

void MeHQ::onEnter()
{
    Super::onEnter();
}

void MeHQ::setHQCarouselData()
{
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
    const std::vector<HQContentItemObjectRef> recentContentGames = RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kGameHQName);
    for(const HQContentItemObjectRef& item : recentContentGames)
    {
        if(item)
        {
            recentContentCarousel->addContentItemToCarousel(item);
        }
    }
    const std::vector<HQContentItemObjectRef> recentContentVideos = RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kVideoHQName);
    for(const HQContentItemObjectRef& item : recentContentVideos)
    {
        if(item)
        {
            recentContentCarousel->addContentItemToCarousel(item);
        }
    }
    
    const std::vector<HQContentItemObjectRef> favContent = FavouritesManager::getInstance()->getFavouriteContent();
    HQCarouselObjectRef favContentCarousel = HQCarouselObject::create();
    favContentCarousel->setTitle("FAVOURITES");
    carouselExists = false;
    for(auto carousel : carouselData)
    {
        if(carousel->getTitle() == "FAVOURITES")
        {
            favContentCarousel = carousel;
            carouselExists = true;
            break;
        }
    }
    if(!carouselExists)
    {
        HQData->addCarouselToHqFront(favContentCarousel);
    }
    
    favContentCarousel->removeAllItemsFromCarousel();
    for(const HQContentItemObjectRef& item : favContent)
    {
        if(item)
        {
            favContentCarousel->addContentItemToCarousel(item);
        }
    }
     
}

Layer* MeHQ::createContentCarousel(const std::vector<HQContentItemObjectRef>& content)
{
    Layer* contentLayer = Layer::create();
    contentLayer->setContentSize(Size(_contentScrollView->getContentSize().width, 0));
    
    float lowestElementYPosition = 0;
    for(const auto& item : content)
    {
        auto hqSceneElement = HQSceneElement::create();
        hqSceneElement->setCategory(ConfigStorage::kGameHQName);
        hqSceneElement->setItemData(item);
        hqSceneElement->setElementRow(0);
        hqSceneElement->setElementIndex(0);
        hqSceneElement->setMargin(HQScene2::kContentItemMargin);
        hqSceneElement->setManualSizeMultiplier(1); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
        hqSceneElement->addHQSceneElement();
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(hqSceneElement);
        hqScene2ElementPositioner.setCarouselLayer(contentLayer);
        hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
        hqScene2ElementPositioner.setBaseUnitSize(Size(590,442));
        
        Point position = hqScene2ElementPositioner.positionHQSceneElement();
        
        hqSceneElement->setPosition(position);
        contentLayer->addChild(hqSceneElement);
        
        if(position.y < lowestElementYPosition)
        {
            lowestElementYPosition = position.y;
        }
    }
    
    contentLayer->setContentSize(Size(contentLayer->getContentSize().width, -lowestElementYPosition));
    
    return contentLayer;
}

NS_AZOOMEE_END
