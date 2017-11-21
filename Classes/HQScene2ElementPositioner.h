#ifndef Azoomee_HQScene2ElementPositioner_h
#define Azoomee_HQScene2ElementPositioner_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQScene2ElementPositioner : public cocos2d::Ref
{
public:
    void setElement(cocos2d::Node* element);
    void setCarouselLayer(cocos2d::Node* carouselLayer);
    void setHighlightData(cocos2d::Vec2 highlightData);
    void setBaseUnitSize(cocos2d::Size unitSize);
    
    cocos2d::Point positionHQSceneElement();
    
private:
    cocos2d::Node* _element;
    cocos2d::Node* _carouselLayer;
    cocos2d::Vec2 _highlightData;
    cocos2d::Size _unitSize;
    
    bool isElementCovered(cocos2d::Point position);
};

NS_AZOOMEE_END

#endif
