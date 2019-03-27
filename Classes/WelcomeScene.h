//
//  WelcomeScene.h
//  azoomee2
//
//  Created by Macauley on 22/01/2019.
//

#ifndef WelcomeScene_h
#define WelcomeScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/Strings.h>

NS_AZOOMEE_BEGIN

class WelcomeScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	
	cocos2d::ui::Layout* _welcomeLayout = nullptr;
	cocos2d::ui::Layout* _oomeeGuideLayout = nullptr;
	
	void createWelcomeUI();
	void createOomeeGuideUI();
	
protected:
	virtual void onSizeChanged() override;
	
public:
	
	void onEnter() override;
	void onExit() override;
	
	virtual bool init() override;
	
	CREATE_FUNC(WelcomeScene);
};

NS_AZOOMEE_END

#endif /* WelcomeScene_h */
