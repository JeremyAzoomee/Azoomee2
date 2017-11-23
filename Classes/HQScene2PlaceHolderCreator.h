#ifndef Azoomee_HQScene2PlaceHolderCreator_h
#define Azoomee_HQScene2PlaceHolderCreator_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQScene2PlaceHolderCreator : public cocos2d::Ref
{
public:
    void setCarouselLayer(cocos2d::Node* carouselLayer);
    void setBaseUnitSize(cocos2d::Size unitSize);
    void setLowestElementYPosition(float lowestElementYPosition);
    void setMargin(float margin);
    
    void addPlaceHoldersToCarousel();
    
private:
    cocos2d::Node* _carouselLayer;
    cocos2d::Size _unitSize;
    float _lowestElementYPosition = 0.0f;
    float _margin = 10.0f;
    
    cocos2d::Layer* createPlaceHolderLayer();
};

NS_AZOOMEE_END

#endif
