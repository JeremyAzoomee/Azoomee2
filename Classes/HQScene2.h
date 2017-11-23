#ifndef __HQSCENE2_H__
#define __HQSCENE2_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class HQScene2 : public cocos2d::Layer
{
public:
    static const float _sideMarginSize;
    static const float _spaceAboveCarousel;
    static const int _unitsOnScreen;
    static const float _contentItemMargin;
    static const float _spaceForPrivacyPolicy;
    
    CREATE_FUNC(HQScene2);
    virtual bool init();
    void startBuildingScrollView();
    
    void setHQCategory(std::string hqCategory);
    std::string _hqCategory;

private:
    cocos2d::Vec2 _origin;
    cocos2d::Size _visibleSize;
    float _unitMultiplier = 0;
    float _unitWidth = 0;
    std::vector<cocos2d::Layer*> _carouselStorage;
    
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);

    cocos2d::ui::ScrollView* createScrollView();
    cocos2d::Layer* createElementForCarousel(cocos2d::Node *toBeAddedTo, const HQContentItemObjectRef &itemData, int rowNumber, int elementIndex);
    
    cocos2d::Sprite* createGradientForScrollView(float scrollViewWith);
    
    //creating carousel elements
    cocos2d::LayerColor* createNewCarousel();
    float calculateUnitMultiplier();
    void postSizeAndAlignCarousel(cocos2d::Node* carouselLayer, float lowestElementY);
};

NS_AZOOMEE_END

#endif
