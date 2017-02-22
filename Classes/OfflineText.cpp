#include "OfflineText.h"

USING_NS_CC;

Scene* OfflineText::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineText::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineText::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    
    return true;
}

void OfflineText::onEnter()
{
    Node::onEnter();
}

//--------------------------------------All methods beyond this line are private

void OfflineText::createForLogin()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen("Oh No! You are offline");
    addTextBodyToScreen("You need to be online to use this app.\nCheck your connection and try again,\nor enter offline mode.");
}

void OfflineText::createForOfflineHub()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen("Oh No! You are offline");
    addTextBodyToScreen("You need to be online to use this app.\nCheck your connection and try again.\n\n\nIn the meantime you can still enjoy these.");
}

void OfflineText::addOfflineLogoToScreen()
{
    auto offlineIcon = Sprite::create("res/offline/offlineIcon.png");
    offlineIcon->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height - offlineIcon->getContentSize().height / 1.1);
    offlineIcon->setOpacity(255);
    this->addChild(offlineIcon);
}

void OfflineText::addTextTitleToScreen(std::string text)
{
    auto offlineTextTitle = Label::createWithTTF(text, "fonts/azoomee.ttf", 84);
    offlineTextTitle->setCascadeOpacityEnabled(true);
    offlineTextTitle->setColor(Color3B(28,244,244));
    offlineTextTitle->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.75);
    offlineTextTitle->setOpacity(0);
    
    this->addChild(offlineTextTitle);
    
    offlineTextTitle->runAction(FadeIn::create(1));
}

void OfflineText::addTextBodyToScreen(std::string text)
{
    auto offlineTextBody = Label::createWithTTF(text, "fonts/azoomee.ttf", 57.1);
    offlineTextBody->setCascadeOpacityEnabled(true);
    offlineTextBody->setColor(Color3B(255,255,255));
    offlineTextBody->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.65);
    offlineTextBody->setOpacity(0);
    
    this->addChild(offlineTextBody);
    
    offlineTextBody->runAction(FadeIn::create(1.5));
}
