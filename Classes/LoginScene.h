#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "OfflineChecker.h"

NS_AZOOMEE_BEGIN

enum LoginScreenLocationEnum { emailLoginScreen, passwordLoginScreen};

class LoginScene : public Azoomee::Scene, public cocos2d::IMEDelegate, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate, public OfflineCheckerDelegate
{
    typedef Azoomee::Scene Super;
private:

    TextInputLayer* _emailTextInput = nullptr;
    TextInputLayer* _passwordTextInput = nullptr;
    
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    
    LoginScreenLocationEnum _currentScreen;
    
    cocos2d::Label* _title = nullptr;
    cocos2d::Label* _versionLabel = nullptr;
    
    ElectricDreamsButton* _backButton = nullptr;
    ElectricDreamsButton* _nextButton = nullptr;

    std::string _storedUsername;
    
    void getUserDefaults();
    
    void addBackground();
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();

    void changeElementsToPasswordScreen();
    void changeElementsToEmailScreen();
    
    void backButtonPressed();
    void nextButtonPressed();
    
    void login(std::string username, std::string password);
    
    virtual void onEnterTransitionDidFinish() override;
    void onExit() override;
    
    bool shouldDisplayMessage = false;
    
    // - IMEDelegate
    void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid) override;
    void textInputReturnPressed(TextInputLayer* inputLayer) override;
    void editBoxEditingDidBegin(TextInputLayer* inputLayer) override;
    void editBoxEditingDidEnd(TextInputLayer* inputLayer) override;
    void buttonPressed(ElectricDreamsButton* button) override;
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) override;
    void connectivityStateChanged(bool online) override;
    
    CREATE_FUNC(LoginScene);
};

NS_AZOOMEE_END

#endif
