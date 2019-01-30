//
//  NavigationScene.h
//  azoomee2
//
//  Created by Macauley on 25/01/2019.
//

#ifndef NavigationScene_h
#define NavigationScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>

NS_AZOOMEE_BEGIN

class NavigationScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	
	cocos2d::ui::Button* _gamesNavButton = nullptr;
	cocos2d::ui::Button* _videosNavButton = nullptr;
	cocos2d::ui::Button* _chatNavButton = nullptr;
	cocos2d::ui::Button* _galleryNavButton = nullptr;
	cocos2d::ui::Button* _favouritesNavButton = nullptr;
	cocos2d::ui::Button* _oomeeMakerNavButton = nullptr;
	cocos2d::ui::Button* _shopNavButton = nullptr;
	cocos2d::ui::Button* _questsNavButton = nullptr;
	
	cocos2d::ui::Button* createNavButton(const std::string& buttonText);
	
public:
	
	bool init() override;
	void onEnter() override;
	
	CREATE_FUNC(NavigationScene);
};

NS_AZOOMEE_END

#endif /* NavigationScene_h */
