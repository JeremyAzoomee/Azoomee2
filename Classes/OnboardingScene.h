#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

USING_NS_CC;
using namespace Azoomee;

class OnboardingScene : public Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    Label* mainTitle;
    
    TextInputLayer *emailTextInput;
    TextInputLayer *passwordTextInput;
    TextInputLayer *pinTextInput;
    
    ElectricDreamsButton* signupButton;
    ElectricDreamsButton* cancelButton;
    
    Size visibleSize;
    Vec2 origin;
    
    bool hasBeenSet;
    
    void addMainTitleToScene();
    void addTextboxScene();
    void addLabelsToScene();
    void addButtonsScene();
    
    void signUp();
    
    virtual void onEnterTransitionDidFinish();
    virtual void onEnter();

public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingScene);

};

#endif
