#ifndef AzoomeeCommon_MessageBox_SignLoginLayer_h
#define AzoomeeCommon_MessageBox_SignLoginLayer_h

#include <cocos/cocos2d.h>
#include <cocos/ui/UIScrollView.h>
#include "../ElectricDreamsButton.h"
#include "../../ErrorCodes.h"
#include "MessageBoxLayer.h"

namespace Azoomee
{

class MessageBoxSignLoginLayer : public MessageBoxLayer, public ElectricDreamsButtonDelegate
{
private:
    
    std::vector<std::string> _buttonsTitleList;
    std::vector<std::string> _buttonsReferenceList;
    std::string _messageBoxTitle;
    std::string _messageBoxBody;
    
    cocos2d::Label* messageTitleLabel;
    cocos2d::Label* messageBodyLabel;
    cocos2d::ui::ScrollView* scrollView;
    std::vector<ElectricDreamsButton*> buttonsList;
    ElectricDreamsButton* cancelButton;
    
    cocos2d::Layer* windowLayer;
    
    float textMaxWidth;
    float buttonSpaceWidth;

    float percentageOfScreenForBox;
    void setPercentageofScreenForBox();
    void addOrRemoveWires();
    
    //MessageBox functions
    void createTitle();
    void createBody();
    void createButtons();
    void createCancelButton();
    void positionButtonsBasedOnWidth(float yPosition);
    void createMessageWindow();
    void addObjectsToWindow();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button) override;
  
protected:
    
    virtual bool init() override;
    
    void initMessageBoxTextLayer(std::string Title, std::string Body, long errorCode);
    
    void addButtonWithTitle(const std::string& buttonTitle);
    
    // Cancel button was pressed
    virtual void onCancelPressed();
    // A button other than Cancel was pressed
    virtual void onButtonPressed(int buttonSelect);
  
public:
    static Layer* createSignLoginLayer(Layer* parentLayer);
    
    CREATE_FUNC(MessageBoxSignLoginLayer);
  
    /// Called when the content size of the scene has changed
    void onSizeChanged() override;
};
  
}

#endif
