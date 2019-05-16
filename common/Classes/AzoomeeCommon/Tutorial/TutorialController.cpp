//
//  TutorialController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 10/01/2019.
//

#include "TutorialController.h"
#include <algorithm>
#include "../UI/NotificationNodeDisplayManager.h"
#include "../Strings.h"
#include "../Data/Child/ChildDataProvider.h"

NS_AZOOMEE_BEGIN

static std::auto_ptr<TutorialController> sTutorialContollerSharedInstance;

// Tutorial step ids
const std::string TutorialController::kFTUGameHQContent = "ftu.gamehq.content";
const std::string TutorialController::kFTUVideoHQContent = "ftu.videohq.content";
const std::string TutorialController::kFTUGroupHQContent = "ftu.grouphq.content";
const std::string TutorialController::kFTUSpendRewards = "ftu.spendrewards";
const std::string TutorialController::kFTUShopEarnMoreRewards = "ftu.earnmorerewards";
const std::string TutorialController::kTutorialEnded = "tutorialend";
const std::string TutorialController::kCreateOomee = "om.create";
const std::string TutorialController::kConfirmOomee = "om.confirm";
const std::string TutorialController::kNameEntry = "child.name";
const std::string TutorialController::kAgeEntry = "child.age";
// Tutorial ids
const std::string TutorialController::kFTUOomeeTutorialID = "FTUOomeeTutorial";
const std::string TutorialController::kFTUAddChildID = "FTUAddChild";
const std::string TutorialController::kFTUPlayGameID = "FTUPlayGame";
const std::string TutorialController::kFTUWatchVideoID = "FTUWatchVideo";
const std::string TutorialController::kFTUShopID = "FTUShop";
const std::string TutorialController::kFTUPostPurchaseID = "FTUPostPurchase";
// Tutorials
const std::vector<std::string> TutorialController::kFTUAddChildTutorial = {kNameEntry,kAgeEntry};
const std::vector<std::string> TutorialController::kFTUOomeeTutorial = {kCreateOomee, kConfirmOomee};
const std::vector<std::string> TutorialController::kFTUGameTutorial = {kFTUGameHQContent};
const std::vector<std::string> TutorialController::kFTUVideoTutorial = {kFTUVideoHQContent, kFTUGroupHQContent};
const std::vector<std::string> TutorialController::kFTUShopTutorial = {kFTUSpendRewards};
const std::vector<std::string> TutorialController::kFTUPostPurchaseTutorial = {kFTUShopEarnMoreRewards};

// Tutorial storage map
const std::map<std::string, std::vector<std::string>> TutorialController::kTutorialMap = {
	{kFTUAddChildID,kFTUAddChildTutorial},
	{kFTUOomeeTutorialID,kFTUOomeeTutorial},
	{kFTUPlayGameID,kFTUGameTutorial},
	{kFTUWatchVideoID,kFTUVideoTutorial},
	{kFTUShopID,kFTUShopTutorial},
	{kFTUPostPurchaseID, kFTUPostPurchaseTutorial}
};

const std::map<std::string,std::pair<std::string,MessageLocation>> TutorialController::kDisplayMessageMap = {
	{kNameEntry, {"Hi! What should I call you?",MessageLocation::TOP_LEFT}},
	{kAgeEntry, {"How old are you?",MessageLocation::TOP_LEFT}},
	{kConfirmOomee, {"",MessageLocation::TOP_LEFT}},
	{kCreateOomee, {"",MessageLocation::TOP_LEFT}},
	{kFTUShopEarnMoreRewards, {"Try a new game or video and win more coins!",MessageLocation::TOP_LEFT}},
	{kFTUSpendRewards, {"Let’s buy a present",MessageLocation::TOP_LEFT}},
	{kFTUGroupHQContent, {"",MessageLocation::TOP_LEFT}},
	{kFTUVideoHQContent, {"",MessageLocation::TOP_LEFT}},
	{kFTUGameHQContent, {"",MessageLocation::TOP_LEFT}}
};

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
		displayMessageForTutorialState();
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
		displayMessageForTutorialState();
		
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
	_removingNode = _messagingNode;
	_messagingNode = nullptr;
	_removingNode->animateOutGuideAndMessage([this](){
		_removingNode->removeFromParent();
		_removingNode = nullptr;
	});
	
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

void TutorialController::displayMessageForTutorialState(const std::string& msg)
{
	std::string message = msg;
	if(msg == "")
	{
		if(kDisplayMessageMap.find(_activeTutorialStates.front()) != kDisplayMessageMap.end())
		{
			message = kDisplayMessageMap.at(_activeTutorialStates.front()).first;
		}
		message = _(message);
	}
	if(!_messagingNode)
	{
		_messagingNode = TutorialMessagingNode::create(message, kDisplayMessageMap.at(_activeTutorialStates.front()).second);
		NotificationNodeDisplayManager::getInstance()->addMessagingNode(_messagingNode);
		_messagingNode->animateInGuideAndMessage();
		_messagingNode->setVisible(message != "");
	}
	else
	{
		_messagingNode->setMessage(message);
		_messagingNode->setLocation(kDisplayMessageMap.at(_activeTutorialStates.front()).second);
		if(_messagingNode->isVisible())
		{
			_messagingNode->animateInMessage();
		}
		else
		{
			_messagingNode->animateInGuideAndMessage();
		}
		_messagingNode->setVisible(message != "");
	}
	
}

bool TutorialController::isTutorialCompleted(const std::string& tutorialId)
{
	return cocos2d::UserDefault::getInstance()->getBoolForKey((tutorialId + "_" + ChildDataProvider::getInstance()->getParentOrChildId()).c_str(), false);
}

void TutorialController::setTutorialCompleted(const std::string& tutorialId)
{
	cocos2d::UserDefault::getInstance()->setBoolForKey((tutorialId + "_" + ChildDataProvider::getInstance()->getParentOrChildId()).c_str(), true);
}

NS_AZOOMEE_END
