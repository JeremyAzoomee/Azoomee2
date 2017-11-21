#include "HQScene2PlaceHolderCreator.h"
#include "HQScene2.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQScene2ElementPositioner.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void HQScene2PlaceHolderCreator::setCarouselLayer(cocos2d::Node* carouselLayer)
{
    _carouselLayer = carouselLayer;
}

void HQScene2PlaceHolderCreator::setBaseUnitSize(cocos2d::Size unitSize)
{
    _unitSize = unitSize;
}

void HQScene2PlaceHolderCreator::setLowestElementYPosition(float lowestElementYPosition)
{
    _lowestElementYPosition = lowestElementYPosition;
}

void HQScene2PlaceHolderCreator::addPlaceHoldersToCarousel()
{
    cocos2d::Point position = Point(0, -_unitSize.height);
    
    while(true)
    {
        cocos2d::Layer* placeHolder = createPlaceHolderLayer();
        
        HQScene2ElementPositioner* hqScene2ElementPositioner = new HQScene2ElementPositioner();
        hqScene2ElementPositioner->setElement(placeHolder);
        hqScene2ElementPositioner->setBaseUnitSize(_unitSize);
        hqScene2ElementPositioner->setCarouselLayer(_carouselLayer);
        hqScene2ElementPositioner->setHighlightData(cocos2d::Vec2(1.0f,1.0f));
        
        cocos2d::Point placeHolderPosition = hqScene2ElementPositioner->positionHQSceneElement();
        
        if(placeHolderPosition.y < _lowestElementYPosition)
        {
            return;
        }
        else
        {
            placeHolder->setPosition(placeHolderPosition);
            _carouselLayer->addChild(placeHolder);
        }
    }
}

cocos2d::Layer* HQScene2PlaceHolderCreator::createPlaceHolderLayer()
{
    cocos2d::Layer* placeHolderLayer = cocos2d::Layer::create();
    placeHolderLayer->setContentSize(_unitSize);
    
    cocos2d::LayerColor* visual = cocos2d::LayerColor::create(cocos2d::Color4B(186, 186, 186, 100), placeHolderLayer->getContentSize().width - 20, placeHolderLayer->getContentSize().height - 20);
    visual->setPosition(Vec2(10,10));
    placeHolderLayer->addChild(visual);
    
    return placeHolderLayer;
}

NS_AZOOMEE_END
