#include "OfflineHubHQElement.h"

USING_NS_CC;

Scene* OfflineHubHQElement::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubHQElement::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubHQElement::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void OfflineHubHQElement::onEnter()
{
    
    
    Node::onEnter();
}
