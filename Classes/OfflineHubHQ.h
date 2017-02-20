#ifndef __OFFLINEHUB_HQ_H__
#define __OFFLINEHUB_HQ_H__

#include "cocos2d.h"
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
    //void addListenerToScrollView(cocos2d::ui::ScrollView* toBeAddedTo);
    
    cocos2d::Point startLocation;
    bool directionDecided;
};

#endif
