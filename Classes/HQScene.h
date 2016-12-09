#ifndef __HQSCENE_SCENE_H__
#define __HQSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/UIScrollView.h"

class HQScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    bool directionDecided;
    cocos2d::Point startLocation;
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);
    void setName(std::string name);
    void setBackground(std::string name);
    void setCategoryFromName(std::string name);
    
    cocos2d::Point calculatePositionForIcon(cocos2d::Point currentPosition, cocos2d::Size currentIconSize, cocos2d::Node *toBeAddedTo);
    
    cocos2d::ui::ScrollView* createVerticalScrollView();
    cocos2d::ui::ScrollView* createHorizontalScrollView(cocos2d::Size contentSize, cocos2d::Point position);
    void addElementToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, cocos2d::Point position, int category, int highlight, std::string imageName, std::string label);
    
    void startBuildingScrollViewBasedOnName();
    void createBidirectionalScrollView();
    void createMonodirectionalScrollView();
    
    std::vector<bool> scrollViewSpaceAllocation;
    cocos2d::Point getItemPositionForBidirectionalScrollView(int highlight);
    
    int category;
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HQScene);
};

#endif // __HQSCENE_SCENE_H__
