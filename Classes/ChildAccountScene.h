#ifndef __CHILDACCOUNTSCENE_SCENE_H__
#define __CHILDACCOUNTSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/PrivacyLayer.h>

NS_AZOOMEE_BEGIN

class ChildAccountScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    cocos2d::Sprite* _progressIndicatior = nullptr;

    TextInputLayer *_childNameInputText = nullptr;
    TextInputLayer *_dayInputText = nullptr;
    TextInputLayer *_monthInputText = nullptr;
    TextInputLayer *_yearInputText = nullptr;
    
    bool _doNotShowInputError = false;
    
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    
    cocos2d::Label* _sceneTitle = nullptr;
    cocos2d::Label* _profileNameTitle = nullptr;
    cocos2d::Label* _profileNameError = nullptr;
    cocos2d::Label* _profileDOBTitle = nullptr;
    cocos2d::Label* _profileDOBError = nullptr;
    cocos2d::Label* _profileDOBSubTitle = nullptr;
    cocos2d::Label* _oomeesTitle = nullptr;
    
    ElectricDreamsButton *_cancelButton = nullptr;
    ElectricDreamsButton *_nextButton = nullptr;
    ElectricDreamsButton *_backButton = nullptr;
    ElectricDreamsButton *_submitButton = nullptr;
    
    PrivacyLayer* _privacyLayer = nullptr;
    
    void setupScene();
    void addBackgroundandDecoration();
    void addProgressIndicator();
    void AddTitleToScene();
    void addLabelToScene();
    void addTextboxScene();
    void addButtonsScene();
    void addPrivacyButton();
    
    void clearElementsOnScreen();
    
    void changeElementsToTextInputScreen();
    void changeElementsToOomeeScreen();
    void shouldChangeElementsToOomeeScreen();
    
    bool DOBisDate();
    bool DOBisDateInFuture();
    
    void setNewLayout();
    void setDateInputHasError(bool hasError);
    void checkProfileNameInputForError();
    
    void registerChildAccount();
    
    std::vector<ElectricDreamsButton*> _oomeeButtons;
    int _selectedOomeeNo;
    
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
