#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/UI/RequestAdultPinLayer.h>
#include "OfflineChecker.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "ForceUpdateSingleton.h"

NS_AZOOMEE_BEGIN

class ChildSelectorScene : public Azoomee::Scene, public RequestAdultPinLayerDelegate, public OfflineCheckerDelegate, public MessageBoxDelegate, public Azoomee::HttpRequestCreatorResponseDelegate, public ForceUpdateDelegate
{
    typedef Azoomee::Scene Super;
public:
    CREATE_FUNC(ChildSelectorScene);
    
    static const std::string kSceneName;
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    void onExit() override;
    static Azoomee::Scene* createScene();
    
    //Delegate Functions
    void AdultPinCancelled(RequestAdultPinLayer* layer) override;
    void AdultPinAccepted(RequestAdultPinLayer* layer) override;
    void callDelegateFunction(float dt);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) override;
	void onForceUpdateCheckFinished(const ForceUpdateResult& result) override;
    
    void connectivityStateChanged(bool online) override;
    
    void setParentButtonVisible(bool visible);
    
    //Delegate functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
private:
    static const std::string kOomeeLayerName;
    
    cocos2d::Node* _contentNode = nullptr;
    
    cocos2d::Size _visibleSize;
    bool _isPortrait = false;
    cocos2d::ui::Button* _parentButton = nullptr;
    cocos2d::ui::Button* _addChildButton = nullptr;
    
    void addScrollViewForProfiles();
    cocos2d::Size getScrollviewInnerSize(float scrollviewWidth);
    cocos2d::ui::ScrollView *_scrollView = nullptr;
    
    void addVisualsToScene();
    void createSettingsButton();
    void addProfilesToScrollView();
    void addPrivacyButton();

    cocos2d::ui::Button* createChildProfileButton(const std::string& profileName, int oomeeNumber);
    cocos2d::Point positionElementOnScrollView(cocos2d::Node *layerToBeAdded);

    void addChildButtonPressed(cocos2d::Node* target);
    
    cocos2d::ui::Button* createParentProfileButton();
    
    cocos2d::Point _startTouchPosition;
    bool _touchMovedAway = false;
    bool _parentIconSelected = false;
    
    void addBackgroundToScreen();
    
    void refreshParentCookiesRequest();
    void getParentCookiesRequest();
    
    RequestAdultPinLayer* _awaitingAdultPinLayer = nullptr;
    void createAdultPinLayerWithDelegate();
    void removeAdultPinLayerDelegate();
    
    bool _firstTime = false;
    
protected:
    virtual void onSizeChanged() override;
};

NS_AZOOMEE_END

#endif
