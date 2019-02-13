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
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include "NavigationLayer.h"
#include "SettingsButton.h"

NS_AZOOMEE_BEGIN

enum class HQSceneType {CONTENT_FEED_HQ, LOCAL_CONTENT_HQ, ART_HQ, STORE_HQ, QUESTS_HQ, DEFAULT};

class HQScene : public Azoomee::Scene, TutorialDelegate
{
	typedef Azoomee::Scene Super;
private:
	void buildCoreUI();
	
protected:
	HQSceneType _type = HQSceneType::DEFAULT;
	
	std::string _hqCategory;
	
	SettingsButton* _settingsButton = nullptr;
	
	NavigationLayer* _navLayer = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	
	void setHQCategory(const std::string &hqCategory);
	
	HQSceneType getSceneType() const;
	
	CREATE_FUNC(HQScene);
	
	//delegate functions
	virtual void onTutorialStateChanged(const std::string& stateId) override;
};

NS_AZOOMEE_END


#endif /* HQScene_h */
