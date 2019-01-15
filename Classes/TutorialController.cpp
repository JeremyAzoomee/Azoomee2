//
//  TutorialController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 10/01/2019.
//

#include "TutorialController.h"

NS_AZOOMEE_BEGIN

static std::auto_ptr<TutorialController> sTutorialContollerSharedInstance;

// Tutorial step ids
const std::string TutorialController::kFTUGameHQNav = "ftu.gamehq.nav";
const std::string TutorialController::kFTUGameHQContent = "ftu.gamehq.content";
const std::string TutorialController::kFTUVideoHQNav = "ftu.videohq.nav";
const std::string TutorialController::kFTUVideoHQContent = "ftu.videohq.content";
const std::string TutorialController::kFTUGroupHQContent = "ftu.grouphq.content";
const std::string TutorialController::kFTUGroupHQBack = "ftu.grouphq.back";
const std::string TutorialController::kFTURewards = "ftu.rewards";
const std::string TutorialController::kFTUSpendRewards = "ftu.spendrewards";
const std::string TutorialController::kFTUEarnMoreRewards = "ftu.earnmorerewards";
const std::string TutorialController::kTutorialEnded = "tutorialend";
// Tutorial ids
const std::string TutorialController::kFTUNavTutorialID = "FTUNavTutorial";
// Tutorials
const std::vector<std::string> TutorialController::kFTUNavTutorial = {kFTUGameHQNav,kFTUGameHQContent,kFTUVideoHQNav,kFTUVideoHQContent,kFTUGroupHQContent,kFTUGroupHQBack};
// Tutorial storage map
const std::map<std::string, std::vector<std::string>> TutorialController::kTutorialMap = {{kFTUNavTutorialID,kFTUNavTutorial}};

TutorialController* TutorialController::getInstance()
{
	if(!sTutorialContollerSharedInstance.get())
	{
		sTutorialContollerSharedInstance.reset(new TutorialController());
		sTutorialContollerSharedInstance->init();
	}
	return sTutorialContollerSharedInstance.get();
}

TutorialController::~TutorialController(void)
{
	
}

bool TutorialController::init(void)
{
	return true;
}

void TutorialController::startTutorial(const std::string& tutorialID)
{
	if(kTutorialMap.find(tutorialID) != kTutorialMap.end())
	{
		_tutorialActive = true;
		_activeTutorialStates = kTutorialMap.at(tutorialID);
		for(auto delegate : _delegates)
		{
			delegate->onTutorialStateChanged(_activeTutorialStates.front());
		}
	}
}
void TutorialController::nextStep()
{
	if(_activeTutorialStates.size() > 1)
	{
		_activeTutorialStates.erase(_activeTutorialStates.begin());
		
		//do tut visual overlay update here
		
		for(auto delegate : _delegates)
		{
			delegate->onTutorialStateChanged(_activeTutorialStates.front());
		}
	}
	else
	{
		endTutorial();
	}
}
void TutorialController::endTutorial()
{
	_tutorialActive = false;
	_activeTutorialStates.clear();
	for(auto delegate : _delegates)
	{
		delegate->onTutorialStateChanged(kTutorialEnded);
	}
}

void TutorialController::registerDelegate(TutorialDelegate* delegate)
{
	auto it = std::find(_delegates.begin(), _delegates.end(), delegate);
	if(it == _delegates.end())
	{
		_delegates.push_back(delegate);
	}
}
void TutorialController::unRegisterDelegate(TutorialDelegate* delegate)
{
	auto it = std::find(_delegates.begin(), _delegates.end(), delegate);
	if(it != _delegates.end())
	{
		_delegates.erase(it);
	}
}

bool TutorialController::isTutorialActive() const
{
	return _tutorialActive;
}
std::string TutorialController::getCurrentState() const
{
	return _activeTutorialStates.front();
}

NS_AZOOMEE_END
