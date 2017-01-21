#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;


class OnboardingScene : public Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate
{
private:
    
    void handleErrorCode(long errorCode);
    
    long _errorCode;
    
    TextInputLayer *editBox_email;
    TextInputLayer *editBox_password;
    TextInputLayer *editBox_pin;
    
    Size visibleSize;
    Vec2 origin;
    Layer *onboardingContent;
    
    ElectricDreamsButton *buttonBackEmail;
    ElectricDreamsButton *buttonNextEmail;
    ElectricDreamsButton *buttonBackPassword;
    ElectricDreamsButton *buttonNextPassword;
    ElectricDreamsButton *buttonBackPin;
    ElectricDreamsButton *buttonSignUp;
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    
    void disableButton(Node* button);
    void enableButton(Node* button);
    
    void setTextInputFocus(TextInputLayer* textInputLayer);
    
    void closeOnboarding();
    void moveToAndSetupEmailScreen(ElectricDreamsButton* button);
    void moveToAndSetupPasswordScreen(ElectricDreamsButton* button);
    void moveToAndSetupPinScreen(ElectricDreamsButton* button);
    
    void cleanPasswordScreen();
    void cleanPinScreen();
    
    void signUp();
    
    virtual void onEnterTransitionDidFinish();
    
public:
    static cocos2d::Scene* createScene(long errorCode);

    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingScene);
    
};

#endif
