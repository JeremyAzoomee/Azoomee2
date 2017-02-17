#include "OfflineGameSearch.h"

USING_NS_CC;

Scene* OfflineGameSearch::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineGameSearch::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineGameSearch::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}
