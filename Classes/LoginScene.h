#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class LoginScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate
{
private:
    long _errorCode;
    
    TextInputLayer *_usernameTextInput;
    TextInputLayer *_passwordTextInput;
    
    Size visibleSize;
    Vec2 origin;
    Layer *loginContent;
    
    ElectricDreamsButton *loginOptionButton;
    ElectricDreamsButton *signUpOptionButton;
    ElectricDreamsButton *emailBackButton;
    ElectricDreamsButton *emailNextButton;
    ElectricDreamsButton *passwordBackButton;
    ElectricDreamsButton *loginButton;
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
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
    
    virtual void onEnterTransitionDidFinish();
    
    bool shouldDoAutoLogin;
    
public:
    static Scene* createScene(long errorCode);
    static Scene* createSceneWithAutoLogin();

    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);

    CREATE_FUNC(LoginScene);
};

#endif
