#ifndef __SETTINGS_KIDS_LAYER_H__
#define __SETTINGS_KIDS_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

using namespace cocos2d;

#define IDLE_COVER_LAYER_Z_ORDER 190
#define SELECTED_COVER_LAYER_Z_ORDER 210
#define IDLE_KID_LAYER_Z_ORDER 200
#define SELECTED_KID_LAYER_Z_ORDER 220

class SettingsKidsLayer : public Layer
{
private:
    
    float layerHeight;
    bool swallowTouches;
    
    ui::ScrollView *scrollView;
    LayerColor* kidsCoverLayer;
    LayerColor* tabsCoverLayer;

    void addUIObjects();
    
    void createBlackCoverLayer(Size innerSize);
    void addTabsCoverLayer();
    void removeTabsCoverLayer();
    void addListenerToCoverLayer(Layer* listenerToLayer);
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(SettingsKidsLayer);
    
    void selectChildForTextInput(int ChildNumber);
    void selectChildForSharing(int ChildNumber);
    void scrollReset();

};

NS_AZOOMEE_END

#endif
