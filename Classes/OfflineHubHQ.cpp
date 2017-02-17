#include "OfflineHubHQ.h"

USING_NS_CC;

Scene* OfflineHubHQ::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubHQ::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubHQ::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}
