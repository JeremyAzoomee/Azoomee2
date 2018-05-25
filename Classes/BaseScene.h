#ifndef __BASESCENE_SCENE_H__
#define __BASESCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "network/HttpClient.h"

NS_AZOOMEE_BEGIN

class BaseScene : public Azoomee::Scene
{
    typedef Azoomee::Scene Super;
public:
    CREATE_FUNC(BaseScene);
    
    virtual bool init() override;
    
    void startBuildingHQs();

protected:
    virtual void onSizeChanged() override;
    
private:
    void createHQScene2(const std::string &sceneName, Node* toBeAddedTo);
    cocos2d::Layer* createContentLayer();
    void addNavigationLayer();
    void onEnterTransitionDidFinish() override;
    void addParticleElementsToBackground();
    
    void addXmasDecoration();
};

NS_AZOOMEE_END

#endif
