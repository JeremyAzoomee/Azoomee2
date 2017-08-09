#ifndef __Azoomee_ForceUpdateAppLockScene_H__
#define __Azoomee_ForceUpdateAppLockScene_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

NS_AZOOMEE_BEGIN

class ForceUpdateAppLockScene : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    ElectricDreamsButton *updateButton;
    
    void addVisualComponentsToScene();
    void addUpdateButtonToScene();
    void onExit();

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
    // implement the "static create()" method manually
    CREATE_FUNC(ForceUpdateAppLockScene);

};

NS_AZOOMEE_END

#endif
