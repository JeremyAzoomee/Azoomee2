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
#include <AzoomeeCommon/UI/CTAButton.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class WelcomeScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	
	cocos2d::ui::Layout* _bgColour = nullptr;
	cocos2d::LayerGradient* _bottomGradient = nullptr;
	cocos2d::Sprite* _contentTiles = nullptr;
    cocos2d::Sprite* _fillColour = nullptr;
    
    cocos2d::ui::ImageView* _logo = nullptr;
    CTAButton* _button = nullptr;
    DynamicText* _text = nullptr;
	cocos2d::ui::Layout* _body = nullptr;
	
    cocos2d::ui::Layout* _loginLayout = nullptr;
    DynamicText* _loginButton = nullptr;
    DynamicText* _loginText = nullptr;
    
    cocos2d::Sprite* _oomee = nullptr;
	
    cocos2d::Vec2 _moveVec = cocos2d::Vec2(0,0);
	
protected:
	virtual void onSizeChanged() override;
	
public:
	
	void onEnter() override;
	void onExit() override;
    void update(float deltaT) override;
    
	virtual bool init() override;
	
	CREATE_FUNC(WelcomeScene);
};

NS_AZOOMEE_END

#endif /* WelcomeScene_h */
