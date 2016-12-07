#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include "cocos2d.h"

class OnboardingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createSceneWithEmail(std::string EmailAddress);

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene(std::string EmailAddress);
    void addContentLayerToScene();
    void addTextBoxesToLayer(std::string EmailAddress);
    void addButtonsToLayer();
    void addLabelsToLayer();
    
    void checkIfEmailAlreadyEntered();
    
    void emailNextButton();
    void passwordBackButton();
    void passwordNextButton();
    void pinBackButton();
    void pinNextButton();
    
    //Email String Checker Functions
    bool isCharacter(const char Character);
    bool isNumber(const char Character);
    bool isValidEmailAddress(const char * EmailAddress);
    bool isValidPin(const char * pinNumber);
    
    void addListenerToButton(cocos2d::Sprite *spriteImage);
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Layer *onboardingContent;
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingScene);

};

#endif // __OnboardingScene_SCENE_H__
