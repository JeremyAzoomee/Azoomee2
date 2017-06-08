#ifndef __FTU_SCENE_H__
#define __FTU_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

USING_NS_CC;
using namespace Azoomee;

class FTUScene : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    LayerColor* ftuLayer;
    
    Label* titleLabel;
    Label* subTitleLabel;
    
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

#endif
