#ifndef __CHILDACCOUNTSCENE_SCENE_H__
#define __CHILDACCOUNTSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

class ChildAccountScene : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
private:
    
    void handleErrorCode(long errorCode);
    
    long _errorCode;

public:
    static cocos2d::Scene* createScene(std::string ChildName, long errorCode);

    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    void addOomeesToLayer();
    
    void backButtonCloseScene();
    void childNameNextButtonPressed(Node* button);
    void setDOBLabel();
    void moveToDOBInput(Node* button);
    void moveToChildNameInput(Node* button);
    void moveToOomeeSelection(Node* button);
    void selectOomee(cocos2d::Sprite* selectedOomee);
    void registerChildAccount();
    
    void disableMoveButton(Node* button);
    void enableMoveButton(Node* button);
    
    void setChildName(std::string ChildName);
    bool isNewChildAccount;
    std::string passedChildName;
    
    int selectedOomeeNo;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Layer *childAccountContent;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildAccountScene);
    
    void addListenerToButton(cocos2d::Sprite *spriteImage);
    
    //Editbox Delegate Functions
    //void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    //void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
    cocos2d::ui::EditBox *editBox_childName;
    cocos2d::ui::EditBox *editBox_day;
    cocos2d::ui::EditBox *editBox_month;
    cocos2d::ui::EditBox *editBox_year;
    
    cocos2d::Sprite *buttonCloseChild;
    cocos2d::Sprite *buttonNextChild;
    cocos2d::Sprite *buttonBackDob;
    cocos2d::Sprite *buttonNextDob;
    cocos2d::Sprite *buttonBackOomee;
    cocos2d::Sprite *buttonNextOomee;
    
    cocos2d::Label *labelDob;
    cocos2d::Label *labelOomee;
};

#endif // __ChildAccountScene_SCENE_H__
