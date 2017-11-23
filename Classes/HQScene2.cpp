#include "HQScene2.h"
#include "HQSceneElement.h"
#include "HQDataProvider.h"
#include "HQScene2ElementPositioner.h"
#include "HQScene2PlaceHolderCreator.h"
#include "HQScene2CarouselTitle.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float HQScene2::_sideMarginSize = 20.0f;
const float HQScene2::_spaceAboveCarousel = 200.0f;
const int HQScene2::_unitsOnScreen = 4;
const float HQScene2::_contentItemMargin = 20.0f;
const float HQScene2::_spaceForPrivacyPolicy = 100.0f;

bool HQScene2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void HQScene2::setHQCategory(std::string hqCategory)
{
    _hqCategory = hqCategory;
    this->setName(_hqCategory);     //this is to keep legacy code compatible
    _unitWidth = (_visibleSize.width - 2 * _sideMarginSize) / _unitsOnScreen;
    _unitMultiplier = calculateUnitMultiplier();
}

void HQScene2::startBuildingScrollView()
{
    if(_hqCategory == "" || this->getChildByName("scrollView")) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        return;
    }
    
    _carouselStorage.clear();
    float totalHeightOfCarousels = 0;
    
    for(int j = 0; j < HQDataProvider::getInstance()->getNumberOfRowsForHQ(_hqCategory); j++)
    {
        cocos2d::LayerColor* carouselLayer = createNewCarousel();
        
        const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(_hqCategory, j);
        float lowestElementYPosition = 0;
        
        for(int i = 0; i < elementsForRow.size(); i++)
        {
            cocos2d::Layer* currentElement = createElementForCarousel(carouselLayer, elementsForRow.at(i), j, i);
            cocos2d::Vec2 elementShape = HQDataProvider::getInstance()->getHighlightDataForSpecificItem(_hqCategory, j, i);
            
            HQScene2ElementPositioner* hqScene2ElementPositioner = new HQScene2ElementPositioner();
            hqScene2ElementPositioner->setElement(currentElement);
            hqScene2ElementPositioner->setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner->setHighlightData(elementShape);
            hqScene2ElementPositioner->setBaseUnitSize(ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory) * _unitMultiplier);
            
            cocos2d::Point elementPosition = hqScene2ElementPositioner->positionHQSceneElement();
            
            currentElement->setPosition(elementPosition);
            carouselLayer->addChild(currentElement);
            
            if(elementPosition.y < lowestElementYPosition)
            {
                lowestElementYPosition = elementPosition.y;
            }
        }
        
        //Filling up empty spaces with placeholders (Design requirement)
        
        HQScene2PlaceHolderCreator* hqScene2PlaceHolderCreator = new HQScene2PlaceHolderCreator();
        hqScene2PlaceHolderCreator->setLowestElementYPosition(lowestElementYPosition);
        hqScene2PlaceHolderCreator->setCarouselLayer(carouselLayer);
        hqScene2PlaceHolderCreator->setBaseUnitSize(ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory) * _unitMultiplier);
        hqScene2PlaceHolderCreator->setMargin(_contentItemMargin);
        hqScene2PlaceHolderCreator->addPlaceHoldersToCarousel();
        
        postSizeAndAlignCarousel(carouselLayer, lowestElementYPosition); //with the flexible sizing method, the contentSize of the carousel is not predictable, we need to do it after all elements are in place.
        totalHeightOfCarousels += carouselLayer->getContentSize().height + _spaceAboveCarousel;
        _carouselStorage.push_back(carouselLayer);
    }
    
    //we have all carousels in a vector, time to resize the scrollview and add them one by one
    cocos2d::ui::ScrollView* scrollView = createScrollView();
    scrollView->setInnerContainerSize(cocos2d::Size(Director::getInstance()->getVisibleSize().width - 2 * _sideMarginSize, totalHeightOfCarousels + _spaceForPrivacyPolicy));
    
    float lastCarouselPosition = scrollView->getInnerContainerSize().height;
    for(int i = 0; i < _carouselStorage.size(); i++)
    {
        lastCarouselPosition -= _spaceAboveCarousel;
        
        const std::string &title = HQDataProvider::getInstance()->getTitleForRow(_hqCategory, i);
        cocos2d::Layer *carouselTitle = HQScene2CarouselTitle::createWithText(title);
        carouselTitle->setPosition(cocos2d::Vec2(scrollView->getContentSize().width / 2, lastCarouselPosition));
        scrollView->addChild(carouselTitle);
        
        cocos2d::Node* carousel = _carouselStorage.at(i);
        lastCarouselPosition -= carousel->getContentSize().height;
        carousel->setPosition(0, lastCarouselPosition);
        scrollView->addChild(carousel);
    }
    
    //add privacy policy to the very end to keep COPPA happy
    
    lastCarouselPosition -= _spaceForPrivacyPolicy;
    PrivacyLayer* privacyLayer = PrivacyLayer::createWithColor();
    privacyLayer->setCenterPosition(Vec2(scrollView->getContentSize().width / 2, lastCarouselPosition + privacyLayer->getContentSize().height));
    scrollView->addChild(privacyLayer);
    
    this->addChild(scrollView);
    
    //add gradient on top of scrollView
    
    cocos2d::Sprite* gradient = createGradientForScrollView(scrollView->getContentSize().width);
    gradient->setPosition(scrollView->getContentSize().width / 2 + scrollView->getPosition().x, scrollView->getPosition().y + scrollView->getContentSize().height - gradient->getContentSize().width / 2 + 5);
    this->addChild(gradient);
}

cocos2d::ui::ScrollView* HQScene2::createScrollView()
{
    Size vScrollFrameSize = Size(_visibleSize.width - _sideMarginSize * 2, _visibleSize.height - 300.0f);
    
    cocos2d::ui::ScrollView *vScrollView = cocos2d::ui::ScrollView::create();
    vScrollView->setContentSize(vScrollFrameSize);
    vScrollView->setPosition(Point(_origin.x + _sideMarginSize, _origin.y));
    vScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    vScrollView->setTouchEnabled(true);
    vScrollView->setBounceEnabled(true);
    vScrollView->setScrollBarEnabled(false);
    vScrollView->setSwallowTouches(false);
    vScrollView->setName("scrollView");
    
    //addListenerToScrollView(vScrollView);
    
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
    hqSceneElement->setMargin(_contentItemMargin);
    hqSceneElement->setManualSizeMultiplier(_unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
    
    hqSceneElement->addHQSceneElement();
    
    return hqSceneElement;
}

void HQScene2::addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView)
{
    
}

cocos2d::LayerColor* HQScene2::createNewCarousel()
{
    //cocos2d::Layer* carouselLayer = cocos2d::Layer::create();
    
    cocos2d::LayerColor*carouselLayer = cocos2d::LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), Director::getInstance()->getVisibleSize().width - 2 * _sideMarginSize, 0);
    
    return carouselLayer;
}

void HQScene2::postSizeAndAlignCarousel(cocos2d::Node* carouselLayer, float lowestElementY)
{
    carouselLayer->setContentSize(Size(Director::getInstance()->getVisibleSize().width - 2 * _sideMarginSize, -lowestElementY));
    
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
    
    cocos2d::Size baseSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory);
    
    return _unitWidth / baseSize.width;
}


NS_AZOOMEE_END
