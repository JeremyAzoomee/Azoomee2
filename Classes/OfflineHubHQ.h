#ifndef __OFFLINEHUB_HQ_H__
#define __OFFLINEHUB_HQ_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"

class OfflineHubHQ : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubHQ);
    
    virtual bool init();
    void createOfflineHQ(std::vector<std::map<std::string, std::string>> gameDataList);
    
private:
    cocos2d::ui::ScrollView* createHorizontalScrollView();
    void addElementsToScrollView(std::vector<std::map<std::string, std::string>> gameDataList, cocos2d::ui::ScrollView* toBeAddedTo);
    void addArtAppElementToScrollView(cocos2d::ui::ScrollView* toBeAddedTo);
    void addListenerToArtElement(cocos2d::Layer* toBeAddedTo);
    
    cocos2d::Point startLocation;
    bool directionDecided;
    
    cocos2d::Point touchPoint;
    bool movedAway;
    bool iamtouched;
};

#endif
