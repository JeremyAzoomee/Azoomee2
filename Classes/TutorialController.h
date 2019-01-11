//
//  TutorialController.h
//  azoomee2
//
//  Created by Macauley on 10/01/2019.
//

#ifndef TutorialController_h
#define TutorialController_h

#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class TutorialDelegate
{
public:
	virtual void onTutorialStateChanged(const std::string& stateId) = 0;
};

class TutorialController
{
public:
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
	
private:
	static const std::vector<std::string> kFTUNavTutorial;
	
	std::vector<TutorialDelegate*> _delegates;
	bool _tutorialActive = false;
	std::vector<std::string> _activeTutorialStates;
	
public:
	static TutorialController* getInstance(void);
	virtual ~TutorialController();
	bool init(void);
	
	void startTutorial();
	void nextStep();
	void endTutorial();
	
	void registerDelegate(TutorialDelegate* delegate);
	void unRegisterDelegate(TutorialDelegate* delegate);
	
	bool isTutorialActive() const;
	std::string getCurrentState() const;
	
};

NS_AZOOMEE_END

#endif /* TutorialController_h */
