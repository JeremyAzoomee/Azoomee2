//
//  ChildOnboardingScene.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 25/11/2019.
//

#ifndef ChildOnboardingScene_h
#define ChildOnboardingScene_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/Scene.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/RoundedRectSprite.h>
#include <TinizineCommon/API//API.h>
#include "ChildNameEntry.h"
#include "ChildAgeEntry.h"
#include "ChildCreator.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ChildOnboardingScene : public TZ::Scene, HttpRequestCreatorResponseDelegate
{
    typedef TZ::Scene Super;
public:
    enum State {ENTER_NAME, ENTER_AGE};
    
private:
    
    cocos2d::ui::Layout* _bgColour = nullptr;
    cocos2d::LayerGradient* _gradient = nullptr;
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::DrawNode* _patternHider = nullptr;
    
    cocos2d::ui::ImageView* _oomee = nullptr;
    cocos2d::ui::ImageView* _oomeeFace = nullptr;
    cocos2d::ui::ImageView* _oomeeArms = nullptr;
    cocos2d::ui::ImageView* _speechPoint = nullptr;
    
    ChildNameEntry* _nameEntry = nullptr;
    ChildAgeEntry* _ageEntry = nullptr;
    
    ChildCreatorRef _childCreator = nullptr;
    
    State _state = State::ENTER_NAME;
    
    void transitionState(const State& newState);
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ChildOnboardingScene);
    
    //-HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZ_END


#endif /* ChildOnboardingScene_h */
