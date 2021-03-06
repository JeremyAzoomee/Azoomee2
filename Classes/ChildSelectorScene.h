#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <cocos/ui/CocosGUI.h>
#include "OfflineChecker.h"
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include <AzoomeeCommon/UI/CTAButton.h>
#include <AzoomeeCommon/Data/Child/Child.h>
#include "ForceUpdateSingleton.h"

NS_AZOOMEE_BEGIN

class ChildSelectorScene : public Azoomee::Scene, public OfflineCheckerDelegate, public Azoomee::HttpRequestCreatorResponseDelegate, public ForceUpdateDelegate
{
    typedef Azoomee::Scene Super;
    
private:
    static const cocos2d::Size kBaseButtonSize;
    
    DynamicText* _titleText = nullptr;
    CTAButton* _settingsButton = nullptr;
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::LayerGradient* _gradient = nullptr;
    cocos2d::ui::Layout* _titleLayout = nullptr;

    cocos2d::ui::Layout* _childLayout = nullptr;
    DynamicText* _switchProfileText = nullptr;
    cocos2d::ui::PageView* _childPageView = nullptr;
    
    cocos2d::ui::Layout* createChildButton(const ChildRef& child, int childNum);
    
    void createChildButtons();

public:
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ChildSelectorScene);
    
    //Delegate Functions
    void onForceUpdateCheckFinished(const ForceUpdateResult& result) override;
    
    void connectivityStateChanged(bool online) override;
    
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
};

NS_AZOOMEE_END

#endif
