#ifndef AzoomeeCommon_MessageBox_h
#define AzoomeeCommon_MessageBox_h

#include <cocos/cocos2d.h>
#include "../Input/TextInputLayer.h"
#include "MessageBoxLayers/MessageBoxLayer.h"
#include "../ErrorCodes.h"

enum MessageBoxLayerEnum {
    ChatReportForModeration,
    OnlineSafetySlides,
    ChatResetModeration
};

namespace Azoomee
{

class MessageBox;

class MessageBoxDelegate
{
public:
    virtual void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) = 0;
};

class MessageBox : public cocos2d::Layer
{
private:
    
    Layer* initLayer(MessageBoxLayerEnum messageBoxLayer, const std::map<std::string, std::string>& propertiesMap, Layer* newLayer);
    
    cocos2d::LayerColor *backgroundLayer = nullptr;

    cocos2d::Size currentRunningSceneSize;
    
    Layer* windowLayer = nullptr;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    
    void removeSelf(float dt);
    
    //-------Object To Hide -------
    // Due to UIEditbox always being on top
    TextInputLayer* savedTextInputToHide = nullptr;
    void hideTextInput(TextInputLayer* textInputToHide);
    void UnHideTextInput();
  
protected:
    
    virtual bool init() override;
  
public:
    
    static const char* const messageBoxConst_OK;
    static const char* const messageBoxConst_Cancel;
    static const char* const messageBoxConst_Login;
    static const char* const messageBoxConst_SignUp;
    static const char* const messageBoxConst_ResetPassword;
    static const char* const messageBoxConst_Report;
    static const char* const messageBoxConst_Reset;
    
    //Main function for creating a MessageBox
    static MessageBox* createWith(const std::string& Title, const std::string& Body, const std::vector<std::string>& buttonTitleList, MessageBoxDelegate* _delegate);
    static MessageBox* createWith(const std::string& Title, const std::string& Body, const std::string& Button, MessageBoxDelegate* _delegate);
    static MessageBox* createWith(long errorCode, MessageBoxDelegate* _delegate);
    static MessageBox* createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* _delegate);
    static MessageBox* createWithLayer(MessageBoxLayerEnum messageBoxLayer, MessageBoxDelegate* _delegate);
    static MessageBox* createWithLayer(MessageBoxLayerEnum messageBoxLayer, const std::map<std::string, std::string>& propertiesMap, MessageBoxDelegate* _delegate);
    
    void sendDelegateMessageBoxButtonPressed(const std::string& messageBoxTitle,const std::string& buttonTitle);
    
    CREATE_FUNC(MessageBox);
    
    CC_SYNTHESIZE(MessageBoxDelegate*, _delegate, Delegate);
  
    /// Called when the content size of the scene has changed
    void onSizeChanged();
};
  
}

#endif
