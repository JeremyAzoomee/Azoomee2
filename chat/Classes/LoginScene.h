#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "AzoomeeChat/AzoomeeChat.h"
#include "Auth/AuthAPI.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <cocos/cocos2d.h>


NS_AZOOMEE_CHAT_BEGIN

/**
 * A simple scene to login a parent user account.
 * Based off azoomee2/LoginScene.
 */
class LoginScene : public cocos2d::Scene, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public AuthAPIObserver
{
private:
    typedef cocos2d::Scene Super;
    
    enum LoginScreenLocationEnum { emailLoginScreen, passwordLoginScreen };
    LoginScreenLocationEnum currentScreen;
    
    TextInputLayer* emailTextInput;
    TextInputLayer* passwordTextInput;
    ElectricDreamsButton* backButton;
    ElectricDreamsButton* nextButton;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Label* title;
    std::string storedUsername;
    
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void getUserDefaults();
    
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();

    void changeElementsToPasswordScreen();
    void changeElementsToEmailScreen();
    void backButtonPressed();
    void nextButtonPressed();
    
    void login(const std::string& username, const std::string& password);
    
    // - TextInputLayerDelegate
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid) override;
    // - ElectricDreamsButtonDelegate
    void buttonPressed(ElectricDreamsButton* button) override;
    
    // - AuthAPIObserver
    void onAuthAPILogin() override;
    
public:
    
    CREATE_FUNC(LoginScene);
};

NS_AZOOMEE_CHAT_END

#endif
