//
//  ContentFeedHQScene.h
//  azoomee2
//
//  Created by Macauley on 29/01/2019.
//

#ifndef ContentFeedHQScene_h
#define ContentFeedHQScene_h

#include "HQScene.h"
#include "HQSceneElement.h"

NS_AZOOMEE_BEGIN

class ContentFeedHQScene : public HQScene
{
	typedef HQScene Super;
private:
	static const float kSpaceForPrivacyPolicy;
	static const std::string kGroupLogoName;
	
	cocos2d::Node* _contentNode = nullptr;
	
	cocos2d::ui::ScrollView* _contentScrollview = nullptr;
	std::vector<std::vector<HQSceneElement>> _contentIcons;
	
	void createContentScrollview();
	
	bool showingPostContentCTARequired();
	bool startingReviewProcessRequired();
	
	cocos2d::Sprite* createGradientForScrollView(float scrollViewWith);
	
	void addGroupHQLogo();
	
public:
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onSizeChanged() override;
	
	CREATE_FUNC(ContentFeedHQScene);
	
	//delegate functions
	virtual void onTutorialStateChanged(const std::string& stateId) override;
};

NS_AZOOMEE_END

#endif /* ContentFeedHQScene_h */
