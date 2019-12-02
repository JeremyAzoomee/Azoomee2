#ifndef AzoomeeCommon_MessageBox_h
#define AzoomeeCommon_MessageBox_h

#include <cocos/cocos2d.h>
#include "../Input/TextInputLayer.h"
#include "MessageBoxLayers/MessageBoxLayer.h"
#include "../ErrorCodes.h"

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
    
    static const char* const kOK;
    static const char* const kCancel;
    static const char* const kLogin;
    static const char* const kSignUp;
    static const char* const kResetPassword;
    static const char* const kReport;
    static const char* const kReset;
    static const char* const kDelete;
    
    //Main function for creating a MessageBox
    static MessageBox* createWith(const std::string& Title, const std::string& Body, const std::vector<std::string>& buttonTitleList, MessageBoxDelegate* delegate);
    static MessageBox* createWith(const std::string& Title, const std::string& Body, const std::string& Button, MessageBoxDelegate* delegate);
    static MessageBox* createWith(long errorCode, MessageBoxDelegate* delegate);
    static MessageBox* createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* delegate);
    static MessageBox* createWith(const std::string& Title, const std::string& imagePath, const std::string& Body, const std::string& Button, MessageBoxDelegate* delegate);
    
    void removeMessageBox();
    
    void sendDelegateMessageBoxButtonPressed(const std::string& messageBoxTitle,const std::string& buttonTitle);
    
    CREATE_FUNC(MessageBox);
    
    CC_SYNTHESIZE(MessageBoxDelegate*, _delegate, Delegate);
  
    /// Called when the content size of the scene has changed
    void onSizeChanged();
};
  
}

#endif
