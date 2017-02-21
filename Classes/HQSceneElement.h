#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "HQSceneElementVisual.h"

class HQSceneElement : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElement);
    static cocos2d::Scene* createScene();
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
