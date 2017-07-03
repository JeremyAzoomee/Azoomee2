#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include "AzoomeeChat/AzoomeeChat.h"
#include "Auth/AuthAPI.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/UIScrollView.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/Scene.h>


NS_AZOOMEE_CHAT_BEGIN

/**
 * A simple scene to select a child profile for the currently logged in account.
 * Based off azoomee2/ChildSelectorScene.
 */
class ChildSelectorScene : public Azoomee::Scene, public ElectricDreamsButtonDelegate, public AuthAPIObserver, public MessageBoxDelegate
{
private:
    typedef Azoomee::Scene Super;
    
    /// Alot of old Azoomee stuff uses Director origin automatically, but with Azoomee::Scene we already offset origin
    /// So we use a root layer to offset
    cocos2d::Layer* wiresLayer = nullptr;
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    ElectricDreamsButton* backButton = nullptr;
    cocos2d::ui::ScrollView *scrollView = nullptr;
    cocos2d::Label* titleLabel = nullptr;
    
    cocos2d::Point startTouchPosition;
    bool touchMovedAway = false;
    
    
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
    void onAuthAPIRequestFailed(const std::string& requestTag, long errorCode) override;
    
    // - MessageBoxDelegate
    void MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle) override;

protected:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged() override;
    
public:
    
    CREATE_FUNC(ChildSelectorScene);
    
};

NS_AZOOMEE_CHAT_END

#endif
