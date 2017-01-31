#include "MainHubBgElements.h"
#include "ConfigStorage.h"
#include "HQHistoryManager.h"

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
    
    if(!HQHistoryManager::getInstance()->noHistory())
    {
        quickBuild();
        return true;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgGlow = Sprite::create("res/mainhub/bg_glow.png");
    bgGlow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    bgGlow->setOpacity(0);
    this->addChild(bgGlow);
    
    bgGlow->runAction(FadeIn::create(1));
    
    auto wireLeft = Sprite::create("res/mainhub/wire_left.png");
    wireLeft->setPosition(wireLeft->getContentSize().width / -2, visibleSize.height / 2 + origin.y);
    this->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/mainhub/wire_right.png");
    wireRight->setPosition(wireRight->getContentSize().width / 2 + visibleSize.width, visibleSize.height / 2 + origin.y);
    this->addChild(wireRight);
    
    float backgroundElementsPadding = 0.7;
    
    wireLeft->runAction(Sequence::create(DelayTime::create(3), EaseOut::create(MoveTo::create(1, Vec2(wireLeft->getContentSize().width / 2 * backgroundElementsPadding, visibleSize.height / 2 + origin.y)), 2), NULL));
    wireRight->runAction(Sequence::create(DelayTime::create(3), EaseOut::create(MoveTo::create(1, Vec2(visibleSize.width - wireRight->getContentSize().width / 2 * backgroundElementsPadding, visibleSize.height / 2 + origin.y)), 2), NULL));
    
    auto myParticle = ParticleMeteor::create();
    myParticle->setSpeed(30);
    myParticle->setGravity(Vec2(0, -20));
    myParticle->setScale(1);
    myParticle->setPosVar(Vec2(2732, 2048));
    this->addChild(myParticle);
    
    return true;
}

void MainHubBgElements::quickBuild()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgGlow = Sprite::create("res/mainhub/bg_glow.png");
    bgGlow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(bgGlow);
    
    float backgroundElementsPadding = 0.7;
    
    auto wireLeft = Sprite::create("res/mainhub/wire_left.png");
    wireLeft->setPosition(wireLeft->getContentSize().width / 2 * backgroundElementsPadding, visibleSize.height / 2 + origin.y);
    this->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/mainhub/wire_right.png");
    wireRight->setPosition(visibleSize.width - wireRight->getContentSize().width / 2 * backgroundElementsPadding, visibleSize.height / 2 + origin.y);
    this->addChild(wireRight);
    
    auto myParticle = ParticleMeteor::create();
    myParticle->setSpeed(30);
    myParticle->setGravity(Vec2(0, -20));
    myParticle->setScale(1);
    myParticle->setPosVar(Vec2(2732, 2048));
    this->addChild(myParticle);

}
