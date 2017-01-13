#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

class OnboardingScene : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(OnboardingScene);
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    static cocos2d::Scene* createScene(long errorCode);
    
private:
    void handleErrorCode(long errorCode);
    long _errorCode;
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    
    void closeOnboarding();
    void cleanPasswordScreen();
    void cleanEmailScreen();
    void cleanPinScreen();
    void signup();
    void moveToScreen(cocos2d::Sprite *sender);
    
    void disableMoveButton(Node* button);
    void enableMoveButton(Node* button);
    
    void addListenerToButton(cocos2d::Sprite *spriteImage);
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Layer *onboardingContent;
    
    // implement the "static create()" method manually
    
    
    //Editbox Delegate Functions
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
    //global variables for textboxes and buttons
    cocos2d::ui::EditBox *editBox_email;
    cocos2d::ui::EditBox *editBox_password;
    cocos2d::ui::EditBox *editBox_pin;
    
    cocos2d::Sprite *buttonBackEmail;
    cocos2d::Sprite *buttonNextEmail;
    cocos2d::Sprite *buttonBackPassword;
    cocos2d::Sprite *buttonNextPassword;
    cocos2d::Sprite *buttonNextPin;
    cocos2d::Sprite *buttonBackPin;
};

#endif
