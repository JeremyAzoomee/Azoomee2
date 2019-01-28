//
//  HQScene.h
//  azoomee2
//
//  Created by Macauley on 25/01/2019.
//

#ifndef HQScene_h
#define HQScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "HQSceneElement.h"

NS_AZOOMEE_BEGIN

class HQScene : public Azoomee::Scene
{
	typedef Azoomee:::Scene Super;
private:
	std::string _hqCategory;
	cocos2d::ui::ScrollView* _contentScrollView = nullptr;
	std::vector<std::vector<HQSceneElement*>> _contentItems;
	
	cocos2d::ui::Button* _homeButton = nullptr;
	
	void buildCoreUI();
	void buildScrollView();
	
public:
	
	bool init() override;
	void onEnter() override;
	
	void setHQCategory(const std::string &hqCategory);
	
	CREAT_FUNC(HQScene);
};

NS_AZOOMEE_END


#endif /* HQScene_h */
