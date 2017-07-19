#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "AzoomeeChat/AzoomeeChat.h"
#include "Auth/AuthAPI.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>


NS_AZOOMEE_CHAT_BEGIN

/**
 * A simple scene to login a parent user account.
 * Based off azoomee2/LoginScene.
 */
class LoginScene : public Azoomee::Scene, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public AuthAPIObserver
{
private:
    typedef Azoomee::Scene Super;
    
    enum LoginScreenLocationEnum { emailLoginScreen, passwordLoginScreen };
    LoginScreenLocationEnum currentScreen;
    
    cocos2d::Layer* wiresLayer = nullptr;
    cocos2d::Layer* inputLayer = nullptr;
    TextInputLayer* emailTextInput = nullptr;
    TextInputLayer* passwordTextInput = nullptr;
    ElectricDreamsButton* backButton = nullptr;
    ElectricDreamsButton* nextButton = nullptr;
    cocos2d::Label* versionLabel = nullptr;
    cocos2d::Label* titleLabel = nullptr;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
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
    void textInputReturnPressed(TextInputLayer* inputLayer) override;
    void editBoxEditingDidBegin(TextInputLayer* inputLayer) override;
    void editBoxEditingDidEnd(TextInputLayer* inputLayer) override;
    // - ElectricDreamsButtonDelegate
    void buttonPressed(ElectricDreamsButton* button) override;
    
    // - AuthAPIObserver
    void onAuthAPILogin() override;
    void onAuthAPIRequestFailed(const std::string& requestTag, long errorCode) override;
    
protected:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged() override;
    
public:
    
    CREATE_FUNC(LoginScene);
    
};

NS_AZOOMEE_CHAT_END

#endif
