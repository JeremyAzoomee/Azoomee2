#ifndef ChildAccountSuccessScene_hpp
#define ChildAccountSuccessScene_hpp

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class ChildAccountSuccessScene : public Layer, public ElectricDreamsButtonDelegate
{
private:
    
    Size visibleSize;
    Vec2 origin;
    
    ElectricDreamsButton* oomeeButton;
    
    void addVisualElementsToScene();
    void addButtonsToScene();
    void addLabelsToLayer();
    
    void addOomeeToScreen();
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountSuccessScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void callDelegateFunction(float dt);
    
};

#endif
