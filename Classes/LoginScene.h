#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include "OfflineChecker.h"

NS_AZOOMEE_BEGIN

enum LoginScreenLocationEnum { emailLoginScreen, passwordLoginScreen};

class LoginScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate, public OfflineCheckerDelegate
{
private:

    TextInputLayer *emailTextInput;
    TextInputLayer *passwordTextInput;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    LoginScreenLocationEnum currentScreen;
    
    cocos2d::Label* title;
    
    ElectricDreamsButton *backButton;
    ElectricDreamsButton *nextButton;

    std::string storedUsername;
    
    void getUserDefaults();
    
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();

    void changeElementsToPasswordScreen();
    void changeElementsToEmailScreen();
    
    void backButtonPressed();
    void nextButtonPressed();
    
    void login(std::string username, std::string password);
    
    virtual void onEnterTransitionDidFinish();
    void onExit();
    
    bool shouldDisplayMessage = false;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    void editBoxEditingDidEnd(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    void connectivityStateChanged(bool online);
    
    CREATE_FUNC(LoginScene);
};

NS_AZOOMEE_END

#endif
