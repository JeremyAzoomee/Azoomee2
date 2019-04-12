//
//  ChildSettingsScene.h
//  azoomee2
//
//  Created by Macauley on 13/02/2019.
//

#ifndef ChildSettingsScene_h
#define ChildSettingsScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class ChildSettingsScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	cocos2d::ui::Layout* _contentLayout = nullptr;
	// title bar
	cocos2d::ui::Layout* _titleLayout = nullptr;
	cocos2d::ui::Text* _titleText = nullptr;
	cocos2d::ui::Button* _titleBarButton = nullptr;
	// navigation buttons
	cocos2d::ui::Button* _childSelect = nullptr;
	cocos2d::ui::Button* _parentsArea = nullptr;
	
	cocos2d::LayerColor* _bgColour = nullptr;
	cocos2d::Sprite* _wireLeft = nullptr;
	cocos2d::Sprite* _wireRight = nullptr;
	
public:
	virtual bool init() override;
	virtual void onEnter() override;
	
	virtual void onSizeChanged() override;
	
	CREATE_FUNC(ChildSettingsScene);
	

};

NS_AZOOMEE_END

#endif /* ChildSettingsScene_h */
