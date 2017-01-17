#ifndef __CHILDSELECTOR_SCENE_H__
#define __CHILDSELECTOR_SCENE_H__

#include "cocos2d.h"
#include "ui/UIScrollView.h"

class ChildSelectorScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(ChildSelectorScene);
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    static cocos2d::Scene* createScene(long errorCode);
    
private:
    void handleErrorCode(long errorCode);
    long _errorCode;
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    void addScrollViewForProfiles();
    cocos2d::Size getScrollviewInnerSize(float scrollviewWidth);
    cocos2d::ui::ScrollView *scrollView;
    
    void addVisualsToScene();
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
