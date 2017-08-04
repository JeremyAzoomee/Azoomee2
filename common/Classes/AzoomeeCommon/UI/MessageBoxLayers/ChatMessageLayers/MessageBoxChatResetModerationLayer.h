#ifndef AzoomeeCommon_MessageBox_Chat_ResetModeration_Layer_h
#define AzoomeeCommon_MessageBox_Chat_ResetModeration_Layer_h

#include <cocos/cocos2d.h>
#include "../../ElectricDreamsButton.h"
#include "../../../ErrorCodes.h"
#include "../MessageBoxLayer.h"
#include "../../MessageBox.h"

namespace Azoomee
{

class MessageBoxChatResetModerationLayer : public MessageBoxLayer, public ElectricDreamsButtonDelegate
{
private:
    
    std::map<std::string, std::string> _propertiesMap;
    
    std::string _messageBoxTitle;
    std::string _messageBoxBody;
    
    ElectricDreamsButton* resetButton = nullptr;
    ElectricDreamsButton* onlineSafetyTipsButton = nullptr;
    ElectricDreamsButton* cancelButton = nullptr;
    
    cocos2d::Label* messageTitleLabel = nullptr;
    cocos2d::Label* messageBodyLabel = nullptr;

    cocos2d::Sprite* oomeeSprite = nullptr;

    cocos2d::Layer* windowLayer = nullptr;
    
    MessageBox* onlineSafetySlides = nullptr;
    
    float textMaxWidth;
    float buttonSpaceWidth;

    float percentageOfScreenForBox;
    void setOrientation();
    bool isLandscape = false;
    
    //MessageBox functions
    void createTitle();
    void createBody();
    void createSprite();
    void createButtons();
    void createCancelButton();
    void createMessageWindowLandscape();
    void createMessageWindowPortrait();
    void addObjectsToWindowLandscape();
    void addObjectsToWindowPortrait();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button) override;
  
protected:
    
    virtual bool init() override;
    
    void initMessageBoxTextLayer(std::string Title, std::string Body, long errorCode);
    
    void addButtonWithTitle(const std::string& buttonTitle);
  
public:
    static Layer* create(const std::map<std::string, std::string>& propertiesMap,Layer* parentLayer);
    
    CREATE_FUNC(MessageBoxChatResetModerationLayer);
  
    /// Called when the content size of the scene has changed
    void onSizeChanged() override;
};
  
}

#endif
