#ifndef __ONBOARDINGSCENE_SCENE_H__
#define __ONBOARDINGSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

class OnboardingScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    cocos2d::Label* mainTitle;
    
    TextInputLayer *emailTextInput;
    TextInputLayer *passwordTextInput;
    TextInputLayer *pinTextInput;
    
    ElectricDreamsButton* signupButton;
    ElectricDreamsButton* cancelButton;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
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

NS_AZOOMEE_END

#endif
