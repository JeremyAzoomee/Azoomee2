#ifndef __ONBOARDINGSUCCESSSCENE_SCENE_H__
#define __ONBOARDINGSUCCESSSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include "ElectricDreamsButton.h"

USING_NS_CC;

class OnboardingSuccessScene : public Layer, public ElectricDreamsButtonDelegate
{
private:
    
    Size visibleSize;
    Vec2 origin;
    
    ElectricDreamsButton* oomeeButton;
    ElectricDreamsButton* startTrial;

    void setupScene();
    void addVisualElementsToScene();
    void addButtonsToScene();
    void addLabelsToLayer();
    
    void addOomeeToScreen();
    
    bool buttonHasBeenPressed;
    
    virtual void onEnterTransitionDidFinish();
    
    bool IAPEnabled;
    bool IAPSuccess;
    
public:
    static cocos2d::Scene* createScene();
    
    static cocos2d::Scene* createScene(bool IAPEnabled, bool IAPSuccess);
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingSuccessScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void callDelegateFunction(float dt);
    
};

#endif
