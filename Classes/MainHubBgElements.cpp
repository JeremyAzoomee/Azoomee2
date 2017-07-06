#include "MainHubBgElements.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* MainHubBgElements::createScene()
{
    auto scene = Scene::create();
    auto layer = MainHubBgElements::create();
    scene->addChild(layer);

    return scene;
}

bool MainHubBgElements::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void MainHubBgElements::onEnter()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto funcCallAction = CallFunc::create([=](){
    
        auto myParticle = ParticleMeteor::create();
        myParticle->setSpeed(30);
        myParticle->setGravity(Vec2(0, -20));
        myParticle->setScale(1);
        myParticle->setPosVar(Vec2(2732, 5192));
        this->addChild(myParticle);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.1), funcCallAction, NULL));

    auto funcCallAction3 = CallFunc::create([=](){
        if(HQHistoryManager::getInstance()->noHistory()) addMainHubSideWiresToScreen(this, 3, 2);
        else addMainHubSideWiresToScreen(this);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.3), funcCallAction3, NULL));
    
    Node::onEnter();
}

NS_AZOOMEE_END
