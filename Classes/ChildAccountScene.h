#ifndef __CHILDACCOUNTSCENE_SCENE_H__
#define __CHILDACCOUNTSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

class ChildAccountScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    cocos2d::Sprite* _progressIndicatior = nullptr;
    
    TextInputLayer *childNameInputText = nullptr;
    TextInputLayer *dayInputText = nullptr;
    TextInputLayer *monthInputText = nullptr;
    TextInputLayer *yearInputText = nullptr;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::Label* sceneTitle = nullptr;
    cocos2d::Label* profileNameTitle = nullptr;
    cocos2d::Label* profileNameError = nullptr;
    cocos2d::Label* profileDOBTitle = nullptr;
    cocos2d::Label* profileDOBError = nullptr;
    cocos2d::Label* profileDOBSubTitle = nullptr;
    cocos2d::Label* oomeesTitle = nullptr;
    
    ElectricDreamsButton *cancelButton = nullptr;
    ElectricDreamsButton *nextButton = nullptr;
    ElectricDreamsButton *backButton = nullptr;
    ElectricDreamsButton *submitButton = nullptr;
    
    void setupScene();
    void addBackgroundandDecoration();
    void addProgressIndicator();
    void AddTitleToScene();
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();
    
    void clearElementsOnScreen();
    
    void changeElementsToTextInputScreen();
    void changeElementsToOomeeScreen();
    void shouldChangeElementsToOomeeScreen();
    
    bool DOBisDate();
    bool DOBisDateInFuture();
    
    void setNewLayout();
    
    void registerChildAccount();
    
    std::vector<ElectricDreamsButton*> OomeeButtons;
    cocos2d::Sprite* oomeeGlow;
    
    int selectedOomeeNo;
    void addOomeesToScene();
    void hideOomees();
    void showOomees();
    void selectOomee(int oomeeNumber);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountScene);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    void editBoxEditingDidEnd(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);

};

NS_AZOOMEE_END

#endif // __ChildAccountScene_SCENE_H__
