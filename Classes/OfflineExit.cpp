#include "OfflineExit.h"

USING_NS_CC;

Scene* OfflineExit::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineExit::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineExit::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    
    return true;
}

void OfflineExit::onEnter()
{
    
    Node::onEnter();
}

void OfflineExit::addOfflineLogoToScreen()
{
    
    
    auto offlineIcon = Sprite::create("res/offline/offlineIcon.png");
    offlineIcon->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height - offlineIcon->getContentSize().height / 1.5);
    offlineIcon->setOpacity(0);
    this->addChild(offlineIcon);
    
    offlineIcon->runAction(Sequence::create(FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

void OfflineExit::addOfflineTextToScren()
{
    auto offlineText = Label::createWithTTF("Shit, you are offline", "fonts/azoomee.ttf", 30);
    offlineText->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.6);
}

void OfflineExit::addExitButtonToScreen()
{
    
}
