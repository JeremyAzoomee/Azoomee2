#ifndef __NAVIGATION_LAYER_H__
#define __NAVIGATION_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "DisplayChildNameLayer.h"
#include <AzoomeeChat/ChatAPI.h>
#include <AzoomeeChat/Data/Message.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

NS_AZOOMEE_BEGIN

class NavigationLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
public:
    CREATE_FUNC(NavigationLayer);
    static cocos2d::Scene* createScene();
    virtual bool init();
    void startLoadingGroupHQ(std::string uri);
    void changeToScene(HubTargetTagNumber target, float duration);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
    void onExit();
    
    void showNotificationBadge();
    void hideNotificationBadge();
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    ElectricDreamsButton *previewLoginButton;
    ElectricDreamsButton *previewSignUpButton;
    ElectricDreamsButton *settingsButton;
    ElectricDreamsButton *returnToChildSelectorButton;

    //MenuItem creation phase
    cocos2d::Sprite* addMenuItemHolder(int itemNumber);
    cocos2d::Sprite* addMenuItemCircle(int itemNumber, cocos2d::Node *toBeAddedTo);
    cocos2d::Sprite* addMenuItemInactive(int itemNumber, cocos2d::Node *toBeAddedTo);
    cocos2d::Sprite* addMenuItemActive(int itemNumber, cocos2d::Node *toBeAddedTo);
    void addListenerToMenuItem(Node *toBeAddedTo);
    
    void runDisplayAnimationForMenuItem(cocos2d::Node* node1, bool quick);
    
    void moveMenuPointsToHorizontalState(float duration);
    void moveMenuPointsToHorizontalStateInGroupHQ(float duration);
    void moveMenuPointsToCircleState(float duration);
    
    //---------TOP OBJECTS, SETTINGS/BACK TO CHILD SELECTOR----------
    void createTopObjects();
    void topObjectsOffScreen();
    void topObjectsOnScreen();
    
    //--------PREVIEW BUTTONS--------
    void createPreviewLoginButton();
    void createPreviewSignUpButton();
    
    //Handling created menuitems or all menuitems
    void turnOffAllMenuItems();
    void turnOnMenuItem(int tagNumber);
    
    void setButtonOn(int i);
    void delayedSetButtonOn(float dt);
    void startLoadingHQScene(HubTargetTagNumber target);
    void addBackButtonToNavigation();
    void removeBackButtonFromNavigation();
    void addListenerToBackButton(cocos2d::Node* toBeAddedTo);

    //Handle arts app
    void loadArtsAppHQ();
    
    //memory management
    void cleanUpPreviousHQ();
    
    //chat notifications
    void addNotificationBadgeToChatIcon(cocos2d::Node* chatIcon);
    
    void onEnter();
};

NS_AZOOMEE_END

#endif
