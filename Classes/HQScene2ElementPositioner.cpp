#include "HQScene2ElementPositioner.h"
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
    cocos2d::Point position = Point(0, -_highlightData.y * _unitSize.height);
    
    while(true)
    {
        if(!isElementCovered(Point(position.x + _unitSize.width /2, position.y + _unitSize.height / 2)))
        {
            return position;
        }
        
        position.x += _unitSize.width;
        if(position.x > _carouselLayer->getContentSize().width - _element->getContentSize().width)
        {
            position.x = 0;
            position.y -= _unitSize.height;
        }
    }
}

bool HQScene2ElementPositioner::isElementCovered(cocos2d::Point position)
{
    for(int highlightX = 0; highlightX < _highlightData.x; highlightX++)
    {
        for(int highlightY = 0; highlightY < _highlightData.y; highlightY++)
        {
            cocos2d::Point checkPosition = Point(position.x + (highlightX * _unitSize.width), position.y + (highlightY * _unitSize.height));
            
            for(cocos2d::Node* contentItemElement : _carouselLayer->getChildren())
            {
                if(contentItemElement->getBoundingBox().containsPoint(checkPosition))
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

NS_AZOOMEE_END
