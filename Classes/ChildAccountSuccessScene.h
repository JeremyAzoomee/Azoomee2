#ifndef ChildAccountSuccessScene_hpp
#define ChildAccountSuccessScene_hpp

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

NS_AZOOMEE_BEGIN

class ChildAccountSuccessScene : public cocos2d::Layer, public Azoomee::ElectricDreamsButtonDelegate
{
private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    int ChildNumber;
    
    Azoomee::ElectricDreamsButton* oomeeButton;
    Azoomee::ElectricDreamsButton* addChildButton;
    
    void addVisualElementsToScene();
    void addButtonsToScene();
    void addLabelsToLayer();
    
    void findChildNumber(std::string ChildNameToFind);
    
    bool buttonHasBeenPressed = false;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountSuccessScene);
    
    //Delegate Functions
    void buttonPressed(Azoomee::ElectricDreamsButton* button);
    void moveToHubScene(float dt);
    void moveToChildAccountScene(float dt);
    
};

NS_AZOOMEE_END

#endif
