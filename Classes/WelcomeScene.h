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
	
	cocos2d::LayerColor* _bgColour = nullptr;
	cocos2d::Sprite* _wires = nullptr;
	cocos2d::Sprite* _bottomGradient = nullptr;
	cocos2d::Sprite* _contentTiles = nullptr;
	cocos2d::ui::ImageView* _logo = nullptr;
	cocos2d::ui::Button* _button = nullptr;
	cocos2d::Label* _text = nullptr;
	cocos2d::ui::Layout* _textHolder = nullptr;
	cocos2d::ui::Layout* _body = nullptr;
	
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
