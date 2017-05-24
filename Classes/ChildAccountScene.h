#ifndef __CHILDACCOUNTSCENE_SCENE_H__
#define __CHILDACCOUNTSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

USING_NS_CC;
using namespace Azoomee;

class ChildAccountScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    long _errorCode;
    
    TextInputLayer *childNameInputText;
    TextInputLayer *dayInputText;
    TextInputLayer *monthInputText;
    TextInputLayer *yearInputText;
    
    Size visibleSize;
    Vec2 origin;
    
    Label* sceneTitle;
    Label* profileNameTitle;
    Label* profileDOBTitle;
    Label* profileDOBSubTitle;
    Label* oomeesTitle;
    Label* oomeesSubTitle;
    
    ElectricDreamsButton *cancelButton;
    ElectricDreamsButton *nextButton;
    ElectricDreamsButton *nextButtonPlaceholder;
    ElectricDreamsButton *backButton;
    ElectricDreamsButton *submitButton;
    ElectricDreamsButton *submitButtonPlaceholder;
    
    void AddTitleToScene();
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();
    
    void clearElementsOnScreen();
    
    void changeElementsToTextInputScreen();
    void changeElementsToOomeeScreen();
    
    bool DOBisDate();
    
    void registerChildAccount();
    
    std::vector<ElectricDreamsButton*> OomeeButtons;
    Sprite* oomeeGlow;
    
    int selectedOomeeNo;
    void addOomeesToScene();
    void hideOomees();
    void showOomees();
    void selectOomee(int oomeeNumber);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();

public:
    static cocos2d::Scene* createScene(std::string ChildName, long errorCode);

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountScene);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);

};

#endif // __ChildAccountScene_SCENE_H__
