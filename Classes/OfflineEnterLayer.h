#ifndef __OFFLINEENTER_LAYER_H__
#define __OFFLINEENTER_LAYER_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"
#include "OfflineChecker.h"

class OfflineEnterLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate, public OfflineCheckerDelegate
{
public:
    CREATE_FUNC(OfflineEnterLayer);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void buttonPressed(ElectricDreamsButton* button);
    void connectivityStateChanged(bool online);
    void onEnterTransitionDidFinish();
    
private:
    void createEnterButton();
    void createBlackBackground();
    void createText();
    void createTextForNoUser();
};

#endif
