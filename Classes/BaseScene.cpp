#include "BaseScene.h"
#include "SimpleAudioEngine.h"
#include "NavigationLayer.h"

#include "MainHubScene.h"
#include "HQScene.h"

#include "ConfigStorage.h"
#include "ChildDataProvider.h"
#include "HQDataParser.h"

#include "SimpleAudioEngine.h"

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
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/audio/bgmusic.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/audio/maintitle_in.mp3");
    
    if(!ChildDataProvider::getInstance()->getIsChildLoggedIn())  //if basescene is being run without a child logged in, preview mode has to be activated
    {
        HQDataParser::getInstance()->getPreviewContent(ConfigStorage::getInstance()->getServerUrl() + ConfigStorage::getInstance()->getPathForTag("PreviewHOME"), "HOME");
    }
    else
    {
        startBuildingHQs();
    }
    
    return true;
}

void BaseScene::startBuildingHQs()
{
    CCLOG("startBuildingHQs happen!!!!!!");
    
    Layer *contentLayer = createContentLayer();
    
    addMainHubScene(contentLayer);
    createHQScene("VIDEO HQ", contentLayer);            //We build each and every scene by its name. This is the name that we get from back-end.
    createHQScene("GAME HQ", contentLayer);             //Probably worth moving these to configStorage?
    createHQScene("AUDIO HQ", contentLayer);
    createHQScene("ARTS APP", contentLayer);
    createHQScene("GROUP HQ", contentLayer);
    
    addNavigationLayer();  //The navigation layer is being added to "this", because that won't move with the menu.
}

void BaseScene::addMainHubScene(Node* toBeAddedTo)
{
    auto sMainHubScene = MainHubScene::create();
    sMainHubScene->setPosition(ConfigStorage::getInstance()->getHQScenePositions("HOME"));
    sMainHubScene->setTag(0);
    toBeAddedTo->addChild(sMainHubScene);
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

void BaseScene::addNavigationLayer()
{
    //Adding main menu to BaseScene (this), instead of contentLayer, as we don't want to move it, when panning contentlayer
    auto sNavigationLayer = NavigationLayer::create();
    sNavigationLayer->setPosition(ConfigStorage::getInstance()->getHQScenePositions("NavigationLayer"));
    sNavigationLayer->setName("NavigationLayer");
    this->addChild(sNavigationLayer);
}
