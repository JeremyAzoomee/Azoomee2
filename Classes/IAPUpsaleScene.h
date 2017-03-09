#ifndef __ONBOARDINGSUCCESSSCENE_SCENE_H__
#define __ONBOARDINGSUCCESSSCENE_SCENE_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class IAPUpsaleScene : public Layer, public ElectricDreamsButtonDelegate
{
private:
    
    Size visibleSize;
    Vec2 origin;
    
    void addVisualElementsToScene();
    void addButtonsToScene();
    void addLabelsToLayer();
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingSuccessScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
};

#endif
