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
    
    int ChildNumber;
    int OomeeAvatarNumber;
    
    ElectricDreamsButton* oomeeButton;
    ElectricDreamsButton* addChildButton;
    
    void addVisualElementsToScene();
    void addButtonsToScene();
    void addLabelsToLayer();
    
    void findChildNumber(std::string ChildNameToFind);
    
    bool buttonHasBeenPressed;
    
public:
    static cocos2d::Scene* createScene(std::string newChildName, int OomeeAvatarNumber);
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountSuccessScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void moveToHubScene(float dt);
    void moveToChildAccountScene(float dt);
    
};

#endif
