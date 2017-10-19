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
    
    cocos2d::Sprite* _progressIndicatior = nullptr;
    cocos2d::Label* mainTitle = nullptr;
    
    TextInputLayer *_emailTextInput = nullptr;
    TextInputLayer *passwordTextInput = nullptr;
    TextInputLayer *pinTextInput = nullptr;
    
    ElectricDreamsButton* signupButton = nullptr;
    ElectricDreamsButton* cancelButton = nullptr;
    ElectricDreamsButton* privacyButton = nullptr;
    ElectricDreamsButton* termsButton = nullptr;
    
    cocos2d::Label* _emailTitle = nullptr;
    cocos2d::Label* _emailError = nullptr;
    cocos2d::Label* _passwordTitle = nullptr;
    cocos2d::Label* _passwordError = nullptr;
    cocos2d::Label* _pinTitle = nullptr;
    cocos2d::Label* _pinError = nullptr;
    cocos2d::Label* _pinSubTitle = nullptr;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    bool hasBeenSet = false;
    
    void addBackgroundandDecoration();
    void addProgressIndicator();
    void addMainTitleToScene();
    void addTextboxScene();
    void addLabelsToScene();
    void addButtonsScene();
    void addTermsAndConditionsToScene();
    
    void signUp();
    
    virtual void onEnterTransitionDidFinish();
    virtual void onEnter();

public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    void editBoxEditingDidEnd(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    // implement the "static create()" method manually
    CREATE_FUNC(OnboardingScene);

};

NS_AZOOMEE_END

#endif
