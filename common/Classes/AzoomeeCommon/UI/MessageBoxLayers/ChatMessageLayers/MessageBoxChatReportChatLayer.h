#ifndef AzoomeeCommon_MessageBox_Chat_ReportChat_Layer_h
#define AzoomeeCommon_MessageBox_Chat_ReportChat_Layer_h

#include <cocos/cocos2d.h>
#include "../../ElectricDreamsButton.h"
#include "../../../ErrorCodes.h"
#include "../MessageBoxLayer.h"
#include "../../YouTubeVideoLayer.h"
#include "../../MessageBox.h"

namespace Azoomee
{

class MessageBoxChatReportChatLayer : public MessageBoxLayer, public ElectricDreamsButtonDelegate
{
private:
    
    std::string _messageBoxTitle;
    std::string _messageBoxBody;
    
    ElectricDreamsButton* reportButton;
    ElectricDreamsButton* onlineSafetyTipsButton;
    ElectricDreamsButton* cancelButton;
    
    YouTubeVideoLayer* youTubeVideoLayer;
    
    cocos2d::Label* messageTitleLabel;

    cocos2d::Sprite* oomeeSprite;

    cocos2d::Layer* windowLayer;
    
    MessageBox* onlineSafetySlides;
    
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
    static Layer* create(Layer* parentLayer);
    
    CREATE_FUNC(MessageBoxChatReportChatLayer);
  
    /// Called when the content size of the scene has changed
    void onSizeChanged() override;
};
  
}

#endif
