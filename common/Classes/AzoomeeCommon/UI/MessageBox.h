#ifndef AzoomeeCommon_MessageBox_h
#define AzoomeeCommon_MessageBox_h

#include <cocos/cocos2d.h>
#include "ElectricDreamsButton.h"
#include "../Input/TextInputLayer.h"
#include "../ErrorCodes.h"


namespace Azoomee
{

class MessageBox;

class MessageBoxDelegate
{
public:
    virtual void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) = 0;
};

class MessageBox : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    std::vector<std::string> _buttonsTitleList;
    std::string _messageBoxTitle;
    
    cocos2d::LayerColor *backgroundLayer;
    cocos2d::Label* messageTitleLabel;
    cocos2d::Label* messageBodyLabel;
    std::vector<ElectricDreamsButton*> buttonsList;
    ElectricDreamsButton* cancelButton;
    cocos2d::Layer* windowLayer;
    
    float textMaxWidth;
    float buttonSpaceWidth;

    
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
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button) override;
  
protected:
    
    virtual bool init();
    
    void initMessageBoxLayer(std::string Title, std::string Body, MessageBoxDelegate* _delegate);
    void addButtonWithTitle(const std::string& buttonTitle);
    
    // Cancel button was pressed
    virtual void onCancelPressed();
    // A button other than Cancel was pressed
    virtual void onButtonPressed(int buttonSelect);
  
public:
    //Main function for creating a MessageBox
    static MessageBox* createWith(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MessageBoxDelegate* _delegate);
    static MessageBox* createWith(std::string Title, std::string Body, std::string Button, MessageBoxDelegate* _delegate);
    static MessageBox* createWith(long errorCode, MessageBoxDelegate* _delegate);
    static MessageBox* createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* _delegate);
    
    CREATE_FUNC(MessageBox);
    
    CC_SYNTHESIZE(MessageBoxDelegate*, _delegate, Delegate);
  
};
  
}

#endif