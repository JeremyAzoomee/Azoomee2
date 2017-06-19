#ifndef __SETTINGS_CONFIRMATION_LAYER_H__
#define __SETTINGS_CONFIRMATION_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"

USING_NS_CC;

class SettingsConfirmationLayer : public Layer
{
private:
    float layerHeight;
    ui::ScrollView *scrollView;

    void addExitOrLogoutUIObjects();
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(SettingsConfirmationLayer);

};

#endif
