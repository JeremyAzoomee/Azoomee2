#ifndef __ONBOARDINGSUCCESSSCENE_SCENE_H__
#define __ONBOARDINGSUCCESSSCENE_SCENE_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;


class OnboardingSuccessScene : public Layer, public ElectricDreamsButtonDelegate
{
private:
    
    Size visibleSize;
    Vec2 origin;
    
    ElectricDreamsButton *createOomeeButton;
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addButtonsToLayer();
    void addLabelsToLayer();
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingSuccessScene);
    
};

#endif
