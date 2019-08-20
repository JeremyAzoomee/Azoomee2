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
//#include "NavigationLayer.h"
#include "NavigationBar.h"
#include "SettingsButton.h"
#include "UserTypeMessagingLayer.h"
#include "CoinDisplay.h"

NS_AZOOMEE_BEGIN

enum class HQSceneType {CONTENT_FEED_HQ, LOCAL_CONTENT_HQ, ART_HQ, STORE_HQ, QUESTS_HQ, DEFAULT};

class HQScene : public Azoomee::Scene, TutorialDelegate
{
	typedef Azoomee::Scene Super;
private:
	void buildCoreUI();
	void addParticleElementsToBackground();
	void addXmasDecoration();

	cocos2d::Sprite* _verticalScrollGradient = nullptr;
	
protected:
	static const std::string kTutHandName;
	
	HQSceneType _type = HQSceneType::DEFAULT;
	
	std::string _hqCategory;
	
	//NavigationLayer* _navLayer = nullptr;
    NavigationBar* _navBar = nullptr;
    
	UserTypeMessagingLayer* _messagingLayer = nullptr;
	bool _showingMessagingLayer = true;
	
	CoinDisplay* _coinDisplay = nullptr;
	
	cocos2d::EventListenerCustom* _rewardRedeemedListener = nullptr;
	
	bool _initialised = false;
	bool _isPortrait = false;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void onSizeChanged() override;
	
	void setHQCategory(const std::string &hqCategory);
	
	HQSceneType getSceneType() const;
	
	CREATE_FUNC(HQScene);
	
	//delegate functions
	virtual void onTutorialStateChanged(const std::string& stateId) override;
};

NS_AZOOMEE_END


#endif /* HQScene_h */
