#ifndef __ONBOARDINGSUCCESSSCENE_SCENE_H__
#define __ONBOARDINGSUCCESSSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

NS_AZOOMEE_BEGIN

class OnboardingSuccessScene : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::Label* subTitleLabel;
    
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

NS_AZOOMEE_END

#endif
