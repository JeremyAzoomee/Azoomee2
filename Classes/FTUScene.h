#ifndef __FTU_SCENE_H__
#define __FTU_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

NS_AZOOMEE_BEGIN

class FTUScene : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor* ftuLayer;
    
    cocos2d::Label* titleLabel;
    cocos2d::Label* subTitleLabel;
    
    ElectricDreamsButton *getStartedButton;
    ElectricDreamsButton *notNowButton;
    
    void addBackground();
    void addLayer();
    void addLabels();
    void addImages();
    void addButtons();

public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
    // implement the "static create()" method manually
    CREATE_FUNC(FTUScene);

};

NS_AZOOMEE_END

#endif
