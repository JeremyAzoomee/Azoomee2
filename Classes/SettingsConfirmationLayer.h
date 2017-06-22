#ifndef __SETTINGS_CONFIRMATION_LAYER_H__
#define __SETTINGS_CONFIRMATION_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"

USING_NS_CC;

#define CONFIRMATION_CONFIRMED_Z 300
#define CONFIRMATION_REJECT_Z 400
#define CONFIRMATION_IDLE_Z 200


class SettingsConfirmationLayer : public Layer
{
private:
    float layerHeight;
    float contentWidth;
    
    ui::ScrollView *scrollView;
    
    Label* detailsLabel;

    void addDetailsLabel();
    void addScrollView();
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(SettingsConfirmationLayer);

};

#endif
