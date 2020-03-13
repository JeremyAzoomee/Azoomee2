//
//  TutorialController.h
//  azoomee2
//
//  Created by Macauley on 10/01/2019.
//

#ifndef TutorialController_h
#define TutorialController_h

#include <TinizineCommon/Tinizine.h>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "TutorialMessagingNode.h"
#include "../Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class TutorialDelegate
{
public:
	virtual void onTutorialStateChanged(const std::string& stateId) = 0;
};

class TutorialController
{
public:
	// Tutorial step ids
	static const std::string kFTUGameHQContent;
	static const std::string kFTUVideoHQContent;
	static const std::string kFTUGroupHQContent;
	static const std::string kFTUSpendRewards;
	static const std::string kFTUShopEarnMoreRewards;
	static const std::string kTutorialEnded;
	static const std::string kCreateOomee;
	static const std::string kConfirmOomee;
	static const std::string kNameEntry;
	static const std::string kAgeEntry;
	//Tutorial ids
	static const std::string kFTUOomeeTutorialID;
	static const std::string kFTUAddChildID;
	static const std::string kFTUPlayGameID;
	static const std::string kFTUWatchVideoID;
	static const std::string kFTUShopID;
	static const std::string kFTUPostPurchaseID;
	
private:
	// Tutorials
	static const std::vector<std::string> kFTUAddChildTutorial;
	static const std::vector<std::string> kFTUOomeeTutorial;
	static const std::vector<std::string> kFTUGameTutorial;
	static const std::vector<std::string> kFTUVideoTutorial;
	static const std::vector<std::string> kFTUShopTutorial;
	static const std::vector<std::string> kFTUPostPurchaseTutorial;
	// Tutorial storage map
	static const std::map<std::string,std::vector<std::string>> kTutorialMap;
	// Tutorial message Strings Map
	static const std::map<std::string,std::pair<std::string,MessageLocation>> kDisplayMessageMap;
	
	std::vector<TutorialDelegate*> _delegates;
	bool _tutorialActive = false;
	std::vector<std::string> _activeTutorialStates;
	
	TutorialMessagingNode* _messagingNode = nullptr;
	TutorialMessagingNode* _removingNode = nullptr;
	
public:
	static TutorialController* getInstance(void);
	virtual ~TutorialController();
	bool init(void);
	
	void startTutorial(const std::string& tutorialID);
	void nextStep();
	void endTutorial();
	
	void registerDelegate(TutorialDelegate* delegate);
	void unRegisterDelegate(TutorialDelegate* delegate);
	
	bool isTutorialActive() const;
	std::string getCurrentState() const;
	
	bool isTutorialCompleted(const std::string& tutorialId);
	void setTutorialCompleted(const std::string& tutorialId);
	
	void displayMessageForTutorialState(const std::string& message = "");
	void highlightMessageString(const std::string& targetStr, const cocos2d::Color3B& highlightColour);
};

NS_AZ_END

#endif /* TutorialController_h */
