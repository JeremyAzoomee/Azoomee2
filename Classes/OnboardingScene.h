#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include "ElectricDreamsButton.h"
#include "MessageBox.h"

USING_NS_CC;

enum OnboardingScreenLocationEnum { emailOnboardinScreen, passwordOnboardingScreen, pinOnboardingScreen};

class OnboardingScene : public Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    long _errorCode;
    
    TextInputLayer *emailTextInput;
    TextInputLayer *passwordTextInput;
    TextInputLayer *pinTextInput;
    
    Size visibleSize;
    Vec2 origin;
    
    OnboardingScreenLocationEnum currentScreen;
    
    Label* title;
    Label* subTitle;
    
    ElectricDreamsButton *backButton;
    ElectricDreamsButton *nextButton;
    
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();
    
    void clearElementsOnScreen();
    
    void changeElementsToPasswordScreen();
    void changeElementsToEmailScreen();
    void changeElementsToPinScreen();
    
    void backButtonPressed();
    void nextButtonPressed();
    
    void signUp();
    
    virtual void onEnterTransitionDidFinish();

public:
    static cocos2d::Scene* createScene(long errorCode);
    
    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingScene);

};

#endif
