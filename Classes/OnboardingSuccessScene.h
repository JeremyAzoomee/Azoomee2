#ifndef __ONBOARDINGSUCCESSSCENE_SCENE_H__
#define __ONBOARDINGSUCCESSSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

USING_NS_CC;
using namespace Azoomee;


class OnboardingSuccessScene : public Layer, public ElectricDreamsButtonDelegate
{
private:
    
    cocos2d::Size visibleSize;
    Vec2 origin;
    
    Label* subTitleLabel;
    
    ElectricDreamsButton* oomeeButton;
    ElectricDreamsButton* startTrial;

    void setupScene();
    void addVisualElementsToScene();
    void addButtonsToScene();
    void addTitleLabelsToLayer();
    void addSubLabelsToLayer();
    
    void addOomeeToScreen();
    
    bool buttonHasBeenPressed;
    
    virtual void onEnterTransitionDidFinish();
    
    bool IAPSuccess;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingSuccessScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void callDelegateFunction(float dt);
    
};

#endif
