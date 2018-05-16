#include "EmptySceneForSettings.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN


Scene* EmptySceneForSettings::createScene(SettingsOrigin origin)
{
    auto scene = Scene::create();
    auto layer = EmptySceneForSettings::create();
    layer->_origin = origin;
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
    switch (_origin) {
        case CHAT:
            SettingsControlLayer::createFromChat();
            break;
        case MAIN_APP:
            SettingsControlLayer::create();
            break;
    }
    
}

NS_AZOOMEE_END
