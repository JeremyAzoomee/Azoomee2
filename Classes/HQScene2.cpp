//Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
//We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
//The listener works the same way, as with all other nodes.

#include "HQScene2.h"
#include "HQSceneElement.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float HQScene2::_marginSize = 50;
const int HQScene2::_unitsOnScreen = 4;

bool HQScene2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    _unitSize = calculateUnitSize();
    
    return true;
}

void HQScene2::setHQCategory(std::string hqCategory)
{
    _hqCategory = hqCategory;
    this->setName(_hqCategory);     //this is to keep legacy code compatible
}

void HQScene2::startBuildingScrollView()
{
    if(_hqCategory == "" || this->getChildByName("scrollView")) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        return;
    }
    
    cocos2d::ui::ScrollView* scrollView = createScrollView();
    float lastScrollViewPosition = 0;
    
    for(int j = 0; j < HQDataProvider::getInstance()->getNumberOfRowsForHQ(_hqCategory); j++)
    {
        cocos2d::Layer* carouselLayer = createNewCarousel();
        carouselLayer->setPosition(cocos2d::Point(0, lastScrollViewPosition - _marginSize - carouselLayer->getContentSize().height));
        scrollView->addChild(carouselLayer);
        
        const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(_hqCategory, j);
        
        for(int i = 0; i < elementsForRow.size(); i++)
        {
            
        }
    }
}

cocos2d::ui::ScrollView*  HQScene2::createScrollView()
{
    Size vScrollFrameSize = Size(_visibleSize.width - _marginSize * 2, _visibleSize.height - 200);
    
    cocos2d::ui::ScrollView *vScrollView = cocos2d::ui::ScrollView::create();
    vScrollView->setContentSize(vScrollFrameSize);
    vScrollView->setPosition(_origin);
    vScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    vScrollView->setTouchEnabled(true);
    vScrollView->setBounceEnabled(true);
    vScrollView->setInnerContainerSize(Size(_visibleSize.width, _visibleSize.height * 2));
    vScrollView->setScrollBarEnabled(false);
    vScrollView->setSwallowTouches(false);
    vScrollView->setName("scrollView");
    
    //addListenerToScrollView(vScrollView);
    
    this->addChild(vScrollView);
    
    return vScrollView;
}

void HQScene2::addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView)
{
    
}

cocos2d::Layer* HQScene2::createNewCarousel()
{
    cocos2d::Layer* carouselLayer = cocos2d::Layer::create();
    carouselLayer->setContentSize(Size(_visibleSize.width - _marginSize * 2, _unitSize.height));
    return carouselLayer;
}

cocos2d::Size HQScene2::calculateUnitSize()
{
    if(_hqCategory == "")
    {
        return cocos2d::Size(0,0);
    }
    
    cocos2d::Size baseSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(_hqCategory);
    float unitWidth = (_visibleSize.width - _marginSize * 2) / _unitsOnScreen;
    
    return baseSize * (baseSize.width / unitWidth);
}


NS_AZOOMEE_END
