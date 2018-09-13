#ifndef __Azoomee_ForceUpdateAppLockScene_H__
#define __Azoomee_ForceUpdateAppLockScene_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/Scene.h>

NS_AZOOMEE_BEGIN

class ForceUpdateAppLockScene : public Azoomee::Scene, public ElectricDreamsButtonDelegate
{
    typedef Azoomee::Scene Super;
private:
    cocos2d::Size _visibleSize;
    
    ElectricDreamsButton *updateButton = nullptr;
    
    void addVisualComponentsToScene();
    void addUpdateButtonToScene();
    void onExit() override;
    
protected:
    virtual void onSizeChanged() override;

public:
    //static Azoomee::Scene* createScene();
    virtual bool init() override;
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ForceUpdateAppLockScene);

};

NS_AZOOMEE_END

#endif
