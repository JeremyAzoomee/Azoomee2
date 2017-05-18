#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include "AzoomeeChat/AzoomeeChat.h"
#include "Auth/AuthAPI.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/UIScrollView.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/Scene.h>


NS_AZOOMEE_CHAT_BEGIN

/**
 * A simple scene to select a child profile for the currently logged in account.
 * Based off azoomee2/ChildSelectorScene.
 */
class ChildSelectorScene : public Azoomee::Scene, public ElectricDreamsButtonDelegate, public AuthAPIObserver
{
private:
    typedef Azoomee::Scene Super;
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    ElectricDreamsButton* backButton;
    cocos2d::ui::ScrollView *scrollView;
    
    cocos2d::Point startTouchPosition;
    bool touchMovedAway;
    
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void addScrollViewForProfiles();
    cocos2d::Size getScrollviewInnerSize(float scrollviewWidth);
    
    void addVisualsToScene();
    void addButtonsScene();
    void addProfilesToScrollView();
    cocos2d::Layer* createChildProfileButton(std::string profileName, int oomeeNumber);
    cocos2d::Point positionElementOnScrollView(cocos2d::Layer* layerToBeAdded);
    void addListenerToProfileLayer(cocos2d::Node* profileLayer);
    
    // - ElectricDreamsButtonDelegate
    void buttonPressed(ElectricDreamsButton* button) override;
    
    // - AuthAPIObserver
    void onAuthAPIGetAvailableChildren() override;
    void onAuthAPIChildLogin() override;
    
public:
    
    CREATE_FUNC(ChildSelectorScene);
    
};

NS_AZOOMEE_CHAT_END

#endif
