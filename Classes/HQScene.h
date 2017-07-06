#ifndef __HQSCENE_H__
#define __HQSCENE_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(HQScene);
    static cocos2d::Scene* createSceneForOfflineArtsAppHQ();
    virtual bool init();
    void startBuildingScrollViewBasedOnName();

private:
    void addGroupHQLogo();
    
    bool directionDecided = false;
    cocos2d::Point startLocation;
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);
    
    cocos2d::Point calculatePositionForIcon(cocos2d::Point currentPosition, cocos2d::Size currentIconSize, cocos2d::Node *toBeAddedTo);
    
    cocos2d::ui::ScrollView* createVerticalScrollView();
    cocos2d::Sprite* createVerticalScrollGradient();
    cocos2d::ui::ScrollView* createHorizontalScrollView(cocos2d::Size contentSize, cocos2d::Point position);
    void addTitleToHorizontalScrollView(std::string title, Node *toBeAddedTo, cocos2d::Point position);
    void addElementToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::map<std::string, std::string> itemData, int rowNumber, int itemNumber);

    void createBidirectionalScrollView();
    void createMonodirectionalScrollView();
    
    std::vector<bool> scrollViewSpaceAllocation;
    cocos2d::Point getItemPositionForBidirectionalScrollView(int highlight);
    
    int category;
    
};

NS_AZOOMEE_END

#endif
