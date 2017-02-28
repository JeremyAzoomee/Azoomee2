#include "MainHubBgElements.h"
#include "ConfigStorage.h"
#include "HQHistoryManager.h"
#include "ElectricDreamsDecoration.h"

USING_NS_CC;

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
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    if(!HQHistoryManager::getInstance()->noHistory())
    {
        quickBuild();
        return;
    }
    
    auto funcCallAction = CallFunc::create([=](){
    
        auto myParticle = ParticleMeteor::create();
        myParticle->setSpeed(30);
        myParticle->setGravity(Vec2(0, -20));
        myParticle->setScale(1);
        myParticle->setPosVar(Vec2(2732, 2048));
        this->addChild(myParticle);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.1), funcCallAction, NULL));
    
    auto funcCallAction2 = CallFunc::create([=](){
        addGlowToScreen(this, 1);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.2), funcCallAction2, NULL));
    
    auto funcCallAction3 = CallFunc::create([=](){
        addSideWiresToScreen(this, 3, 2);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.3), funcCallAction3, NULL));
    
    Node::onEnter();
}

void MainHubBgElements::quickBuild()
{
    addGlowToScreen(this);
    addSideWiresToScreen(this);
    
    auto myParticle = ParticleMeteor::create();
    myParticle->setSpeed(30);
    myParticle->setGravity(Vec2(0, -20));
    myParticle->setScale(1);
    myParticle->setPosVar(Vec2(2732, 2048));
    this->addChild(myParticle);
}
