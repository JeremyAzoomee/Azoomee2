#ifndef __OFFLINEHUB_HQELEMENT_H__
#define __OFFLINEHUB_HQELEMENT_H__

#include "cocos2d.h"
#include "HQSceneElementVisual.h"

class OfflineHubHQElement : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubHQElement);
    virtual bool init();
    void addHQSceneElement(std::string category, std::map<std::string, std::string>itemData, cocos2d::Vec2 shape, float delay);
    
private:
    HQSceneElementVisual* elementVisual;
    void addListenerToElement(std::string uri, std::string contentId, std::string category, std::string title, std::string description, std::string type, bool preview);
    void startUpElementDependingOnType(std::string uri, std::string contentId, std::string category);
    
    cocos2d::Point touchPoint;
    bool movedAway;
    bool iamtouched;

};

#endif
