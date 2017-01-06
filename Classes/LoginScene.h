#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

class LoginScene : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
private:
    void handleErrorCode(long errorCode);
    
    long _errorCode;
    
public:
    static cocos2d::Scene* createScene(long errorCode);

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    cocos2d::Sprite *emailBackButton;
    cocos2d::Sprite *emailNextButton;
    cocos2d::Sprite *passwordBackButton;
    cocos2d::Sprite *passwordNextButton;
    
    cocos2d::ui::Button *loginButton;
    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
    
    virtual void onEnterTransitionDidFinish();
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    
    void moveLoginToEmailScreen(cocos2d::ui::Button* button);
    void moveToBackFirstScreenEnableLogin(Node* button);
    void moveToPasswordScreen(Node* button);
    void moveToEmailScreen(Node* button);
    void login();
    
    void disableMoveButton(Node* button);
    void enableMoveButton(Node* button);
   
    void addListenerToButton(cocos2d::Sprite *spriteImage);
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Layer *loginContent;
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);

    CREATE_FUNC(LoginScene);
};

#endif
