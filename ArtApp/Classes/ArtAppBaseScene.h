#ifndef __ARTAPP_BASE_SCENE_H__
#define __ARTAPP_BASE_SCENE_H__

#include "cocos2d.h"
#include "DrawingCanvas.h"
class ArtAppBase : public cocos2d::Layer
{
private:
    cocos2d::ui::Button* backButton;
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void addBackButton();
    
    void backButtonCallBack();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ArtAppBase);
};

#endif // __ARTAPP_BASE_SCENE_H__
