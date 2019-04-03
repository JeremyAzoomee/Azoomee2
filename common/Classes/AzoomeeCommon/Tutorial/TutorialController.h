//
//  TutorialController.h
//  azoomee2
//
//  Created by Macauley on 10/01/2019.
//

#ifndef TutorialController_h
#define TutorialController_h

#include "../Azoomee.h"
#include <string>
#include <memory>
#include <vector>
#include <map>

NS_AZOOMEE_BEGIN

class TutorialDelegate
{
public:
	virtual void onTutorialStateChanged(const std::string& stateId) = 0;
};

class TutorialController
{
public:
	// Tutorial step ids
	static const std::string kFTUGameHQNav;
	static const std::string kFTUGameHQContent;
	static const std::string kFTUVideoHQNav;
	static const std::string kFTUVideoHQContent;
	static const std::string kFTUGroupHQContent;
	static const std::string kFTUGroupHQBack;
	static const std::string kFTURewards;
	static const std::string kFTUSpendRewards;
	static const std::string kFTUEarnMoreRewards;
	static const std::string kTutorialEnded;
	static const std::string kCreateOomee;
	static const std::string kConfirmOomee;
	static const std::string kNameEntry;
	static const std::string kAgeEntry;
	//Tutorial ids
	static const std::string kFTUNavTutorialID;
	static const std::string kFTUOomeeTutorialID;
	
private:
	// Tutorials
	static const std::vector<std::string> kFTUNavTutorial;
	static const std::vector<std::string> kFTUOomeeTutorial;
	// Tutorial storage map
	static const std::map<std::string,std::vector<std::string>> kTutorialMap;
	
	std::vector<TutorialDelegate*> _delegates;
	bool _tutorialActive = false;
	std::vector<std::string> _activeTutorialStates;
	
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
	
};

NS_AZOOMEE_END

#endif /* TutorialController_h */