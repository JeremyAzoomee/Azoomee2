#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

USING_NS_CC;
using namespace Azoomee;

class OnboardingScene : public Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate
{
private:
    long _errorCode;
    
    TextInputLayer *emailTextInput;
    TextInputLayer *passwordTextInput;
    TextInputLayer *pinTextInput;
    
    ElectricDreamsButton* signupButton;
    ElectricDreamsButton* cancelButton;
    
    Size visibleSize;
    Vec2 origin;
    
    bool hasBeenSet;
    
    void addLabelsToScene();
    void addTextboxScene();
    void addButtonsScene();
    
    void signUp();
    
    //virtual void onEnterTransitionDidFinish();
    virtual void onEnter();

public:
    static cocos2d::Scene* createScene(long errorCode);
    
    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingScene);

};

#endif
