#include "HQScene2.h"
#include "HQSceneElement.h"
#include "HQDataProvider.h"
#include "HQScene2ElementPositioner.h"
#include "HQScene2PlaceHolderCreator.h"
#include "HQScene2CarouselTitle.h"
#include "ContentHistoryManager.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float HQScene2::kSideMarginSize = 20.0f;
const float HQScene2::kSpaceAboveCarousel = 200.0f;
const int HQScene2::kUnitsOnScreen = 4;
const float HQScene2::kContentItemMargin = 20.0f;
const float HQScene2::kSpaceForPrivacyPolicy = 100.0f;
const std::string& HQScene2::kScrollViewName = "scrollview";

bool HQScene2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        _visibleSize.width -= 200;
        _origin.x += 100;
    }
    
    return true;
}

void HQScene2::setHQCategory(const std::string &hqCategory)
{
    _hqCategory = hqCategory;
    this->setName(_hqCategory);     //this is to keep legacy code compatible
}

void HQScene2::startBuildingScrollView()
{
    _contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory);
    _unitWidth = (_visibleSize.width - 2 * kSideMarginSize) / kUnitsOnScreen;
    _unitMultiplier = calculateUnitMultiplier();
    
    if(_hqCategory == "" || this->getChildByName(kScrollViewName)) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        return;
    }
    
    _carouselStorage.clear();
    float totalHeightOfCarousels = 0;
    
    for(int rowIndex = 0; rowIndex < HQDataProvider::getInstance()->getNumberOfRowsForHQ(_hqCategory); rowIndex++)
    {
        cocos2d::LayerColor* carouselLayer = createNewCarousel();
        
        const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(_hqCategory, rowIndex);
        float lowestElementYPosition = 0;
        
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
            hqScene2PlaceHolderCreator.setMargin(kContentItemMargin);
            hqScene2PlaceHolderCreator.addPlaceHoldersToCarousel();
        }
        
        postSizeAndAlignCarousel(carouselLayer, lowestElementYPosition); //with the flexible sizing method, the contentSize of the carousel is not predictable, we need to do it after all elements are in place.
        totalHeightOfCarousels += carouselLayer->getContentSize().height + kSpaceAboveCarousel;
        _carouselStorage.push_back(carouselLayer);
    }
    
    //we have all carousels in a vector, time to resize the scrollview and add them one by one
    cocos2d::ui::ScrollView* scrollView = createScrollView();
    scrollView->setInnerContainerSize(cocos2d::Size(_visibleSize.width - 2 * kSideMarginSize, totalHeightOfCarousels + kSpaceForPrivacyPolicy));
    
    float lastCarouselPosition = scrollView->getInnerContainerSize().height;
    for(int carouselIndex = 0; carouselIndex < _carouselStorage.size(); carouselIndex++)
    {
        lastCarouselPosition -= kSpaceAboveCarousel;
        
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
    
    showPostContentCTA();
}

void HQScene2::showPostContentCTA()
{
    if(!ContentHistoryManager::getInstance()->getReturnedFromContent() || _hqCategory == ConfigStorage::kGroupHQName)
    {
        return;
    }
    
    ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    HQContentItemObjectRef lastContent = ContentHistoryManager::getInstance()->getLastOpenedContent();
    if(lastContent == nullptr)
    {
        return;
    }
    std::vector<HQCarouselObjectRef> hqCarousels = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(this->getName())->getHqCarousels();
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
                DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(_hqCategory + ".json", randomContent->getJSONRepresentationOfStructure());
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
    Size vScrollFrameSize = Size(_visibleSize.width - kSideMarginSize * 2, _visibleSize.height - 300.0f);
    
    cocos2d::ui::ScrollView *vScrollView = cocos2d::ui::ScrollView::create();
    vScrollView->setContentSize(vScrollFrameSize);
    vScrollView->setPosition(Point(_origin.x + kSideMarginSize, _origin.y));
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
    auto hqSceneElement = HQSceneElement::create();
    hqSceneElement->setCategory(_hqCategory);
    hqSceneElement->setItemData(itemData);
    hqSceneElement->setElementRow(rowNumber);
    hqSceneElement->setElementIndex(elementIndex);
    hqSceneElement->setMargin(kContentItemMargin);
    hqSceneElement->setManualSizeMultiplier(_unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
    
    hqSceneElement->addHQSceneElement();
    
    return hqSceneElement;
}

void HQScene2::addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView)
{
    
}

cocos2d::LayerColor* HQScene2::createNewCarousel()
{
    cocos2d::LayerColor*carouselLayer = cocos2d::LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), _visibleSize.width - 2 * kSideMarginSize, 0);
    
    return carouselLayer;
}

void HQScene2::postSizeAndAlignCarousel(cocos2d::Node* carouselLayer, float lowestElementY)
{
    carouselLayer->setContentSize(Size(_visibleSize.width - 2 * kSideMarginSize, -lowestElementY));
    
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


NS_AZOOMEE_END
