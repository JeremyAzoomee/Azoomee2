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
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    virtual void onEnterTransitionDidFinish();
    
    void addVisualElementsToScene();
    void addFunctionalElementsToScene();
    void addContentLayerToScene();
    void addTextBoxesToLayer();
    void addButtonsToLayer();
    void addLabelsToLayer();
    void addOomeesToLayer();
    
    void backButtonCloseScene();
    void childNameNextButton(Node* button);
    void setDOBLabel();
    void moveToDOBInput(Node* button);
    bool isDate();
    void moveToChildNameInput(Node* button);
    void moveToOomeeSelection(Node* button);
    void selectOomee(cocos2d::Sprite* selectedOomee);
    void doneButton();
    
    void disableMoveButton(Node* button);
    void enableMoveButton(Node* button);
    
    bool childNameExists(std::string newChildsName);
    
    //Email String Checker Functions
    bool isCharacter(const char Character);
    bool isNumber(const char Character);
    
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

};

#endif // __ChildAccountScene_SCENE_H__
