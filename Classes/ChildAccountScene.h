#ifndef __CHILDACCOUNTSCENE_SCENE_H__
#define __CHILDACCOUNTSCENE_SCENE_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class ChildAccountScene : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate
{
private:
    
    void handleErrorCode(long errorCode);
    
    long _errorCode;
    
    TextInputLayer *editBox_childName;
    TextInputLayer *editBox_day;
    TextInputLayer *editBox_month;
    TextInputLayer *editBox_year;
    
    Size visibleSize;
    Vec2 origin;
    Layer *childAccountContent;
    
    ElectricDreamsButton *buttonCloseChild;
    ElectricDreamsButton *buttonNextChild;
    ElectricDreamsButton *buttonBackDob;
    ElectricDreamsButton *buttonNextDob;
    ElectricDreamsButton *buttonBackOomee;
    ElectricDreamsButton *buttonNextOomee;
    
    Label *labelDob;
    Label *labelOomee;
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    void addOomeesToLayer();
    
    void disableButton(Node* button);
    void enableButton(Node* button);
    
    void setTextInputFocus(TextInputLayer* textInputLayer);
    
    void backButtonCloseScene();
    void moveToAndSetupDOBInput(ElectricDreamsButton* button);
    void moveBackToChildNameInput(ElectricDreamsButton* button);
    void moveToAndSetupOomeeSelection(ElectricDreamsButton* button);
    void selectOomee(Sprite* selectedOomee);
    void registerChildAccount();
    
    void addListenerToOomeeButton(Sprite *oomeeSprite);
    
    void setDOBLabel();
    void setChildName(std::string ChildName);
    bool isNewChildAccount;
    std::string passedChildName;
    
    int selectedOomeeNo;
    
    virtual void onEnterTransitionDidFinish();
    

public:
    static cocos2d::Scene* createScene(std::string ChildName, long errorCode);

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountScene);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);

};

#endif // __ChildAccountScene_SCENE_H__
