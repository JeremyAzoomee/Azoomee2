#ifndef __OFFLINEHUB_HQELEMENT_H__
#define __OFFLINEHUB_HQELEMENT_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "HQSceneElementVisual.h"

NS_AZOOMEE_BEGIN

class OfflineHubHQElement : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubHQElement);
    virtual bool init();
    void addHQSceneElement(std::string category, std::map<std::string, std::string>itemData, cocos2d::Vec2 shape, float delay);
    
private:
    HQSceneElementVisual* elementVisual;
    void addListenerToElement(std::map<std::string, std::string> itemData, bool preview);
    void startUpElementDependingOnType(std::map<std::string, std::string> itemData);
    
    cocos2d::Point touchPoint;
    bool movedAway = false;
    bool iamtouched = false;

};

NS_AZOOMEE_END

#endif
