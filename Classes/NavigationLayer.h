#ifndef __NAVIGATION_LAYER_H__
#define __NAVIGATION_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "DisplayChildNameLayer.h"


NS_AZOOMEE_BEGIN

class NavigationLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
public:
    CREATE_FUNC(NavigationLayer);
    static cocos2d::Scene* createScene();
    virtual bool init();
    void startLoadingGroupHQ(std::string uri);
    void changeToScene(int target, float duration);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    ElectricDreamsButton *previewLoginButton;
    ElectricDreamsButton *previewSignUpButton;
    ElectricDreamsButton *settingsButton;
    ElectricDreamsButton *returnToChildSelectorButton;

    //MenuItem creation phase
    cocos2d::Sprite* addMenuItemImage(int itemNumber);
    cocos2d::Sprite* addMenuItemInactive(int itemNumber, cocos2d::Node *toBeAddedTo);
    cocos2d::Sprite* addMenuItemActive(int itemNumber, cocos2d::Node *toBeAddedTo);
    void addListenerToMenuItem(Node *toBeAddedTo);
    
    void runDisplayAnimationForMenuItem(cocos2d::Node* node1, cocos2d::Node* node2);
    void runDisplayAnimationForMenuItemQuick(cocos2d::Node* node1, cocos2d::Node* node2);
    
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
    void startLoadingHQScene(int categoryTag);
    void addBackButtonToNavigation();
    void removeBackButtonFromNavigation();
    void addListenerToBackButton(cocos2d::Node* toBeAddedTo);

    //Handle arts app
    void loadArtsAppHQ();
    
    //memory management
    void cleanUpPreviousHQ();
};

NS_AZOOMEE_END

#endif
