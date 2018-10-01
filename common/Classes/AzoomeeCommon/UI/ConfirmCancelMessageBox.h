//
//  ConfirmCancelMessageBox.h
//  AzoomeeCommon
//
//  Created by Macauley on 20/09/2018.
//

#ifndef ConfirmCancelMessageBox_h
#define ConfirmCancelMessageBox_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "../Azoomee.h"

NS_AZOOMEE_BEGIN
class ConfirmCancelMessageBox;

class ConfirmCancelMessageBoxDelegate
{
public:
    virtual void onConfirmPressed(ConfirmCancelMessageBox* pSender) = 0;
    virtual void onCancelPressed(ConfirmCancelMessageBox* pSender) = 0;
};

class ConfirmCancelMessageBox : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    
    cocos2d::ui::Button* _confirmButton = nullptr;
    cocos2d::ui::Button* _cancelButton = nullptr;
    cocos2d::Label* _titleText = nullptr;
    cocos2d::ui::Layout* _messageBox = nullptr;
    
    ConfirmCancelMessageBoxDelegate* _delegate = nullptr;
    
public:
    static ConfirmCancelMessageBox* createWithParams(const std::string& title, const std::string& confirmButtonFilename, const std::string& cancelButtonFilename, const cocos2d::Color3B& backgroundColour = cocos2d::Color3B::WHITE, const cocos2d::Color4B& textColour = cocos2d::Color4B::BLACK);
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setParams(const std::string& title, const std::string& confirmButtonFilename, const std::string& cancelButtonFilename, const cocos2d::Color3B& backgroundColour, const cocos2d::Color4B& textColour);
    void setDelegate(ConfirmCancelMessageBoxDelegate* delegate);
    
    CREATE_FUNC(ConfirmCancelMessageBox);
};

NS_AZOOMEE_END


#endif /* ConfirmCancelMessageBox_h */
