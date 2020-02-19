#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include "OfflineChecker.h"
#include "LoginEntry.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class LoginScene : public TZ::Scene, public cocos2d::IMEDelegate, public OfflineCheckerDelegate
{
    typedef TZ::Scene Super;
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

NS_AZ_END

#endif
