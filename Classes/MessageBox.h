#ifndef __MULTI_BUTTON_MESSAGEBOX_LAYER_H__
#define __MULTI_BUTTON_MESSAGEBOX_LAYER_H__

#include <cocos/cocos2d.h>
#include "ElectricDreamsButton.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/ErrorCodes.h>

USING_NS_CC;

class MessageBox;

class MessageBoxDelegate
{
public:
    virtual void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) = 0;
};

class MessageBox : public Layer, public ElectricDreamsButtonDelegate
{
private:

    cocos2d::Size visibleSize;
    Vec2 origin;
    
    std::vector<std::string> _buttonsTitleList;
    std::string _messageBoxTitle;
    
    LayerColor *backgroundLayer;
    Label* messageTitleLabel;
    Label* messageBodyLabel;
    std::vector<ElectricDreamsButton*> buttonsList;
    ElectricDreamsButton* cancelButton;
    Layer* windowLayer;
    
    float textMaxWidth;
    float buttonSpaceWidth;
    
    void initMessageBoxLayer(std::string Title, std::string Body, MessageBoxDelegate* _delegate);
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    
    //MessageBox functions
    void createTitle();
    void createBody(std::string messageBody);
    void createButtons();
    void createCancelButton();
    void positionButtonsBasedOnWidth(float yPosition);
    void createMessageWindow();
    void addObjectsToWindow();
    
    void removeSelf(float dt);
    
    //-------Object To Hide -------
    // Due to UIEditbox always being on top
    TextInputLayer* savedTextInputToHide;
    void hideTextInput(TextInputLayer* textInputToHide);
    void UnHideTextInput();
    
    void handlePreviewLoginSignupMessageBoxSelection(int buttonSelect);
    
public:
    //Main function for creating a MessageBox
    static Layer* createWith(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MessageBoxDelegate* _delegate);
    static Layer* createWith(std::string Title, std::string Body, std::string Button, MessageBoxDelegate* _delegate);
    static Layer* createWith(long errorCode, MessageBoxDelegate* _delegate);
    static Layer* createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* _delegate);
    
    static Layer* createPreviewLoginSignupMessageBox();
    
    virtual bool init();
    
    CREATE_FUNC(MessageBox);
    
    CC_SYNTHESIZE(MessageBoxDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
