#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include "OfflineChecker.h"

USING_NS_CC;
using namespace Azoomee;

enum LoginScreenLocationEnum { emailLoginScreen, passwordLoginScreen};

class LoginScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate, public OfflineCheckerDelegate
{
private:

    TextInputLayer *emailTextInput;
    TextInputLayer *passwordTextInput;
    
    cocos2d::Size visibleSize;
    Vec2 origin;
    
    LoginScreenLocationEnum currentScreen;
    
    Label* title;
    
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
    
    bool shouldDisplayMessage;
    
public:
    static Scene* createScene();
    
    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    void connectivityStateChanged(bool online);
    
    CREATE_FUNC(LoginScene);
};

#endif
