#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include "cocos2d.h"
#include "ui/UIScrollView.h"

class ChildSelectorScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    void addScrollViewForProfiles();
    cocos2d::ui::ScrollView *scrollView;
    
    void addVisualsToScene();
    void addProfilesToScrollView();
    Layer *createChildProfileButton(std::string profileName, int oomeeNumber);
    cocos2d::Point positionElementOnScrollView(Layer *layerToBeAdded);
    void addListenerToProfileLayer(Layer *profileLayer);
    
    void addNewChildButtonToScrollView();
    
    cocos2d::Point startTouchPosition;
    bool touchMovedAway;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChildSelectorScene);
};

#endif // __CHILDSELECTOR_SCENE_H__
