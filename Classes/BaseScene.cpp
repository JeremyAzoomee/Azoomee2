#include "BaseScene.h"
#include "SimpleAudioEngine.h"
#include "NavigationLayer.h"

#include "MainHubScene.h"
#include "HQScene.h"

#include "InterSceneLoader.h"

USING_NS_CC;

Scene* BaseScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BaseScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BaseScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto contentLayer = Layer::create();
    contentLayer->setPosition(0,0);
    contentLayer->setName("contentLayer");
    this->addChild(contentLayer);
    
    auto sMainHubScene = MainHubScene::create();
    sMainHubScene->setPosition(0,0);
    contentLayer->addChild(sMainHubScene);
    
    auto sVideoHQ = HQScene::create();
    sVideoHQ->setPosition(2732, 0);
    sVideoHQ->setName("sVideoHQ");
    sVideoHQ->createBidirectionalScrollView();
    contentLayer->addChild(sVideoHQ);
    
    auto sGameHQ = HQScene::create();
    sGameHQ->setPosition(0, -2048);
    sGameHQ->setName("sGameHQ");
    sGameHQ->createMonodirectionalScrollView();
    contentLayer->addChild(sGameHQ);
    
    auto sAudioHQ = HQScene::create();
    sAudioHQ->setPosition(-2732, 0);
    sAudioHQ->setName("sAudioHQ");
    sAudioHQ->createMonodirectionalScrollView();
    contentLayer->addChild(sAudioHQ);
    
    auto sArtsHQ = HQScene::create();
    sArtsHQ->setPosition(0, 2048);
    sArtsHQ->setName("sArtsHQ");
    sArtsHQ->createMonodirectionalScrollView();
    contentLayer->addChild(sArtsHQ);
    
    //Adding InterSceneLoader to top of all layers, but below main navigation, because we want to hide all layers, except for the menu
    
    auto interSceneLoader = InterSceneLoader::create();
    this->addChild(interSceneLoader);
    
    
    //Adding main menu to BaseScene (this), instead of contentLayer, as we don't want to move it, when panning contentlayer
    auto sNavigationLayer = NavigationLayer::create();
    sNavigationLayer->setPosition(0,0);
    this->addChild(sNavigationLayer);
    
    
    
    
    return true;
}


void BaseScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
