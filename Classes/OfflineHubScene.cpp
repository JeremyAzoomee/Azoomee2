#include "OfflineHubScene.h"
#include "OfflineHubSceneVisuals.h"
#include "OfflineHubHQ.h"
#include "OfflineExit.h"

USING_NS_CC;

Scene* OfflineHubScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void OfflineHubScene::onEnterTransitionDidFinish()
{
    addVisuals();
    addOfflineExit();
    addHubHQ();
}

//All calls are private below this line

void OfflineHubScene::addVisuals()
{
    auto offlineHubSceneVisuals = OfflineHubSceneVisuals::create();
    this->addChild(offlineHubSceneVisuals);
}

void OfflineHubScene::addOfflineExit()
{
    
}

void OfflineHubScene::addHubHQ()
{
    
}
