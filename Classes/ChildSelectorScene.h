#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include "AwaitingAdultPinLayer.h"
#include "OfflineChecker.h"
#include <AzoomeeCommon/UI/MessageBox.h>

class ChildSelectorScene : public cocos2d::Layer, public AwaitingAdultPinLayerDelegate, public OfflineCheckerDelegate, public MessageBoxDelegate
{
public:
    CREATE_FUNC(ChildSelectorScene);
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    static cocos2d::Scene* createScene();
    
    //Delegate Functions
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
    void callDelegateFunction(float dt);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle,int buttonNumber);
    
    void connectivityStateChanged(bool online);
    
private:
    long _errorCode;
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    void addScrollViewForProfiles();
    cocos2d::Size getScrollviewInnerSize(float scrollviewWidth);
    cocos2d::ui::ScrollView *scrollView;
    
    void addVisualsToScene();
    void createSettingsButton();
    void addProfilesToScrollView();
    Layer *createChildProfileButton(std::string profileName, int oomeeNumber);
    cocos2d::Point positionElementOnScrollView(Layer *layerToBeAdded);
    void addListenerToProfileLayer(Node *profileLayer);
    
    void addNewChildButtonToScrollView();
    void addChildButtonPressed(Node* target);
    
    cocos2d::Point startTouchPosition;
    bool touchMovedAway;
};

#endif
