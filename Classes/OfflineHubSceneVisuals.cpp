#include "OfflineHubSceneVisuals.h"

USING_NS_CC;

Scene* OfflineHubSceneVisuals::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubSceneVisuals::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubSceneVisuals::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}
