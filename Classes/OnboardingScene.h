#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

class OnboardingScene : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    
    static cocos2d::Scene* createSceneWithErrorCode(long errorCode);
    
    void handleErrorCode(long errorCode);

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    
    void closeOnboarding();
    void moveToPasswordScreen(Node* button);
    void moveToEmailScreen(Node* button);
    void moveToPinScreen(Node* button);
    void signup();
    
    void disableMoveButton(Node* button);
    void enableMoveButton(Node* button);
    
    void addListenerToButton(cocos2d::Sprite *spriteImage);
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Layer *onboardingContent;
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingScene);
    
    //Editbox Delegate Functions
    //void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    //void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
    //Input String Checker Functions
    bool isCharacter(const char Character);
    bool isNumber(const char Character);
    bool isValidEmailAddress(const char * EmailAddress);
    bool isValidPassword(const char * password);
    bool isValidPin(const char * pinNumber);

};

#endif // __OnboardingScene_SCENE_H__
