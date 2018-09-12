//
//  EditAccountLayer.h
//  azoomee2
//
//  Created by Macauley on 28/08/2018.
//

#ifndef EditAccountLayer_h
#define EditAccountLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class EditAccountLayer : public cocos2d::ui::Layout, public HttpRequestCreatorResponseDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    cocos2d::ui::Layout* _accountTypeLayout = nullptr;
    cocos2d::ui::Layout* _nameLayout = nullptr;
    cocos2d::ui::Layout* _editNameLayout = nullptr;
    cocos2d::ui::Layout* _displayNameLayout = nullptr;
    TextInputLayer* _editNameInput = nullptr;
    cocos2d::ui::Text* _nameText = nullptr;
    cocos2d::ui::Button* _editNameButton = nullptr;
    cocos2d::ui::Text* _emailText = nullptr;
    TextInputLayer* _pinEditbox = nullptr;
    cocos2d::ui::Button* _editPinButton = nullptr;
    TextInputLayer* _passwordEditBox = nullptr;
    cocos2d::ui::Button* _editPasswordButton = nullptr;
    
    bool _editingPin = false;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(EditAccountLayer);
    
    //delegate functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END

#endif /* EditAccountLayer_h */
