#ifndef __CHILDACCOUNTSCENE_SCENE_H__
#define __CHILDACCOUNTSCENE_SCENE_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"
#include "MessageBox.h"

USING_NS_CC;

enum ChildAccountScreenLocationEnum { childNameScreen, childDOBScreen, childSelectOomeeScreen};

class ChildAccountScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    long _errorCode;
    bool isNewChildAccount;
    std::string passedChildName;
    
    TextInputLayer *childNameInputText;
    TextInputLayer *dayInputText;
    TextInputLayer *monthInputText;
    TextInputLayer *yearInputText;
    
    Size visibleSize;
    Vec2 origin;
    
    ChildAccountScreenLocationEnum currentScreen;
    
    Label* title;
    Label* subTitle;

    ElectricDreamsButton *backButton;
    ElectricDreamsButton *nextButton;
    
    void addVisualElementsToScene();
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();
    
    void clearElementsOnScreen();
    
    void changeElementsToDOBScreen();
    void changeElementsToChildNameScreen();
    void changeElementsToOomeeScreen();
    
    void setDOBNextButtonVisible();
    
    void backButtonPressed();
    void nextButtonPressed();
    
    void registerChildAccount();
    
    std::vector<ElectricDreamsButton*> OomeeButtons;
    
    ElectricDreamsButton* oomeeButton0;
    ElectricDreamsButton* oomeeButton1;
    ElectricDreamsButton* oomeeButton2;
    ElectricDreamsButton* oomeeButton3;
    ElectricDreamsButton* oomeeButton4;
    
    int selectedOomeeNo;
    void addOomeesToScene();
    void hideOomees();
    void showOomees();
    void selectOomee(int oomeeNumber);
    
    virtual void onEnterTransitionDidFinish();

public:
    static cocos2d::Scene* createScene(std::string ChildName, long errorCode);

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountScene);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);

};

#endif // __ChildAccountScene_SCENE_H__
