#include "OfflineHubScene.h"

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
