#include "HQScene2ElementPositioner.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void HQScene2ElementPositioner::setElement(cocos2d::Node* element)
{
    _element = element;
}

void HQScene2ElementPositioner::setCarouselLayer(cocos2d::Node* carouselLayer)
{
    _carouselLayer = carouselLayer;
}

void HQScene2ElementPositioner::setHighlightData(cocos2d::Vec2 highlightData)
{
    _highlightData = highlightData;
}

void HQScene2ElementPositioner::setBaseUnitSize(cocos2d::Size unitSize)
{
    _unitSize = unitSize;
}

cocos2d::Point HQScene2ElementPositioner::positionHQSceneElement()
{    
    //return when
    // - starting point is empty
    // - all covered points are empty (if larger than 1,1
    // - all covered points are visible on screen
    
    cocos2d::Point position = Point(0, -_highlightData.y * _unitSize.height);
    bool placeFound = false;
    
    while(!placeFound)
    {
        if(!isElementCovered(position, _carouselLayer))
        {
            return position;
        }
        
        position.x += _unitSize.width;
        if(position.x >= _carouselLayer->getContentSize().width - _element->getContentSize().width)
        {
            position.x = 0;
            position.y -= _unitSize.height;
        }
        
    }
    
    return cocos2d::Point(0,0);
}

bool HQScene2ElementPositioner::isElementCovered(cocos2d::Point position, cocos2d::Node *carouselElement)
{
    for(cocos2d::Node* contentItemElement : carouselElement->getChildren())
    {
        if(contentItemElement->getBoundingBox().containsPoint(position)) return true;
    }
    
    return false;
}

NS_AZOOMEE_END
