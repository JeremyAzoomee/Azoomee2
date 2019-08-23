#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include "OfflineChecker.h"
#include "LoginEntry.h"

NS_AZOOMEE_BEGIN

class LoginScene : public Azoomee::Scene, public cocos2d::IMEDelegate, public OfflineCheckerDelegate
{
    typedef Azoomee::Scene Super;
private:
	
	LoginEntry* _loginEntryForm = nullptr;
	DynamicText* _titleText = nullptr;
	cocos2d::Sprite* _bgPattern = nullptr;
	cocos2d::LayerGradient* _gradient = nullptr;
	cocos2d::ui::Layout* _bgColour = nullptr;
	cocos2d::DrawNode* _patternHider = nullptr;
	
    std::string _storedUsername;
    
    void getUserDefaults();
    
    void login(std::string username, std::string password);
	
	
    
    bool shouldDisplayMessage = false;
    
    // - IMEDelegate
    void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    
protected:
    virtual void onSizeChanged() override;
    
public:
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
    //Delegate Functions
    void connectivityStateChanged(bool online) override;
    
    CREATE_FUNC(LoginScene);
};

NS_AZOOMEE_END

#endif
