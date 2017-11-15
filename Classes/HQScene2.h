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
    CREATE_FUNC(HQScene2);
    virtual bool init();
    void startBuildingScrollView();
    
    void setHQCategory(std::string hqCategory);
    std::string _hqCategory;

private:
    static const float _marginSize;
    static const int _unitsOnScreen;
    
    cocos2d::Vec2 _origin;
    cocos2d::Size _visibleSize;
    float _unitMultiplier = 0;
    
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);

    cocos2d::ui::ScrollView* createScrollView();
    cocos2d::Layer* addElementToCarousel(cocos2d::ui::ScrollView *toBeAddedTo, const HQContentItemObjectRef &itemData, int rowNumber, int elementIndex);
    
    //creating carousel elements
    cocos2d::Layer* createNewCarousel();
    float calculateUnitMultiplier();
};

NS_AZOOMEE_END

#endif
