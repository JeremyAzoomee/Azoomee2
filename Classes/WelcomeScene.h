//
//  WelcomeScene.h
//  azoomee2
//
//  Created by Macauley on 22/01/2019.
//

#ifndef WelcomeScene_h
#define WelcomeScene_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/Scene.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/TextButton.h>
#include <TinizineCommon/UI/DynamicText.h>
#include <TinizineCommon/UI/RoundedRectSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class WelcomeScene : public TZ::Scene
{
	typedef TZ::Scene Super;
private:
	
	cocos2d::ui::Layout* _bgColour = nullptr;
	cocos2d::LayerGradient* _bottomGradient = nullptr;
	cocos2d::Sprite* _contentTiles = nullptr;
    cocos2d::Sprite* _fillColour = nullptr;
    
    cocos2d::ui::ImageView* _logo = nullptr;
    TextButton* _button = nullptr;
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

NS_AZ_END

#endif /* WelcomeScene_h */
