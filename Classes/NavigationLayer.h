#ifndef __NAVIGATION_LAYER_H__
#define __NAVIGATION_LAYER_H__

#include "cocos2d.h"

class NavigationLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void turnOffAllMenuItems();
    void setButtonOn(int i);
    void delayedSetButtonOn(float dt);
    void changeToScene(int target);
    
    std::vector<cocos2d::Point> circleMenuItemPositions;
    std::vector<cocos2d::Point> horizontalMenuItemPositions;
    std::vector<cocos2d::Vec4> menuItemColours;
    std::vector<std::string> menuItemNames;
    std::vector<int> currentPositions;
    
    void startRotatingTheMenu();
    void keepMenuPointsSideUp(float dt);
    
    void addMenuItemPositionsAndColours();
    
    // implement the "static create()" method manually
    CREATE_FUNC(NavigationLayer);
    
    void startLoadingHQScene(int categoryTag);
};

#endif // __NAVIGATION_LAYER_H__
