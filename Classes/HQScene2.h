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
    void startBuildingScrollViewBasedOnName();

private:
    cocos2d::Vec2 _origin;
    cocos2d::Size _visibleSize;
    
    cocos2d::Point startLocation;
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);
    
    cocos2d::ui::ScrollView* createVerticalScrollView(const std::string& hqTypeName);

    void createScrollView();
    
    //creating carousel elements
    cocos2d::Layer createNewLayer
};

NS_AZOOMEE_END

#endif
