#ifndef __SETTINGS_KIDS_LAYER_H__
#define __SETTINGS_KIDS_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"

USING_NS_CC;

class SettingsKidsLayer : public Layer
{
private:
    Size visibleSize;
    Vec2 origin;
    
    float layerHeight;
    
    ui::ScrollView *scrollView;
    
    void addExitOrLogoutUIObjects();
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(SettingsKidsLayer);

};

#endif
