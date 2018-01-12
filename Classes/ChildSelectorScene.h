#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "ui/UIScrollView.h"
#include "AwaitingAdultPinLayer.h"
#include "OfflineChecker.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

class ChildSelectorScene : public cocos2d::Layer, public AwaitingAdultPinLayerDelegate, public OfflineCheckerDelegate, public MessageBoxDelegate, public Azoomee::HttpRequestCreatorResponseDelegate
{
public:
    CREATE_FUNC(ChildSelectorScene);
    
    //event name tag for billing data recieved.  event fired from BackEndCaller::onUpdateBillingDataAnswerReceived
    static const char* const kBillingDataRecievedEvent;
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    void onExit();
    static cocos2d::Scene* createScene();
    
    //Delegate Functions
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
    void callDelegateFunction(float dt);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    void connectivityStateChanged(bool online);
    
    //Delegate functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);
    
private:
    cocos2d::Vec2 _origin;
    cocos2d::Size _visibleSize;
    
    void addScrollViewForProfiles();
    cocos2d::Size getScrollviewInnerSize(float scrollviewWidth);
    cocos2d::ui::ScrollView *_scrollView = nullptr;
    
    cocos2d::EventListenerCustom* _billingDataRecievedListener = nullptr;
    void addBillingDataRecievedListener();
    
    void addVisualsToScene();
    void createSettingsButton();
    void addProfilesToScrollView();
    void addPrivacyButton();
    Layer *createChildProfileButton(std::string profileName, int oomeeNumber);
    cocos2d::Point positionElementOnScrollView(Layer *layerToBeAdded);
    void addListenerToProfileLayer(Node *profileLayer);
    
    Layer* createNewProfileButton();
    void addChildButtonPressed(Node* target);
    
    Layer* createParentProfileButton();
    
    cocos2d::Point _startTouchPosition;
    bool _touchMovedAway = false;
    bool _parentIconSelected = false;
    
    void addBackgroundToScreen();
    
    void refreshParentCookiesRequest();
    void getParentCookiesRequest();
};

NS_AZOOMEE_END

#endif
