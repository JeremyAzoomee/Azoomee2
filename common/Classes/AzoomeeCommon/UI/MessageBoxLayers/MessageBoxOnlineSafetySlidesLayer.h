#ifndef AzoomeeCommon_MessageBox_OnlineSafetySlides_Layer_h
#define AzoomeeCommon_MessageBox_OnlineSafetySlides_Layer_h

#include <cocos/cocos2d.h>
#include "../ElectricDreamsButton.h"
#include "../../ErrorCodes.h"
#include "MessageBoxLayer.h"

namespace Azoomee
{

class MessageBoxOnlineSafetySlidesLayer : public MessageBoxLayer, public ElectricDreamsButtonDelegate
{
private:

    int currentSlideNumber;
    
    ElectricDreamsButton *watchSearchItUpButton;
    ElectricDreamsButton *chevronLeftButton;
    ElectricDreamsButton *chevronRightButton;
    ElectricDreamsButton *cancelButton;
    
    cocos2d::Label* titleLabel;
    cocos2d::Label* slideTitleLabel;
    cocos2d::Label* mainTextLabel;
    
    cocos2d::Sprite* mainImage;

    cocos2d::Layer* windowLayer;
    
    void createBackground();
    void addUIObjects();
    void setToCurrentSlideNumber();
    void moveSlideNumberBy(int moveBy);
    
    void createMessageWindow();
    void addObjectsToWindowLandscape();
    void addObjectsToWindowPortrait();
    void createCancelButton();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button) override;
  
protected:
    
    virtual bool init() override;
    
    /*void initMessageBoxTextLayer(std::string Title, std::string Body, long errorCode);
    
    void addButtonWithTitle(const std::string& buttonTitle);*/
  
public:
    static Layer* create(Layer* parentLayer);
    
    CREATE_FUNC(MessageBoxOnlineSafetySlidesLayer);
  
    /// Called when the content size of the scene has changed
    void onSizeChanged() override;
};
  
}

#endif
