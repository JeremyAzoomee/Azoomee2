#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"
#include "MultiButtonMessageBoxLayer.h"
#include "OfflineChecker.h"

USING_NS_CC;

class LoginScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MultiButtonMessageBoxLayerDelegate, public OfflineCheckerDelegate
{
private:
    long _errorCode;
    
    TextInputLayer *_usernameTextInput;
    TextInputLayer *_passwordTextInput;
    
    cocos2d::Size visibleSize;
    Vec2 origin;
    Layer *loginContent;
    
    ElectricDreamsButton *loginOptionButton;
    ElectricDreamsButton *signUpOptionButton;
    ElectricDreamsButton *emailBackButton;
    ElectricDreamsButton *emailNextButton;
    ElectricDreamsButton *passwordBackButton;
    ElectricDreamsButton *loginButton;
    ElectricDreamsButton *previewModeButton;
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addOfflineOptionToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    
    void disableButton(Node* button);
    void enableButton(Node* button);
    
    void setTextInputFocus(TextInputLayer* textInputLayer);
    
    void switchToSignupScene(ElectricDreamsButton* button);
    void moveToAndSetupEmailScreen(ElectricDreamsButton* button);
    void moveToBackToSelectionScreen(ElectricDreamsButton* button);
    void moveToAndSetupPasswordScreen(ElectricDreamsButton* button);
    void login(ElectricDreamsButton* button);
    void autoLogin(std::string username, std::string password);
    void moveToPreviewScene();
    
    virtual void onEnterTransitionDidFinish();
    
    bool shouldDoAutoLogin;
    bool shouldDisplayMessage;
    
public:
    static Scene* createScene(long errorCode);
    static Scene* createSceneWithAutoLogin();
    static Scene* createSceneWithAutoLoginAndErrorDisplay();

    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);
    void MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    void connectivityStateChanged(bool online);

    CREATE_FUNC(LoginScene);
};

#endif
