#ifndef __SETTINGS_KIDS_LAYER_H__
#define __SETTINGS_KIDS_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"

USING_NS_CC;

class SettingsKidsLayer : public Layer
{
private:
    
    float layerHeight;
    bool swallowTouches;
    
    ui::ScrollView *scrollView;
    LayerColor* kidsCoverLayer;
    LayerColor* tabsCoverLayer;

    void addExitOrLogoutUIObjects();
    
    void createBlackCoverLayer();
    void addTabsCoverLayer();
    void addListenerToCoverLayer(Layer* listenerToLayer);
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(SettingsKidsLayer);
    
    void scrollToPosition(int ChildNumber);
    void scrollReset();

};

#endif
