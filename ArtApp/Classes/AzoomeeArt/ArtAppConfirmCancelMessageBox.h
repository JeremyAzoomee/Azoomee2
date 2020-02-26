//
//  ArtAppConfirmCancelMessageBox.h
//  AzoomeeCommon
//
//  Created by Macauley on 20/09/2018.
//

#ifndef ArtAppConfirmCancelMessageBox_h
#define ArtAppConfirmCancelMessageBox_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "AzoomeeArtApp.h"
#include <AzoomeeCommon/UI/MessagePopupBase.h>

USING_NS_TZ

NS_AZ_AA_BEGIN

class ArtAppConfirmCancelMessageBox : public MessagePopupBase
{
    typedef MessagePopupBase Super;
private:
    
    cocos2d::ui::Button* _confirmButton = nullptr;
    cocos2d::ui::Button* _cancelButton = nullptr;
    cocos2d::Label* _titleText = nullptr;
    cocos2d::ui::Layout* _messageBox = nullptr;
    
    ButtonPressedCallback _confirmCallback = nullptr;
    ButtonPressedCallback _cancelCallback = nullptr;
    
public:
    static ArtAppConfirmCancelMessageBox* createWithParams(const std::string& title, const std::string& confirmButtonFilename, const std::string& cancelButtonFilename, const cocos2d::Color3B& backgroundColour = cocos2d::Color3B::WHITE, const cocos2d::Color4B& textColour = cocos2d::Color4B::BLACK);
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setParams(const std::string& title, const std::string& confirmButtonFilename, const std::string& cancelButtonFilename, const cocos2d::Color3B& backgroundColour, const cocos2d::Color4B& textColour);
    void setOnConfirmCallback(const ButtonPressedCallback& callback);
    void setOnCancelCallback(const ButtonPressedCallback& callback);
    
    CREATE_FUNC(ArtAppConfirmCancelMessageBox);
};

NS_AZ_AA_END


#endif /* ArtAppConfirmCancelMessageBox_h */
