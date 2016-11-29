#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "cocos2d.h"

class LoginScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    
    void loginButtonAction();
    void backButtonAction();
    void nextButtonAction();
    
    void addListenerToButton(cocos2d::Sprite *spriteImage);
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Layer *loginContent;
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoginScene);
};

#endif // __LoginScene_SCENE_H__
