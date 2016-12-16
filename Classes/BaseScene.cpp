#include "BaseScene.h"
#include "SimpleAudioEngine.h"
#include "NavigationLayer.h"

#include "MainHubScene.h"
#include "HQScene.h"

#include "ConfigStorage.h"

USING_NS_CC;

Scene* BaseScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BaseScene::create();
    layer->setName("baseLayer");
    scene->addChild(layer);
    
    return scene;
}

bool BaseScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Layer *contentLayer = createContentLayer();         //We use contentLayer to hold all HQ-s, to be able to pan between them.
    addMainHubScene(contentLayer);
    
    createHQScene("VIDEO HQ", contentLayer);            //We build each and every scene by its name. This is the name that we get from back-end.
    createHQScene("GAME HQ", contentLayer);             //Probably worth moving these to configStorage?
    createHQScene("AUDIO HQ", contentLayer);
    createHQScene("ARTS APP", contentLayer);
    
    addNavigationLayer();                               //The navigation layer is being added to "this", because that won't move with the menu.
    
    return true;
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void BaseScene::createHQScene(std::string sceneName, Node *toBeAddedTo)
{
    auto hqScene = HQScene::create();
    hqScene->setPosition(ConfigStorage::getInstance()->getHQScenePositions(sceneName));
    hqScene->setName(sceneName);
    toBeAddedTo->addChild(hqScene);
}

Layer* BaseScene::createContentLayer()
{
    auto contentLayer = Layer::create();
    contentLayer->setPosition(ConfigStorage::getInstance()->getHQScenePositions("contentLayer"));
    contentLayer->setName("contentLayer");
    this->addChild(contentLayer);
    
    return contentLayer;
}

void BaseScene::addMainHubScene(Node* toBeAddedTo)
{
    auto sMainHubScene = MainHubScene::create();
    sMainHubScene->setPosition(ConfigStorage::getInstance()->getHQScenePositions("HOME"));
    sMainHubScene->setName("HOME");
    sMainHubScene->setTag(0);
    toBeAddedTo->addChild(sMainHubScene);
}

void BaseScene::addNavigationLayer()
{
    //Adding main menu to BaseScene (this), instead of contentLayer, as we don't want to move it, when panning contentlayer
    auto sNavigationLayer = NavigationLayer::create();
    sNavigationLayer->setPosition(ConfigStorage::getInstance()->getHQScenePositions("NavigationLayer"));
    sNavigationLayer->setName("NavigationLayer");
    this->addChild(sNavigationLayer);
}
