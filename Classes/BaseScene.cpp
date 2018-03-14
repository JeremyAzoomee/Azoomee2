#include "BaseScene.h"
#include "NavigationLayer.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "SimpleAudioEngine.h"

#include "MainHubScene.h"
#include "HQScene2.h"

#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Utils/PushNotificationsHandler.h>
#include <AzoomeeCommon/Utils/SpecialCalendarEventManager.h>
#include "ForceUpdateSingleton.h"

#include "HQDataParser.h"
#include "HQHistoryManager.h"
#include "BackEndCaller.h"

#include "IAPProductDataHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* BaseScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BaseScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool BaseScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void BaseScene::onEnterTransitionDidFinish()
{
    this->setName("baseLayer");
    
    IAPProductDataHandler::getInstance()->fetchProductData();
    
    Director::getInstance()->purgeCachedData();
    
    AudioMixer::getInstance()->playBackgroundMusic(HQ_BACKGROUND_MUSIC);
 
    addParticleElementsToBackground();
    startBuildingHQs();
    
    if(SpecialCalendarEventManager::getInstance()->isXmasTime())
    {
        addXmasDecoration();
    }
    
    if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
    {
        Director::getInstance()->getScheduler()->schedule([&](float dt){
            PushNotificationsHandler::getInstance()->enablePushNotifications();
        }, this, 4, 0, 0, false, "enablePush");
    }
    
    ForceUpdateSingleton::getInstance()->doForceUpdateLogic();
}

void BaseScene::startBuildingHQs()
{
    Layer *contentLayer = createContentLayer();
    
    //addMainHubScene(contentLayer);
    createHQScene2(ConfigStorage::kVideoHQName, contentLayer);            //We build each and every scene by its name. This is the name that we get from back-end.
    createHQScene2(ConfigStorage::kGameHQName, contentLayer);             //Probably worth moving these to configStorage?
    createHQScene2(ConfigStorage::kAudioHQName, contentLayer);
    createHQScene2(ConfigStorage::kArtAppHQName, contentLayer);
    createHQScene2(ConfigStorage::kGroupHQName, contentLayer);
    createHQScene2(ConfigStorage::kMixHQName, contentLayer);
    
    addNavigationLayer();  //The navigation layer is being added to "this", because that won't move with the menu.
}

void BaseScene::addMainHubScene(Node* toBeAddedTo)
{
    auto sMainHubScene = MainHubScene::create();
    sMainHubScene->setPosition(ConfigStorage::getInstance()->getHQScenePositions(ConfigStorage::kHomeHQName));
    sMainHubScene->setTag(0);
    toBeAddedTo->addChild(sMainHubScene);
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void BaseScene::createHQScene2(const std::string &sceneName, Node *toBeAddedTo)
{
    HQScene2* hqScene = HQScene2::create();
    hqScene->setHQCategory(sceneName);
    hqScene->setPosition(ConfigStorage::getInstance()->getHQScenePositions(sceneName));
    toBeAddedTo->addChild(hqScene);
    
    if(HQHistoryManager::getInstance()->getCurrentHQ() == sceneName)
    {
        hqScene->startBuildingScrollView();
    }
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
    
    if(!HQHistoryManager::getInstance()->noHistory())
    {
        sNavigationLayer->changeToScene(HQHistoryManager::getInstance()->getCurrentHQ(), 0.01);
    }
    else
    {
        sNavigationLayer->changeToScene(ConfigStorage::kDefaultHQName, 0.01);
    }
}

void BaseScene::addParticleElementsToBackground()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
        auto myParticle = ParticleMeteor::create();
    
    if(SpecialCalendarEventManager::getInstance()->isXmasTime())
    {
        myParticle->setColor(cocos2d::Color3B::WHITE);
        myParticle->setStartColor(cocos2d::Color4F::WHITE);
        myParticle->setEndColor(cocos2d::Color4F::WHITE);
        myParticle->setLife(20.0f);
    }
    
    myParticle->setSpeed(30);
    myParticle->setGravity(Vec2(0, -20));
    myParticle->setScale(1);
    myParticle->setPosVar(Vec2(2732, 5192));
    this->addChild(myParticle, 0);
}

void BaseScene::addXmasDecoration()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* snow1 = Sprite::create("res/xmasdecoration/snowPileLeft.png");
    snow1->setPosition(origin.x + snow1->getContentSize().width / 2, origin.y - snow1->getContentSize().height / 2);
    this->addChild(snow1, DECORATION_ZORDER);
    snow1->runAction(Sequence::create(DelayTime::create(0.3f), EaseOut::create(MoveTo::create(2, Vec2(snow1->getPosition().x, origin.y + snow1->getContentSize().height / 2)), 2.0f), NULL));
    
    Sprite *snow2 = Sprite::create("res/xmasdecoration/snowPileRight.png");
    snow2->setPosition(origin.x + visibleSize.width - snow2->getContentSize().width / 2, origin.y - snow2->getContentSize().height / 2);
    this->addChild(snow2, DECORATION_ZORDER);
    snow2->runAction(Sequence::create(DelayTime::create(0.5f), EaseOut::create(MoveTo::create(2, Vec2(snow2->getPosition().x, origin.y + snow2->getContentSize().height / 2)), 2.0f), NULL));
}

NS_AZOOMEE_END
