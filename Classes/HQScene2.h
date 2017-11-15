#ifndef __HQSCENE_H__
#define __HQSCENE_H__

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
    cocos2d::Size _unitSize;
    
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);

    cocos2d::ui::ScrollView* createScrollView();
    
    //creating carousel elements
    cocos2d::Layer* createNewCarousel();
    cocos2d::Size calculateUnitSize();
};

NS_AZOOMEE_END

#endif
