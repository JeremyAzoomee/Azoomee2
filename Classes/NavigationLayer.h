#ifndef __NAVIGATION_LAYER_H__
#define __NAVIGATION_LAYER_H__

#include "cocos2d.h"

class NavigationLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(NavigationLayer);
    
    void startLoadingGroupHQ(std::string uri);
    
private:
    
    //MenuItem creation phase
    cocos2d::Sprite* addMenuItemImage(int itemNumber);
    cocos2d::Sprite* addMenuItemInactive(int itemNumber, cocos2d::Node *toBeAddedTo);
    cocos2d::Sprite* addMenuItemActive(int itemNumber, cocos2d::Node *toBeAddedTo);
    void addListenerToMenuItem(Node *toBeAddedTo);
    void runDisplayAnimationForMenuItem(cocos2d::Node* node1, cocos2d::Node* node2);
    void moveMenuPointsToHorizontalState();
    void moveMenuPointsToCircleState();
    
    //Handling created menuitems or all menuitems
    void turnOffAllMenuItems();
    void setButtonOn(int i);
    void delayedSetButtonOn(float dt);
    void startLoadingHQScene(int categoryTag);
    void changeToScene(int target);
    
};

#endif
