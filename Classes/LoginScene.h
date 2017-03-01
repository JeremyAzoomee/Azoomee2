#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"
#include "MessageBox.h"

USING_NS_CC;

enum ScreenLocationEnum { emailScreen, passwordScreen};

class LoginScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    long _errorCode;
    
    TextInputLayer *emailTextInput;
    TextInputLayer *passwordTextInput;
    
    cocos2d::Size visibleSize;
    Vec2 origin;
    
    ScreenLocationEnum currentScreen;
    
    Label* title;
    
    ElectricDreamsButton *backButton;
    ElectricDreamsButton *nextButton;

    std::string username;
    std::string password;
    
    void getUserDefaults();
    
    void addVisualElementsToScene();
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();

    void changeElementsToPasswordScreen();
    void changeElementsToEmailScreen();
    
    void backButtonPressed();
    void nextButtonPressed();
    

    
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
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    CREATE_FUNC(LoginScene);
};

#endif
