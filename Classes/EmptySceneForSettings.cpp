#include "EmptySceneForSettings.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN


Scene* EmptySceneForSettings::createScene()
{
    auto scene = Scene::create();
    auto layer = EmptySceneForSettings::create();
    scene->addChild(layer);
    
    return scene;
}

bool EmptySceneForSettings::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void EmptySceneForSettings::onEnterTransitionDidFinish()
{
    SettingsControlLayer::createFromChat();
}

NS_AZOOMEE_END
